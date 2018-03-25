#include "AssetManager.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

//$(ProjectDir)include\assimp

//lib\assimp\x64

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
	//LoadMeshes();
	AssLoadMeshes();
	//AssLoadMeshesAnims();
	LoadSurfaceTextures();
	LoadNormalTextures();
	LoadSkyBoxTextures();
}

void AssetManager::LoadMeshes()
{
	LoadHandCraftedMeshes();
	StoreMesh(new Mesh("Assets/Models/sphere.obj", "Sphere", device));
	StoreMesh(new Mesh("Assets/Models/sphere.obj", "Worlds", device));
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

void AssetManager::AssLoadMeshes()
{
	//meshStorage.get_allocator().allocate(15);
	LoadHandCraftedMeshes();
	AssimpLoadMeshes("Assets/Models/sphere.obj", "Sphere");
	AssimpLoadMeshes("Assets/Models/sphere.obj", "Worlds");
	AssimpLoadMeshes("Assets/Models/cube.obj", "Cube");
	AssimpLoadMeshes("Assets/Models/cone.obj", "Cone");
	AssimpLoadMeshes("Assets/Models/torus.obj", "Torus");
	AssimpLoadMeshes("Assets/Models/helix.obj", "Helix");
	AssimpLoadMeshes("Assets/Models/raygun.obj", "RayGun");
	AssimpLoadMeshes("Assets/Models/plane.obj", "Plane");
	AssimpLoadMeshes("Assets/Models/plane.obj", "BackGroundTile");
	AssimpLoadMeshes("Assets/Models/quad.obj", "Quad");
	AssimpLoadMeshes("Assets/Models/teapot.obj", "Teapot");
	AssimpLoadMeshes("Assets/Models/HaloSword.obj", "HaloSword");
	AssimpLoadMeshes("Assets/Models/RainbowRoad.obj", "RainbowRoad");
	AssimpLoadMeshes("Assets/Models/FighterShip/WraithRaiderStarship.obj", "FighterShip");

}
void AssetManager::AssLoadMeshesAnims()
{
	AssimpLoadMeshesAnims("Assets/Models/BlackDragon/Dragon 2.5_fbx.fbx", 
						  "Assets/Models/BlackDragon/Dragon_Baked_Actions_fbx_7.4_binary.fbx",
						  "Dragon");
}
void AssetManager::LoadHandCraftedMeshes()
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
}

void AssetManager::AssimpLoadMeshes(char* fileName, std::string name)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<int> indices;           // Indices of these verts

	unsigned int indCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertsPerMesh = 0;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[j]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[j]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

			Vertex v;
			v.Position = DirectX::XMFLOAT3(pPos->x, pPos->y, pPos->z);
			v.Uv = DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y);
			v.Normal = DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z);

			verts.push_back(v);
			vertCount++;
		}
		for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {
			const aiFace& Face = paiMesh->mFaces[j];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0] + vertsPerMesh);
			indices.push_back(Face.mIndices[1] + vertsPerMesh);
			indices.push_back(Face.mIndices[2] + vertsPerMesh);

			indCount += 3;
		}
		vertsPerMesh = vertCount;
	}
	StoreMesh(new Mesh(&verts[0], &indices[0], vertCount, indCount, name, device));
 	importer.FreeScene();
}

void AssetManager::AssimpLoadMeshesAnims(char * fileName, std::string name)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<AnimsVertex> verts;           // Verts we're assembling
	std::vector<int> indices;           // Indices of these verts

	unsigned int indCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertsPerMesh = 0;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];

		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[j]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[j]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

			AnimsVertex v;
			v.Position = DirectX::XMFLOAT3(pPos->x, pPos->y, pPos->z);
			v.Uv = DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y);
			v.Normal = DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z);

			verts.push_back(v);
			vertCount++;
		}
		for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {
			const aiFace& Face = paiMesh->mFaces[j];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0] + vertsPerMesh);
			indices.push_back(Face.mIndices[1] + vertsPerMesh);
			indices.push_back(Face.mIndices[2] + vertsPerMesh);

			indCount += 3;
		}
		vertsPerMesh = vertCount;
	}
	StoreMesh(new Mesh(&verts[0], &indices[0], vertCount, indCount, name, device));
	importer.FreeScene();
}

