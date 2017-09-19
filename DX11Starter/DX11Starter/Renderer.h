#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include "Mesh.h"
#include "Camera.h"
#include "SimpleShader.h"
class Renderer
{
public:
	Renderer();
	Renderer(Camera* c, ID3D11Device* dev, ID3D11DeviceContext* con);
	~Renderer();

	std::map<std::string, Mesh*> meshStorage;
	Camera* cam;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	SimpleVertexShader* vertexShader;
	SimpleVertexShader* instanceVShader;
	SimplePixelShader* pixelShader;

	unsigned int instanceThreshold;
	bool instanceRenderingOn;

	unsigned int numInstances;
	DirectX::XMFLOAT4X4* localInstanceData;
	ID3D11Buffer* instanceWorldMatrixBuffer;

	void Init();
	void Render(ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView);
	void LoadMesh(Mesh* newMesh);
	Mesh* GetMesh(std::string name);
	unsigned int PushToRenderer(std::string meshName, RenderingComponent* com);
	void RemoveFromRenderer(std::string meshName, unsigned int Id);
	void LoadShaders();

private:

};

