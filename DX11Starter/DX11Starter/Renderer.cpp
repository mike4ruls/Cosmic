#include "Renderer.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"


// https://www.gamedev.net/forums/topic/644740-dx11-multiple-render-targets/
Renderer::Renderer()
{
}

Renderer::Renderer(Camera * c, ID3D11Device * dev, ID3D11DeviceContext * con, ID3D11RenderTargetView* backB, ID3D11DepthStencilView* depthS, IDXGISwapChain* sw)
{
	cam = c;
	device = dev;
	context = con;
	backBufferRTV = backB;
	depthStencilView = depthS;
	swap = sw;

	assets = new AssetManager(dev, con);

	Init();
	InitSkyBox();
	SetWireFrame();
	//CreateRenderTargets();
}


Renderer::~Renderer()
{
	/*for (auto& x : meshStorage) 
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}
	for (auto& x : surTextStorage)
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}
	for (auto& x : norTextStorage)
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}
	for (auto& x : skyTextStorage)
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}*/

	if (assets != nullptr) { delete assets; assets = nullptr; }

	for(unsigned int i = 0; i < allLights.size(); i++)
	{
		if (allLights[i] != nullptr) { delete allLights[i]; allLights[i] = nullptr; }
	}

	allLights.clear();
	directionalLights.clear();
	pointLights.clear();
	spotLights.clear();

	delete  vertexFShader;
	delete  instanceFVShader;
	delete  vertexDShader;
	delete  instanceDVShader;
	delete  skyVShader;
	delete  quadVShader;


	delete pixelFShader;
	delete pixelFSShader;
	delete pixelFSNShader;
	delete pixelDShader;
	delete pLightingShader;
	delete skyPShader;
	delete bloomShader;
	delete hdrShader;

	delete[] localInstanceData;

	instanceWorldMatrixBuffer->Release();

	if (wireFrame != nullptr) { wireFrame->Release(); wireFrame = nullptr; }
	if (fillFrame != nullptr) { fillFrame->Release(); fillFrame = nullptr; }
	//if (skyBoxSVR != nullptr) { skyBoxSVR->Release(); skyBoxSVR = nullptr; }
	if (textureSample != nullptr) { textureSample->Release(); textureSample = nullptr; }
	if (sample != nullptr) { sample->Release(); sample = nullptr; }
	if (skyRast != nullptr) { skyRast->Release(); skyRast = nullptr; }
	if (skyDepth != nullptr) { skyDepth->Release(); skyDepth = nullptr; }
}