void AssetManager::AssimpLoadMeshesAnims(char * fileName, char * binarFileName, std::string name)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	const aiScene* pAnimScene = importer.ReadFile(binarFileName, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<AnimsVertex> verts;           // Verts we're assembling
	std::vector<int> indices;           // Indices of these verts

	unsigned int indCount = 0;
	unsigned int vertCount = 0;
	unsigned int vertsPerMesh = 0;

	aiMatrix4x4 globalInvTransform = pAnimScene->mRootNode->mTransformation;
	globalInvTransform.Inverse();
	std::map<std::string, unsigned int> m_BoneMapping;
	std::vector<BoneInfo> m_BoneInfo;
	unsigned int m_NumBones = 0;

	for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

		for (unsigned int j = 0; j < paiMesh->mNumVertices; j++) {
			const aiVector3D* pPos = &(paiMesh->mVertices[j]);
			const aiVector3D* pNormal = &(paiMesh->mNormals[j]);
			const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][j]) : &Zero3D;

			AnimsVertex v;
			v.Position = DirectX::XMFLOAT3(pPos->x, pPos->y, pPos->z);
			v.Uv = DirectX::XMFLOAT2(pTexCoord->x, pTexCoord->y);
			v.Normal = DirectX::XMFLOAT3(pNormal->x, pNormal->y, pNormal->z);

			verts.push_back(v);
			vertCount++;
		}
		// Load Bones
		for (unsigned int i = 0; i < paiMesh->mNumBones; i++) {
			unsigned int BoneIndex = 0;
			std::string BoneName(paiMesh->mBones[i]->mName.data);

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
			}
			else {
				BoneIndex = m_BoneMapping[BoneName];
			}

			m_BoneMapping[BoneName] = BoneIndex;
			m_BoneInfo[BoneIndex].BoneOffset = paiMesh->mBones[i]->mOffsetMatrix;

			for (unsigned int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++) {
				
				unsigned int VertexID = paiMesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = paiMesh->mBones[i]->mWeights[j].mWeight;
				verts[VertexID].boneIds.x = (float)BoneIndex;
				verts[VertexID].weights.x = Weight;
				//Bones[VertexID].AddBoneData(BoneIndex, Weight);
			}
		}
		for (unsigned int j = 0; j < paiMesh->mNumFaces; j++) {
			const aiFace& Face = paiMesh->mFaces[j];
			assert(Face.mNumIndices == 3);
			indices.push_back(Face.mIndices[0] + vertsPerMesh);
			indices.push_back(Face.mIndices[1] + vertsPerMesh);
			indices.push_back(Face.mIndices[2] + vertsPerMesh);

			indCount += 3;
		}
		vertsPerMesh = vertCount;
	}
	StoreMesh(new Mesh(&verts[0], &indices[0], vertCount, indCount, name, device));
	importer.FreeScene();
}

