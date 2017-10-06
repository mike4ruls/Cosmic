#include "AssetManager.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"



AssetManager::AssetManager(ID3D11Device* dev, ID3D11DeviceContext* con)
{
	device = dev;
	context = con;
	Init();
}


AssetManager::~AssetManager()
{
	for (auto& x : meshStorage)
	{
		if (x.second) { delete x.second; x.second = nullptr; }
	}
	for (auto& x : surTextStorage)
	{
		if (x.second) { x.second->Release(); x.second = nullptr; }
	}
	for (auto& x : norTextStorage)
	{
		if (x.second) { x.second->Release(); x.second = nullptr; }
	}
	for (auto& x : skyTextStorage)
	{
		if (x.second) { x.second->Release(); x.second = nullptr; }
	}
}

void AssetManager::Init()
{
	LoadMeshes();
	LoadSurfaceTextures();
	LoadNormalTextures();
	LoadSkyBoxTextures();
}

void AssetManager::LoadMeshes()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	DirectX::XMFLOAT4 red = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 green = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 blue = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	DirectX::XMFLOAT3 genNorm = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT2 genUv = DirectX::XMFLOAT2(1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f), genNorm, genUv, red },
		{ DirectX::XMFLOAT3(+1.5f, -1.0f, +0.0f), genNorm, genUv, blue },
		{ DirectX::XMFLOAT3(-1.5f, -1.0f, +0.0f), genNorm, genUv, green },
	};

	Vertex vertices2[] =
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, +0.0f), genNorm, genUv, red },
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, +0.0f), genNorm, genUv, blue },
		{ DirectX::XMFLOAT3(1.0f, 1.0f, +0.0f), genNorm, genUv, green },
		{ DirectX::XMFLOAT3(1.0f, -1.0f, +0.0f), genNorm, genUv, green },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int indices[] = { 0, 1, 2 };
	int indices2[] = { 0, 1, 2, 0, 2, 3 };

	unsigned int vSize = sizeof(vertices) / sizeof(vertices[0]);
	unsigned int iSize = sizeof(indices) / sizeof(indices[0]);

	unsigned int vSize2 = sizeof(vertices2) / sizeof(vertices2[0]);
	unsigned int iSize2 = sizeof(indices2) / sizeof(indices2[0]);

	StoreMesh(new Mesh(vertices, indices, vSize, iSize, "Triangle", device));
	StoreMesh(new Mesh(vertices2, indices2, vSize2, iSize2, "Square", device));
	StoreMesh(new Mesh("Assets/Models/sphere.obj", "Sphere", device));
	StoreMesh(new Mesh("Assets/Models/cube.obj", "Cube", device));
	StoreMesh(new Mesh("Assets/Models/cone.obj", "Cone", device));
	StoreMesh(new Mesh("Assets/Models/torus.obj", "Torus", device));
	StoreMesh(new Mesh("Assets/Models/helix.obj", "Helix", device));
	StoreMesh(new Mesh("Assets/Models/raygun.obj", "RayGun", device));
	StoreMesh(new Mesh("Assets/Models/plane.obj", "Plane", device));
	StoreMesh(new Mesh("Assets/Models/quad.obj", "Quad", device));
	StoreMesh(new Mesh("Assets/Models/teapot.obj", "Teapot", device));
	StoreMesh(new Mesh("Assets/Models/HaloSword.obj", "HaloSword", device));
	StoreMesh(new Mesh("Assets/Models/RainbowRoad.obj", "RainbowRoad", device));
}

void AssetManager::LoadSurfaceTextures()
{
	StoreSurfaceTexture("brick", L"Assets/Textures/brick.jpg");
	StoreSurfaceTexture("harambe", L"Assets/Textures/harambe.jpg");
	//StoreSurfaceTexture("raygun", L"Assets/Textures/raygunUVTest.tga");
}

void AssetManager::LoadNormalTextures()
{
	//StoreNormalTexture("", L"");
}

void AssetManager::LoadSkyBoxTextures()
{
	StoreSkyBoxTexture("sunny", L"Assets/Textures/SkyBox/SunnyCubeMap.dds");
	StoreSkyBoxTexture("mars", L"Assets/Textures/SkyBox/Mars.dds");
	StoreSkyBoxTexture("space", L"Assets/Textures/SkyBox/space.dds");
}

void AssetManager::StoreMesh(Mesh* newMesh)
{
	meshStorage[newMesh->meshName] = newMesh;
}

void AssetManager::StoreSurfaceTexture(std::string name, const wchar_t* filepath)
{
	ID3D11ShaderResourceView* newSurSVR = nullptr;
	DirectX::CreateWICTextureFromFile(device, context, filepath, 0, &newSurSVR);

	surTextStorage[name] = newSurSVR;
}

void AssetManager::StoreNormalTexture(std::string name, const wchar_t* filepath)
{
	ID3D11ShaderResourceView* newNorSVR = nullptr;
	DirectX::CreateWICTextureFromFile(device, context, filepath, 0, &newNorSVR);

	norTextStorage[name] = newNorSVR;
}

void AssetManager::StoreSkyBoxTexture(std::string name, const wchar_t* filepath)
{
	ID3D11ShaderResourceView* newSkySVR = nullptr;
	DirectX::CreateDDSTextureFromFile(device, context, filepath, 0, &newSkySVR);

	skyTextStorage[name] = newSkySVR;
}

Mesh * AssetManager::GetMesh(std::string name)
{
	return meshStorage.find(name)->second;
}

ID3D11ShaderResourceView * AssetManager::GetSurfaceTexture(std::string name)
{
	return surTextStorage.find(name)->second;
}

ID3D11ShaderResourceView * AssetManager::GetNormalTexture(std::string name)
{
	return norTextStorage.find(name)->second;
}

ID3D11ShaderResourceView * AssetManager::GetSkyBoxTexture(std::string name)
{
	return skyTextStorage.find(name)->second;
}