void Renderer::Init()
{
	instanceRenderingOn = true;
	defferedRenderingOn = false;
	HdrOn = false;
	BloomOn = false;
	wireFrameOn = false;
	skyBoxOn = true;
	prevWireStatus = wireFrameOn;
	instanceThreshold = 5;
	skyBoxNum = 1;

	LoadShaders();
	sunLight = CreateDirectionalLight({0.0f,-1.0f,0.0f });
	//sunLight->ligComponent->lightColor = { 0.05f, 0.5f, 0.0f, 1.0f };
	sunLight->ligComponent->lightAmb = {0.2f, 0.2f, 0.2f, 1.0f};

	maxSize = 10;

	D3D11_SAMPLER_DESC tDes = {};
	tDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tDes.Filter = D3D11_FILTER_ANISOTROPIC;
	tDes.MaxAnisotropy = 16;
	tDes.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&tDes, &textureSample);

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
	//Compiling Lights
	////////////////////////////////////////
	CompileLights();

	////////////////////////////////////////
	//DRAWING OBJECTS TO SCREEN
	////////////////////////////////////////

	for (auto& x : assets->meshStorage) 
	{
			if (!x.second->broken && x.second->inUse)
			{
				if (x.second->canInstRender && instanceRenderingOn)
				{
					unsigned int count = 0;

					for(unsigned int i = 0; i < x.second->rendComponents->size(); i++)
					{
						if (x.second->rendComponents->at(i)->canRender)
						{
							if (x.second->rendComponents->at(i)->mat.materialType != Material::Transulcent)
							{
								XMStoreFloat4x4(&localInstanceData[count].worldMat, XMLoadFloat4x4(&x.second->rendComponents->at(i)->worldMat));
								XMStoreFloat4x4(&localInstanceData[count].invTrans, XMLoadFloat4x4(&x.second->rendComponents->at(i)->worldInvTrans));
								XMStoreFloat4(&localInstanceData[count].color, XMLoadFloat4(&x.second->rendComponents->at(i)->mat.surfaceColor));
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
					for (unsigned int i = 0; i < x.second->rendComponents->size(); i++)
					{
						if (x.second->rendComponents->at(i)->canRender)
						{
							if (x.second->rendComponents->at(i)->mat.materialType != Material::Transulcent)
							{
								if (defferedRenderingOn)
								{
									DrawDefferedPass(x.second->rendComponents->at(i));
								}
								else
								{
									DrawForwardPass(x.second->rendComponents->at(i));
								}
							}	
						}
					}
				}
			}
		}
	////////////////////////////////////////
	//DRAWING SKY BOX
	////////////////////////////////////////

	if(skyBoxOn)
	{
		DrawSkyBox();
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



	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swap->Present(0, 0);
	}

	void Renderer::CreateRenderTargets()
	{
		// The above function created the back buffer render target
		// for us, but we need a reference to it
		ID3D11Texture2D* colorTexture;
		swap->GetBuffer(
			1,
			__uuidof(ID3D11Texture2D),
			(void**)&colorTexture);

		// Now that we have the texture, create a render target view
		// for the back buffer so we can render into it.  Then release
		// our local reference to the texture, since we have the view.
		device->CreateRenderTargetView(
			colorTexture,
			0,
			&colorRTV);
		colorTexture->Release();

		//==================================================================================
		//
		//==================================================================================
		D3D11_TEXTURE2D_DESC worldTextDesc = {};
		worldTextDesc.Width = cam->width;
		worldTextDesc.Height = cam->height;
		worldTextDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		worldTextDesc.MipLevels = 1;
		worldTextDesc.ArraySize = 1;
		worldTextDesc.SampleDesc.Count = 1;
		worldTextDesc.Usage = D3D11_USAGE_DEFAULT;
		worldTextDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		worldTextDesc.CPUAccessFlags = 0;
		worldTextDesc.MiscFlags = 0;

		ID3D11Texture2D* worldTexture;
		/*swap->GetBuffer(
			2,
			__uuidof(ID3D11Texture2D),
			(void**)&worldTexture);*/
		device->CreateTexture2D(&worldTextDesc, NULL, &worldTexture);

		device->CreateRenderTargetView(
			worldTexture,
			0,
			&worldRTV);
		worldTexture->Release();

		//==================================================================================
		//
		//==================================================================================

		ID3D11Texture2D* normalTexture;
		swap->GetBuffer(
			3,
			__uuidof(ID3D11Texture2D),
			(void**)&normalTexture);

		device->CreateRenderTargetView(
			normalTexture,
			0,
			&normalRTV);
		normalTexture->Release();

	}

//void Renderer::LoadMesh(Mesh * newMesh)////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//{
//	meshStorage[newMesh->meshName] = newMesh;
//}
//
//Mesh * Renderer::GetMesh(std::string name)
//{
//	return meshStorage.find(name)->second;
//}

void Renderer::PushToRenderer(RenderingComponent * com)
{
	com->rendID = (unsigned int)assets->meshStorage[com->meshName]->rendComponents->size();

	if (com->mat.materialType == Material::Transulcent)
	{
		PushToTranslucent(com);		
	}

	assets->meshStorage[com->meshName]->rendComponents->push_back(com);
	assets->meshStorage[com->meshName]->instances++;
				
	assets->meshStorage[com->meshName]->inUse = assets->meshStorage[com->meshName]->instances != 0 ? true : false;
	assets->meshStorage[com->meshName]->canInstRender = assets->meshStorage[com->meshName]->instances >= instanceThreshold ? true : false;

}

void Renderer::PushToTranslucent(RenderingComponent * com)
{
	com->mat.translucentID = (unsigned int)transRendComponents.size();
	transRendComponents.push_back(com);
}

void Renderer::Flush()
{
	for (auto& x : assets->meshStorage)
	{
		if(x.second->inUse)
		{
			x.second->rendComponents->clear();
			x.second->instances = 0;
			x.second->canInstRender = false;
		}
	}

	for (unsigned int i = 0; i < allLights.size(); i++)
	{
		if (allLights[i] != nullptr) { delete allLights[i]; allLights[i] = nullptr; }
	}

	allLights.clear();
	directionalLights.clear();
	pointLights.clear();
	spotLights.clear();



	sunLight = CreateDirectionalLight({ 0.0f,-1.0f,0.0f });
	//sunLight->ligComponent->lightColor = { 0.05f, 0.5f, 0.0f, 1.0f };
	sunLight->ligComponent->lightAmb = { 0.2f, 0.2f, 0.2f, 1.0f };
}

#pragma region Light Stuff

Light * Renderer::CreateLight(Light::LightType lType)
{
	if (allLights.size() == (maxDLights + maxPLights + maxSLights)) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	if (lType == Light::Directional)
	{
		newLight->lightID = (unsigned int)allLights.size();
		newLight->compID = (unsigned int)directionalLights.size();

		allLights.push_back(newLight);
		directionalLights.push_back(Light::LightComponent());

		newLight->ligComponent = &directionalLights[newLight->compID];

		newLight->lightType = Light::Directional;
		newLight->lightOn = true;

		newLight->ligComponent->lightDir = { 1.0f, -1.0f, 0.0f };
		newLight->ligComponent->lightColor = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightIntensity = 4.0f;


	}
	else if(lType == Light::Point)
	{
		newLight->lightID = (unsigned int)allLights.size();
		newLight->compID = (unsigned int)pointLights.size();

		allLights.push_back(newLight);
		pointLights.push_back(Light::LightComponent());

		newLight->ligComponent = &pointLights[newLight->compID];

		newLight->lightType = Light::Point;
		newLight->lightOn = true;

		newLight->ligComponent->lightPos = { 0.0f, 0.0f, 0.0f };
		newLight->ligComponent->lightColor = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightIntensity = 4.0f;



	}
	else if(lType == Light::Spot)
	{
		newLight->lightID = (unsigned int)allLights.size();
		newLight->compID = (unsigned int)spotLights.size();

		allLights.push_back(newLight);
		spotLights.push_back(Light::LightComponent());

		newLight->ligComponent = &spotLights[newLight->compID];

		newLight->lightType = Light::Spot;
		newLight->lightOn = true;

		newLight->ligComponent->lightPos = { 0.0f, 0.0f, 0.0f };
		newLight->ligComponent->lightColor = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
		newLight->ligComponent->lightIntensity = 4.0f;

	}

	return newLight;
}

Light * Renderer::CreateDirectionalLight(DirectX::XMFLOAT3 direction)
{
	if (directionalLights.size() == maxDLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)directionalLights.size();

	allLights.push_back(newLight);
	directionalLights.push_back(Light::LightComponent());

	newLight->ligComponent = &directionalLights[newLight->compID];

	newLight->lightType = Light::Directional;
	newLight->lightOn = true;

	newLight->ligComponent->lightDir = direction;
	newLight->ligComponent->lightColor = {0.5f, 0.5f, 0.5f, 0.5f};
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreateDirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 ligColor)
{
	if (directionalLights.size() == maxDLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)directionalLights.size();

	allLights.push_back(newLight);
	directionalLights.push_back(Light::LightComponent());

	newLight->ligComponent = &directionalLights[newLight->compID];


	newLight->lightType = Light::Directional;
	newLight->lightOn = true;

	newLight->ligComponent->lightDir = direction;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreateDirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 ligColor, float inten)
{
	if (directionalLights.size() == maxDLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)directionalLights.size();

	allLights.push_back(newLight);
	directionalLights.push_back(Light::LightComponent());

	newLight->ligComponent = &directionalLights[newLight->compID];

	newLight->lightType = Light::Directional;
	newLight->lightOn = true;

	newLight->ligComponent->lightDir = direction;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightIntensity = inten;
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };

	return newLight;
}

Light * Renderer::CreatePointLight(DirectX::XMFLOAT3 position)
{
	if (pointLights.size() == maxPLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)pointLights.size();

	allLights.push_back(newLight);
	pointLights.push_back(Light::LightComponent());

	newLight->ligComponent = &pointLights[newLight->compID];


	newLight->lightType = Light::Point;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreatePointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor)
{
	if (pointLights.size() == maxPLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)pointLights.size();

	allLights.push_back(newLight);
	pointLights.push_back(Light::LightComponent());

	newLight->ligComponent = &pointLights[newLight->compID];

	newLight->lightType = Light::Point;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightPos = { 0.0f, 0.0f, 0.0f };
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreatePointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor, float inten)
{
	if (pointLights.size() == maxPLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)pointLights.size();

	allLights.push_back(newLight);
	pointLights.push_back(Light::LightComponent());

	newLight->ligComponent = &pointLights[newLight->compID];

	newLight->lightType = Light::Point;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightIntensity = inten;
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };

	return newLight;
}

Light * Renderer::CreateSpotLight(DirectX::XMFLOAT3 position)
{
	if (spotLights.size() == maxSLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)spotLights.size();

	allLights.push_back(newLight);
	spotLights.push_back(Light::LightComponent());

	newLight->ligComponent = &spotLights[newLight->compID];

	newLight->lightType = Light::Spot;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreateSpotLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor)
{
	if (spotLights.size() == maxSLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)spotLights.size();

	allLights.push_back(newLight);
	spotLights.push_back(Light::LightComponent());

	newLight->ligComponent = &spotLights[newLight->compID];

	newLight->lightType = Light::Spot;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };
	newLight->ligComponent->lightIntensity = 4.0f;

	return newLight;
}