void AssetManager::LoadSurfaceTextures()
{
	StoreSurfaceTexture("brick", L"Assets/Textures/brick.jpg");
	StoreSurfaceTexture("harambe", L"Assets/Textures/harambe.jpg");
	StoreSurfaceTexture("checker", L"Assets/Textures/checker.jpg");
	StoreSurfaceTexture("water", L"Assets/Textures/water.jpg");
	StoreSurfaceTexture("grass", L"Assets/Textures/grass.jpg");
	StoreSurfaceTexture("star", L"Assets/Textures/stars.jpg");
	StoreSurfaceTexture("dry", L"Assets/Textures/dryGround.jpg");
	StoreSurfaceTexture("sand", L"Assets/Textures/sandGround.jpg");
	StoreSurfaceTexture("rainbow", L"Assets/Textures/rainbow.jpg");
	StoreSurfaceTexture("lava", L"Assets/Textures/lava.jpg");
	StoreSurfaceTexture("lavaGround", L"Assets/Textures/lavaGround.jpg");
	StoreSurfaceTexture("failed1", L"Assets/Textures/failed1.jpg");
	StoreSurfaceTexture("failed2", L"Assets/Textures/failed2.jpg");
	StoreSurfaceTexture("failed3", L"Assets/Textures/failed3.jpg");
	StoreSurfaceTexture("tyrian2000Logo", L"Assets/Textures/tyrian2000Logo.jpg");
	StoreSurfaceTexture("pausedLogo", L"Assets/Textures/pausedLogo.jpg");
	StoreSurfaceTexture("startButtontext", L"Assets/Textures/startButton.jpg");
	StoreSurfaceTexture("resumeButtontext", L"Assets/Textures/resumeButton.jpg");
	StoreSurfaceTexture("retryButtontext", L"Assets/Textures/retryButton.jpg");
	StoreSurfaceTexture("quitButtontext", L"Assets/Textures/quitButton.jpg");
	StoreSurfaceTexture("continueButtontext", L"Assets/Textures/continueButton.jpg");
	StoreSurfaceTexture("complete1", L"Assets/Textures/complete1.jpg");
	StoreSurfaceTexture("dragonSur", L"Assets/Textures/BlackDragon/Dragon_ground_color.jpg");
	StoreSurfaceTexture("fighterShipSur", L"Assets/Textures/FighterShip/vj2c.jpg");
	StoreSurfaceTexture("metalBack", L"Assets/Textures/metalBackText.jpg");
	StoreSurfaceTexture("metalTab", L"Assets/Textures/metalTabText.jpg");
	StoreSurfaceTexture("orangeFire", L"Assets/Textures/orangeFire.png");
	StoreSurfaceTexture("blackFire", L"Assets/Textures/blackFire.png");
	StoreSurfaceTexture("whiteBox", L"Assets/Textures/whiteBox.jpg");
	StoreSurfaceTexture("whiteSmoke", L"Assets/Textures/whiteSmoke.png");
	StoreSurfaceTexture("goldStar", L"Assets/Textures/goldStar.png");
	StoreSurfaceTexture("grayStar", L"Assets/Textures/grayStar.png");
	StoreSurfaceTexture("redStar", L"Assets/Textures/redStar.png");
	StoreSurfaceTexture("blueStar", L"Assets/Textures/blueStar.png");
	StoreSurfaceTexture("snowFlake", L"Assets/Textures/snowFlake.png");
	StoreSurfaceTexture("yellowTri", L"Assets/Textures/yellowTriangleText.png");
	StoreSurfaceTexture("statsText", L"Assets/Textures/statsText.jpg");
	StoreSurfaceTexture("shopText", L"Assets/Textures/shopText.jpg");
	StoreSurfaceTexture("upgradesText", L"Assets/Textures/upgradesText.jpg");
	StoreSurfaceTexture("abilitiesText", L"Assets/Textures/abilitiesText.jpg");
	StoreSurfaceTexture("buyText", L"Assets/Textures/buyText.jpg");
	//StoreSurfaceTexture("raygun", L"Assets/Textures/raygunUVTest.tga");

	StoreSurfaceTexture("attackSpeedIcon", L"Assets/Textures/MenuIcons/attackSpeed.png");
	StoreSurfaceTexture("barrierIcon", L"Assets/Textures/MenuIcons/barrier.png");
	StoreSurfaceTexture("cannonIcon", L"Assets/Textures/MenuIcons/cannon.png");
	StoreSurfaceTexture("defenseUpIcon", L"Assets/Textures/MenuIcons/defenseUp.png");
	StoreSurfaceTexture("guidedMissleIcon", L"Assets/Textures/MenuIcons/guidedMissle.png");
	StoreSurfaceTexture("healthUpIcon", L"Assets/Textures/MenuIcons/healthUp.png");
	StoreSurfaceTexture("powerIcon", L"Assets/Textures/MenuIcons/power.png");
	StoreSurfaceTexture("secondChanceIcon", L"Assets/Textures/MenuIcons/secondChance.png");
	StoreSurfaceTexture("sheildComponentIcon", L"Assets/Textures/MenuIcons/sheildComponent.png");
	StoreSurfaceTexture("sheildPowerIcon", L"Assets/Textures/MenuIcons/sheildPower.png");
	StoreSurfaceTexture("sheildRechargeIcon", L"Assets/Textures/MenuIcons/sheildRecharge.png");
	StoreSurfaceTexture("sheildCombIcon", L"Assets/Textures/MenuIcons/sheildComb.png");
	StoreSurfaceTexture("starSwirlIcon", L"Assets/Textures/MenuIcons/starSwirl.png");
	StoreSurfaceTexture("upgradeIcon", L"Assets/Textures/MenuIcons/upgrade.png");
	StoreSurfaceTexture("jetFighterIcon", L"Assets/Textures/MenuIcons/jet-fighter.png");
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
