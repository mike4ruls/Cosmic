#include "Renderer.h"



Renderer::Renderer()
{
}

Renderer::Renderer(Camera * c, ID3D11Device * dev, ID3D11DeviceContext * con)
{
	cam = c;
	device = dev;
	context = con;
	Init();
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
}

void Renderer::Init()
{
	instanceRenderingOn = true;
	instanceThreshold = 5;
	LoadShaders();

	// The number of instances to draw with a single draw call
	// Code in Update() assumes this is a perfect cube (N*N*N)
	numInstances = 1500;

	// Create the local buffer to hold our data before copying
	// it to the GPU
	localInstanceData = new DirectX::XMFLOAT4X4[numInstances];

	// Set up a new vertex buffer that will hold our
	// PER-INSTANCE data.  From this code, the description 
	// looks about the same as other ones.  How we use it
	// and how the shader interprets it differ though!
	// It needs to be dynamic so we can put new data in
	// at run time
	D3D11_BUFFER_DESC instDesc = {};
	instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instDesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * numInstances;
	instDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instDesc.MiscFlags = 0;
	instDesc.StructureByteStride = 0;
	instDesc.Usage = D3D11_USAGE_DYNAMIC;
	device->CreateBuffer(&instDesc, 0, &instanceWorldMatrixBuffer);
}

void Renderer::Render(ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView)
{
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	for (auto& x : meshStorage) 
	{
			if (!x.second->broken && x.second->inUse)
			{
				if (x.second->canInstRender && instanceRenderingOn)
				{
					unsigned int count = 0;

					//delete[] localInstanceData; localInstanceData = nullptr;
					//localInstanceData = new DirectX::XMFLOAT4X4[numInstances];

					for(unsigned int i = 0; i < x.second->rendComponents.size(); i++)
					{
						if (x.second->rendComponents[i]->canRender)
						{
							XMStoreFloat4x4(&localInstanceData[count], XMLoadFloat4x4(&x.second->rendComponents[i]->worldMat));
							count++;
						}
					}
					D3D11_MAPPED_SUBRESOURCE mapped = {};
					context->Map(instanceWorldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

					// Copy to the resource


					memcpy(mapped.pData, localInstanceData, sizeof(DirectX::XMFLOAT4X4) * numInstances);

					// Unmap so the GPU can use it again
					context->Unmap(instanceWorldMatrixBuffer, 0);

					ID3D11Buffer* vbs[2] = {
						x.second->vertArr,	// Per-vertex data
						instanceWorldMatrixBuffer			// Per-instance data
					};

					// Two buffers means two strides and two offsets!
					UINT strides[2] = { sizeof(Vertex), sizeof(DirectX::XMFLOAT4X4) };
					UINT offsets[2] = { 0, 0 };

					// Set both vertex buffers
					context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
					context->IASetIndexBuffer(x.second->indArr, DXGI_FORMAT_R32_UINT, 0);

					instanceVShader->SetMatrix4x4("view", cam->viewMatrix);
					instanceVShader->SetMatrix4x4("projection", cam->projectionMatrix);

					instanceVShader->CopyAllBufferData();

					instanceVShader->SetShader();
					pixelShader->SetShader();

					context->DrawIndexedInstanced(
						x.second->indCount, // Number of indices from index buffer
						count,					// Number of instances to actually draw
						0, 0, 0);						// Offsets (unnecessary for us)

				}
				else 
				{
					for (unsigned int i = 0; i < x.second->rendComponents.size(); i++)
					{
						if (x.second->rendComponents[i]->canRender)
						{
							vertexShader->SetMatrix4x4("world", x.second->rendComponents[i]->worldMat);
							vertexShader->SetMatrix4x4("view", cam->viewMatrix);
							vertexShader->SetMatrix4x4("projection", cam->projectionMatrix);

							vertexShader->CopyAllBufferData();

							vertexShader->SetShader();
							pixelShader->SetShader();

							UINT stride = sizeof(Vertex);
							UINT offset = 0;
							context->IASetVertexBuffers(0, 1, &x.second->vertArr, &stride, &offset);
							context->IASetIndexBuffer(x.second->indArr, DXGI_FORMAT_R32_UINT, 0);

							context->DrawIndexed(
								x.second->indCount,     // The number of indices to use (we could draw a subset if we wanted)
								0,     // Offset to the first index we want to use
								0);    // Offset to add to each index when looking up vertices
						}
					}
				}
			}
		}
	}

void Renderer::LoadMesh(Mesh * newMesh)
{
	meshStorage[newMesh->meshName] = newMesh;
}

Mesh * Renderer::GetMesh(std::string name)
{
	return meshStorage.find(name)->second;
}

unsigned int Renderer::PushToRenderer(std::string meshName, RenderingComponent * com)
{
	unsigned int pos = (unsigned int)meshStorage[meshName]->rendComponents.size();
	meshStorage[meshName]->rendComponents.push_back(com);
	meshStorage[meshName]->instances++;

	meshStorage[meshName]->inUse = meshStorage[meshName]->instances != 0 ? true : false;
	meshStorage[meshName]->canInstRender = meshStorage[meshName]->instances >= instanceThreshold ? true : false;

	return pos;
}

void Renderer::RemoveFromRenderer(std::string meshName, unsigned int Id)
{
	meshStorage[meshName]->rendComponents.erase(meshStorage[meshName]->rendComponents.begin()+Id);

	meshStorage[meshName]->inUse = meshStorage[meshName]->instances != 0 ? true : false;
	meshStorage[meshName]->canInstRender = meshStorage[meshName]->instances >= instanceThreshold ? true : false;
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