Light * Renderer::CreateSpotLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor, float inten)
{
	if (spotLights.size() == maxSLights) { return nullptr; printf("ERROR: Could not create new light.\nReason: Maximum capacity"); }

	Light* newLight = new Light();

	newLight->lightID = (unsigned int)allLights.size();
	newLight->compID = (unsigned int)spotLights.size();

	allLights.push_back(newLight);
	spotLights.push_back(Light::LightComponent());

	newLight->ligComponent = &spotLights[newLight->compID];

	newLight->lightType = Light::Spot;
	newLight->lightOn = true;

	newLight->ligComponent->lightPos = position;
	newLight->ligComponent->lightColor = ligColor;
	newLight->ligComponent->lightIntensity = inten;
	newLight->ligComponent->lightAmb = { 0.5f, 0.5f, 0.5f, 0.5f };

	return newLight;
}

void Renderer::RemoveLight(Light * light)
{
	allLights.erase(allLights.begin() + light->lightID);



	if(light->lightType == Light::Directional)
	{
		directionalLights.erase(directionalLights.begin() + light->compID);
	}
	else if(light->lightType == Light::Point)
	{
		pointLights.erase(pointLights.begin() + light->compID);
	}
	else if(light->lightType == Light::Spot)
	{
		spotLights.erase(spotLights.begin() + light->compID);
	}



	if (light != nullptr) { delete light; light = nullptr; }
}

