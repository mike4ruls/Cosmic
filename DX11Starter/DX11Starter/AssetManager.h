#pragma once
#include <d3d11.h>
#include <map>
#include <string>
#include "Mesh.h"
#include "BoneInfo.h"
#include "Animation.h"


class AssetManager
{
public:
	AssetManager(ID3D11Device* dev, ID3D11DeviceContext* con);
	~AssetManager();

	void Init();

	void LoadMeshes();
	void AssLoadMeshes();
	void AssLoadMeshesAnims();
	void LoadHandCraftedMeshes();
	void AssimpLoadMeshes(char* fileName, std::string name);
	void AssimpLoadMeshesAnims(char* fileName, std::string name);
	void AssimpLoadMeshesAnims(char* fileName, char* binarFileName, std::string name);
	void LoadSurfaceTextures();
	void LoadNormalTextures();
	void LoadSkyBoxTextures();

	void StoreMesh(Mesh * newMesh);
	void StoreSurfaceTexture(std::string name, const wchar_t* filepath);
	void StoreNormalTexture(std::string name, const wchar_t* filepath);
	void StoreSkyBoxTexture(std::string name, const wchar_t* filepath);

	Mesh* GetMesh(std::string name);
	ID3D11ShaderResourceView* GetSurfaceTexture(std::string name);
	ID3D11ShaderResourceView* GetNormalTexture(std::string name);
	ID3D11ShaderResourceView* GetSkyBoxTexture(std::string name);

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	std::map<std::string, Mesh*> meshStorage;
	std::map<std::string, ID3D11ShaderResourceView*> surTextStorage;
	std::map<std::string, ID3D11ShaderResourceView*> norTextStorage;
	std::map<std::string, ID3D11ShaderResourceView*> skyTextStorage;
};

