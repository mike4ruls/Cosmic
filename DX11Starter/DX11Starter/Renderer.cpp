#include "Renderer.h"



Renderer::Renderer()
{
}

Renderer::Renderer(Camera * c, ID3D11Device * dev, ID3D11DeviceContext * con, ID3D11RenderTargetView* backB, ID3D11DepthStencilView* depthS)
{
	cam = c;
	device = dev;
	context = con;
	backBufferRTV = backB;
	depthStencilView = depthS;


	Init();
	SetWireFrame();
}


Renderer::~Renderer()
{
	for (auto& x : meshStorage) 
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}
	delete vertexShader;
	delete instanceVShader;
	delete pixelShader;
	instanceWorldMatrixBuffer->Release();
	delete[] localInstanceData;
	wireFrame->Release();
	fillFrame->Release();
}

void Renderer::Init()
{
	instanceRenderingOn = true;
	defferedRenderingOn = false;
	HdrOn = false;
	BloomOn = false;
	wireFrameOn = false;
	prevWireStatus = wireFrameOn;
	instanceThreshold = 5;
	LoadShaders();

	D3D11_RASTERIZER_DESC fillDesc;
	fillDesc.FillMode = D3D11_FILL_SOLID;
	fillDesc.CullMode = D3D11_CULL_FRONT;
	fillDesc.FrontCounterClockwise = true;
	fillDesc.DepthBias = 0;
	fillDesc.DepthBiasClamp = 0.0f;
	fillDesc.SlopeScaledDepthBias = 0.0f;
	fillDesc.DepthClipEnable = true;
	fillDesc.ScissorEnable = false;
	fillDesc.MultisampleEnable = false;
	fillDesc.AntialiasedLineEnable = false;

	device->CreateRasterizerState(&fillDesc, &fillFrame);

	D3D11_RASTERIZER_DESC wireDesc;
	wireDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireDesc.CullMode = D3D11_CULL_FRONT;
	wireDesc.FrontCounterClockwise = true;
	wireDesc.DepthBias = 0;
	wireDesc.DepthBiasClamp = 0.0f;
	wireDesc.SlopeScaledDepthBias = 0.0f;
	wireDesc.DepthClipEnable = true;
	wireDesc.ScissorEnable = false;
	wireDesc.MultisampleEnable = false;
	wireDesc.AntialiasedLineEnable = false;

	device->CreateRasterizerState(&wireDesc, &wireFrame);

	// The number of instances to draw with a single draw call
	// Code in Update() assumes this is a perfect cube (N*N*N)
	numInstances = 1500;

	// Create the local buffer to hold our data before copying
	// it to the GPU
	localInstanceData = new InstanceData[numInstances];

	// Set up a new vertex buffer that will hold our
	// PER-INSTANCE data.  From this code, the description 
	// looks about the same as other ones.  How we use it
	// and how the shader interprets it differ though!
	// It needs to be dynamic so we can put new data in
	// at run time
	D3D11_BUFFER_DESC instDesc = {};
	instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instDesc.ByteWidth = sizeof(InstanceData) * numInstances;
	instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instDesc.MiscFlags = 0;
	instDesc.StructureByteStride = 0;
	instDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&instDesc, 0, &instanceWorldMatrixBuffer);
}

void Renderer::Render(float dt)
{

	if (wireFrameOn != prevWireStatus)
	{
		SetWireFrame();
	}

	////////////////////////////////////////
	//CLEARING SCREEN
	////////////////////////////////////////
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	////////////////////////////////////////
	//DRAWING OBJECTS TO SCREEN
	////////////////////////////////////////

	for (auto& x : meshStorage) 
	{
			if (!x.second->broken && x.second->inUse)
			{
				if (x.second->canInstRender && instanceRenderingOn)
				{
					unsigned int count = 0;

					for(unsigned int i = 0; i < x.second->rendComponents.size(); i++)
					{
						if (x.second->rendComponents[i]->canRender)
						{
							if (x.second->rendComponents[i]->mat.materialType != Material::Transulcent) 
							{
								XMStoreFloat4x4(&localInstanceData[count].worldMat, XMLoadFloat4x4(&x.second->rendComponents[i]->worldMat));
								XMStoreFloat4x4(&localInstanceData[count].invTrans, XMLoadFloat4x4(&x.second->rendComponents[i]->worldInvTrans));
								count++;
							}
						}
					}
					if(defferedRenderingOn)
					{
						DrawDInstance(x.second->meshName, localInstanceData, count);
					}
					else 
					{
						DrawFInstance(x.second->meshName, localInstanceData, count);
					}
				}
				else 
				{
					for (unsigned int i = 0; i < x.second->rendComponents.size(); i++)
					{
						if (x.second->rendComponents[i]->canRender)
						{
							if (x.second->rendComponents[i]->mat.materialType != Material::Transulcent)
							{
								if (defferedRenderingOn)
								{
									DrawDefferedPass(x.second->rendComponents[i]);
								}
								else
								{
									DrawForwardPass(x.second->rendComponents[i]);
								}
							}	
						}
					}
				}
			}
		}

	////////////////////////////////////////
	//POST PROCESSING
	////////////////////////////////////////

	// Deffered rendering lighting pass //
	if(defferedRenderingOn)
	{
		/*Insert code here*/
	}

	// Drawing translucent objects with forward rendering
	for (unsigned int i = 0; i < transRendComponents.size(); i++)
	{
		if (transRendComponents[i]->canRender)
		{
			DrawForwardPass(transRendComponents[i]);
		}
	}

	// HDR //

	if (HdrOn)
	{
		/*Insert code here*/
	}


	// Bloom //

	if (BloomOn)
	{
		/*Insert code here*/
	}

	////////////////////////////////////////
	//DRAWING SKY BOX
	////////////////////////////////////////

	DrawSkyBox();

	}