#pragma endregion

void Renderer::RemoveFromRenderer(std::string meshName, unsigned int Id)
{
	if (assets->meshStorage[meshName]->rendComponents->at(Id)->mat.materialType == Material::Transulcent)
	{
		RemoveFromTranslucent(assets->meshStorage[meshName]->rendComponents->at(Id)->mat.translucentID);
	}

	assets->meshStorage[meshName]->rendComponents->erase(assets->meshStorage[meshName]->rendComponents->begin()+Id);

	for (unsigned int i = 0; i < assets->meshStorage[meshName]->rendComponents->size(); i++) 
	{
		assets->meshStorage[meshName]->rendComponents->at(i)->rendID = i;
	}

	assets->meshStorage[meshName]->inUse = assets->meshStorage[meshName]->instances != 0 ? true : false;
	assets->meshStorage[meshName]->canInstRender = assets->meshStorage[meshName]->instances >= instanceThreshold ? true : false;
}

void Renderer::RemoveFromTranslucent(unsigned int Id)
{
	transRendComponents.erase(transRendComponents.begin() + Id);

	for (unsigned int i = 0; i < transRendComponents.size(); i++)
	{
		transRendComponents[i]->mat.translucentID = i;
	}
}

void Renderer::LoadShaders()
{
	//=================================================
	// Init Vertex Shaders
	//=================================================

	vertexFShader = new SimpleVertexShader(device, context);
	vertexFShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/ForwardShaders/VertexFShader.cso");

	instanceFVShader = new SimpleVertexShader(device, context);
	instanceFVShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/ForwardShaders/InstanceFVShader.cso");

	vertexDShader = new SimpleVertexShader(device, context);
	vertexDShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/DeferredShaders/VertexDShader.cso");

	instanceDVShader = new SimpleVertexShader(device, context);
	instanceDVShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/DeferredShaders/InstanceDVShader.cso");

	skyVShader = new SimpleVertexShader(device, context);
	skyVShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/SkyBoxShaders/SkyVShader.cso");

	quadVShader = new SimpleVertexShader(device, context);
	quadVShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/PostProcessShaders/QuadVShader.cso");


	//=================================================
	// Init Pixel Shaders
	//=================================================

	pixelFShader = new SimplePixelShader(device, context);
	pixelFShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/ForwardShaders/PixelFShader.cso");

	pixelFSShader = new SimplePixelShader(device, context);
	pixelFSShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/ForwardShaders/PixelFSShader.cso");

	pixelFSNShader = new SimplePixelShader(device, context);
	pixelFSNShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/ForwardShaders/PixelFSNShader.cso");

	pixelDShader = new SimplePixelShader(device, context);
	pixelDShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/DeferredShaders/PixelDShader.cso");

	pLightingShader = new SimplePixelShader(device, context);
	pLightingShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/DeferredShaders/PLightingShader.cso");

	skyPShader = new SimplePixelShader(device, context);
	skyPShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/SkyBoxShaders/SkyPShader.cso");

	bloomShader = new SimplePixelShader(device, context);
	bloomShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/PostProcessShaders/BloomPShader.cso");

	hdrShader = new SimplePixelShader(device, context);
	hdrShader->LoadShaderFile(L"../DX11Starter/Debug/Shaders/PostProcessShaders/HdrPShader.cso");
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

void Renderer::CompileLights()
{
	for (unsigned int i = 0; i < allLights.size(); i++)
	{
		if (allLights[i]->lightOn)
		{
			//dArr[ligDcount] = directionalLights[i]->dLComponent;
			/*if (ligDcount == maxSize)
			{
				break;
			}
			ligDcount++;*/
		}
	}
}

void Renderer::SetLights(SimplePixelShader* pixel)
{
	int dCount = (int)directionalLights.size();
	pixel->SetInt("dCount", dCount);
	if (dCount != 0)
	{
		pixel->SetData("dirLights", &directionalLights[0], sizeof(Light::LightComponent) * maxDLights);
	}


	int pCount = (int)pointLights.size();
	pixel->SetInt("pCount", pCount);
	if (pCount != 0)
	{
		pixel->SetData("pointLights", &pointLights[0], sizeof(Light::LightComponent) * maxPLights);
	}

	int sCount = (int)spotLights.size();
	pixel->SetInt("sCount", sCount);
	if (sCount != 0)
	{
		pixel->SetData("spotLights", &spotLights[0], sizeof(Light::LightComponent) * maxSLights);
	}
}

void Renderer::DrawForwardPass(RenderingComponent* component)
{
	vertexFShader->SetMatrix4x4("world", component->worldMat);
	vertexFShader->SetMatrix4x4("view", cam->viewMatrix);
	vertexFShader->SetMatrix4x4("projection", cam->projectionMatrix);
	vertexFShader->SetMatrix4x4("worldInvTrans", component->worldInvTrans);

	vertexFShader->SetFloat4("surColor", component->mat.surfaceColor);
	vertexFShader->SetFloat3("camPosition",cam->transform.position);

	vertexFShader->CopyAllBufferData();

	SimplePixelShader* currentPixel = nullptr;
	if (!component->mat.hasSurText)
	{
		currentPixel = pixelFShader;
	}
	else if (!component->mat.hasNorText)
	{
		currentPixel = pixelFSShader;
		currentPixel->SetShaderResourceView("surfaceTexture", component->mat.GetSurfaceTexture());
		currentPixel->SetFloat("uvXOffset", component->mat.uvXOffSet);
		currentPixel->SetFloat("uvYOffset", component->mat.uvYOffSet);
	}
	else
	{
		currentPixel = pixelFSNShader;
		currentPixel->SetShaderResourceView("surfaceTexture", component->mat.GetSurfaceTexture());
		currentPixel->SetShaderResourceView("normalTexture", component->mat.GetNormalTexture());
		currentPixel->SetFloat("uvXOffset", component->mat.uvXOffSet);
		currentPixel->SetFloat("uvYOffset", component->mat.uvYOffSet);
	}

	currentPixel->SetSamplerState("basicSampler", textureSample);
	currentPixel->SetShaderResourceView("skyTexture", skyBoxSVR);
	currentPixel->SetFloat("reflectance", component->mat.surfaceReflectance);

	SetLights(currentPixel);
	currentPixel->CopyAllBufferData();

	vertexFShader->SetShader();
	currentPixel->SetShader();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &assets->meshStorage[component->meshName]->vertArr, &stride, &offset);
	context->IASetIndexBuffer(assets->meshStorage[component->meshName]->indArr, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		assets->meshStorage[component->meshName]->indCount,     // The number of indices to use (we could draw a subset if we wanted)
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
		assets->meshStorage[meshName]->vertArr,	// Per-vertex data
		instanceWorldMatrixBuffer			// Per-instance data
	};

	// Two buffers means two strides and two offsets!
	UINT strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
	UINT offsets[2] = { 0, 0 };

	// Set both vertex buffers
	context->IASetVertexBuffers(0, 2, vbs, strides, offsets);
	context->IASetIndexBuffer(assets->meshStorage[meshName]->indArr, DXGI_FORMAT_R32_UINT, 0);

	instanceFVShader->SetMatrix4x4("view", cam->viewMatrix);
	instanceFVShader->SetMatrix4x4("projection", cam->projectionMatrix);
			
	instanceFVShader->SetFloat3("camPosition", cam->transform.position);
			
	instanceFVShader->CopyAllBufferData();

	SetLights(pixelFShader);
	pixelFShader->CopyAllBufferData();

	instanceFVShader->SetShader();
	pixelFShader->SetShader();

	context->DrawIndexedInstanced(
		assets->meshStorage[meshName]->indCount, // Number of indices from index buffer
		count,					// Number of instances to actually draw
		0, 0, 0);						// Offsets (unnecessary for us)
}

