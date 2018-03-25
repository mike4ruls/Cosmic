#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>
#include "Vertex.h"
#include "AnimsVertex.h"
#include "renderingComponent.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

class Mesh
{
public:
	Mesh();
	Mesh(Vertex* vArr, int* iArr, unsigned int vSize, unsigned int iSize, std::string name, ID3D11Device* device);
	Mesh(AnimsVertex* vArr, int* iArr, unsigned int vSize, unsigned int iSize, std::string name, ID3D11Device* device);
	Mesh(const char* fileName, std::string name, ID3D11Device* device);
	~Mesh();

	std::string meshName;
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertArr;
	ID3D11Buffer* indArr;

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndicesBuffer();

	std::vector<RenderingComponent*>* rendComponents;
	unsigned int instanceThreshold;
	unsigned int instances;
	bool hasAnimations = false;
	bool canInstRender;
	bool inUse;
	bool broken;

	unsigned int indCount;

private:

};