void Renderer::LoadMesh(Mesh * newMesh)
{
	meshStorage[newMesh->meshName] = newMesh;
}

Mesh * Renderer::GetMesh(std::string name)
{
	return meshStorage.find(name)->second;
}

unsigned int Renderer::PushToRenderer(RenderingComponent * com)
{
	unsigned int pos = (unsigned int)meshStorage[com->meshName]->rendComponents.size();

	if (com->mat.materialType == Material::Transulcent)
	{
		com->mat.translucentID = PushToTranslucent(com);		
	}

	meshStorage[com->meshName]->rendComponents.push_back(com);
	meshStorage[com->meshName]->instances++;
				
	meshStorage[com->meshName]->inUse = meshStorage[com->meshName]->instances != 0 ? true : false;
	meshStorage[com->meshName]->canInstRender = meshStorage[com->meshName]->instances >= instanceThreshold ? true : false;

	return pos;
}

unsigned int Renderer::PushToTranslucent(RenderingComponent * com)
{
	unsigned int pos = (unsigned int)transRendComponents.size();
	transRendComponents.push_back(com);
	return pos;
}

void Renderer::RemoveFromRenderer(std::string meshName, unsigned int Id)
{
	if (meshStorage[meshName]->rendComponents[Id]->mat.materialType == Material::Transulcent)
	{
		RemoveFromTranslucent(meshStorage[meshName]->rendComponents[Id]->mat.translucentID);
	}

	meshStorage[meshName]->rendComponents.erase(meshStorage[meshName]->rendComponents.begin()+Id);

	meshStorage[meshName]->inUse = meshStorage[meshName]->instances != 0 ? true : false;
	meshStorage[meshName]->canInstRender = meshStorage[meshName]->instances >= instanceThreshold ? true : false;
}

void Renderer::RemoveFromTranslucent(unsigned int Id)
{
	transRendComponents.erase(transRendComponents.begin() + Id);
}

void Renderer::LoadShaders()
{

	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");


	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	instanceVShader = new SimpleVertexShader(device, context);
	instanceVShader->LoadShaderFile(L"InstanceVShader.cso");

}

void Renderer::SetWireFrame()
{
	if (wireFrameOn)
	{
		context->RSSetState(wireFrame);
	}
	else
	{
		context->RSSetState(fillFrame);
	}
	prevWireStatus = wireFrameOn;
}

void Renderer::ToggleWireFrame()
{
	wireFrameOn = wireFrameOn ? false : true;
	SetWireFrame();
}

void Renderer::DrawSkyBox()
{
}

void Renderer::DrawForwardPass(RenderingComponent* component)
{
	vertexShader->SetMatrix4x4("world", component->worldMat);
	vertexShader->SetMatrix4x4("view", cam->viewMatrix);
	vertexShader->SetMatrix4x4("projection", cam->projectionMatrix);
	vertexShader->SetMatrix4x4("worldInvTrans", component->worldInvTrans);

	vertexShader->SetFloat3("camPosition",cam->transform.position);
	vertexShader->SetFloat3("dirLight", {1.0f,-1.0f,0.0f});

	vertexShader->CopyAllBufferData();

	vertexShader->SetShader();
	pixelShader->SetShader();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &meshStorage[component->meshName]->vertArr, &stride, &offset);
	context->IASetIndexBuffer(meshStorage[component->meshName]->indArr, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		meshStorage[component->meshName]->indCount,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
	
}

void Renderer::DrawFInstance(std::string meshName, InstanceData * components, unsigned int count)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	context->Map(instanceWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

	// Copy to the resource


	memcpy(mapped.pData, components, sizeof(InstanceData) * numInstances);

	// Unmap so the GPU can use it again
	context->Unmap(instanceWorldMatrixBuffer, 0);

	ID3D11Buffer* vbs[2] = {
		meshStorage[meshName]->vertArr,	// Per-vertex data
		instanceWorldMatrixBuffer			// Per-instance data
	};

	// Two buffers means two strides and two offsets!
	UINT strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
	UINT offsets[2] = { 0, 0 };

	// Set both vertex buffers
	context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	context->IASetIndexBuffer(meshStorage[meshName]->indArr, DXGI_FORMAT_R32_UINT, 0);

	instanceVShader->SetMatrix4x4("view", cam->viewMatrix);
	instanceVShader->SetMatrix4x4("projection", cam->projectionMatrix);

	instanceVShader->SetFloat3("camPosition", cam->transform.position);
	instanceVShader->SetFloat3("dirLight", { 1.0f,-1.0f,0.0f });

	instanceVShader->CopyAllBufferData();

	instanceVShader->SetShader();
	pixelShader->SetShader();

	context->DrawIndexedInstanced(
		meshStorage[meshName]->indCount, // Number of indices from index buffer
		count,					// Number of instances to actually draw
		0, 0, 0);						// Offsets (unnecessary for us)
}

void Renderer::DrawDefferedPass(RenderingComponent * component)
{
}

void Renderer::DrawDInstance(std::string meshName, InstanceData * components, unsigned int count)
{
}