void Renderer::DrawDefferedPass(RenderingComponent * component)
{
}

void Renderer::DrawDInstance(std::string meshName, InstanceData * components, unsigned int count)
{
}

void Renderer::InitSkyBox()
{
	D3D11_SAMPLER_DESC sDes = {};
	sDes.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sDes.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sDes.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sDes.Filter = D3D11_FILTER_ANISOTROPIC;
	sDes.MaxAnisotropy = 16;
	sDes.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sDes, &sample);

	// Create a rasterizer state so we can render backfaces
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &skyRast);

	// Create a depth state so that we can accept pixels
	// at a depth less than or EQUAL TO an existing depth
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // Make sure we can see the sky (at max depth)
	device->CreateDepthStencilState(&dsDesc, &skyDepth);

	ToggleSkyBox();
}

void Renderer::ToggleSkyBox()
{
	//if (skyBoxSVR != nullptr) { skyBoxSVR->Release(); skyBoxSVR = nullptr; }
	switch (skyBoxNum)
	{
	case 1:
	{
		skyBoxSVR = assets->GetSkyBoxTexture("sunny");
		//CreateDDSTextureFromFile(device, L"Textures/Mars.dds", 0, &skyBoxSVR);
		break;
	}
	case 2:
	{
		
		skyBoxSVR = assets->GetSkyBoxTexture("mars");
		break;
	}
	case 3:
	{
		skyBoxSVR = assets->GetSkyBoxTexture("space");
		break;
	}
	}

	skyBoxNum += 1;
	if (skyBoxNum > 3)
	{
		skyBoxNum = 1;
	}

}

void Renderer::LoadSkyBox(int skyNum)
{
	//if (skyBoxSVR != nullptr) { skyBoxSVR->Release(); skyBoxSVR = nullptr; }

	skyBoxNum = skyNum;

	switch (skyBoxNum)
	{
	case 1:
	{
		skyBoxSVR = assets->GetSkyBoxTexture("sunny");
		//DirectX::CreateDDSTextureFromFile(device, L"Assets/Textures/SkyBox/SunnyCubeMap.dds", 0, &skyBoxSVR);
		//CreateDDSTextureFromFile(device, L"Textures/Mars.dds", 0, &skyBoxSVR);
		break;
	}
	case 2:
	{
		skyBoxSVR = assets->GetSkyBoxTexture("mars");
		//DirectX::CreateDDSTextureFromFile(device, L"Assets/Textures/SkyBox/Mars.dds", 0, &skyBoxSVR);
		break;
	}
	case 3:
	{
		skyBoxSVR = assets->GetSkyBoxTexture("space");
		//DirectX::CreateDDSTextureFromFile(device, L"Assets/Textures/SkyBox/space.dds", 0, &skyBoxSVR);
		break;
	}
	}

	skyBoxNum += 1;
	if (skyBoxNum > 3)
	{
		skyBoxNum = 1;
	}
}

void Renderer::DrawSkyBox()
{
	ID3D11RasterizerState* oldR;

	context->RSGetState(&oldR);

	ID3D11Buffer* skyVB = assets->meshStorage["Cube"]->GetVertexBuffer();
	ID3D11Buffer* skyIB = assets->meshStorage["Cube"]->GetIndicesBuffer();
	unsigned int count = assets->meshStorage["Cube"]->indCount;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	skyVShader->SetMatrix4x4("view", cam->viewMatrix);
	skyVShader->SetMatrix4x4("projection", cam->projectionMatrix);
	skyVShader->CopyAllBufferData();
	skyVShader->SetShader();
		 
	skyPShader->SetShaderResourceView("Sky", skyBoxSVR);
	skyPShader->SetSamplerState("Sampler", sample);
	skyPShader->CopyAllBufferData();
	skyPShader->SetShader();

	context->RSSetState(skyRast);
	context->OMSetDepthStencilState(skyDepth, 0);

	context->DrawIndexed(count, 0, 0);

	context->RSSetState(oldR);
	context->OMSetDepthStencilState(0, 0);

	oldR->Release();
}

