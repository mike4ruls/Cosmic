#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include "Mesh.h"
#include "Camera.h"
#include "SimpleShader.h"
#include"InstanceData.h"

class Renderer
{
public:
	Renderer();
	Renderer(Camera* c, ID3D11Device* dev, ID3D11DeviceContext* con, ID3D11RenderTargetView* backB, ID3D11DepthStencilView* depthS);
	~Renderer();

	std::map<std::string, Mesh*> meshStorage;
	std::vector<RenderingComponent*> transRendComponents;
	Camera* cam;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* wireFrame;
	ID3D11RasterizerState* fillFrame;

	SimpleVertexShader* vertexShader;
	SimpleVertexShader* instanceVShader;
	SimplePixelShader* pixelShader;

	unsigned int instanceThreshold;
	bool instanceRenderingOn;
	bool defferedRenderingOn;
	bool HdrOn;
	bool BloomOn;
	bool wireFrameOn;

	unsigned int numInstances;
	InstanceData* localInstanceData;
	ID3D11Buffer* instanceWorldMatrixBuffer;

	void Init();
	void Render(float dt);
	void LoadMesh(Mesh* newMesh);
	Mesh* GetMesh(std::string name);
	unsigned int PushToRenderer(RenderingComponent* com);
	unsigned int PushToTranslucent(RenderingComponent* com);
	void RemoveFromRenderer(std::string meshName, unsigned int Id);
	void RemoveFromTranslucent(unsigned int Id);
	void LoadShaders();
	void SetWireFrame();
	void ToggleWireFrame();

	void DrawSkyBox();

	void DrawForwardPass(RenderingComponent* component);
	void DrawFInstance(std::string meshName, InstanceData* components, unsigned int count);

	void DrawDefferedPass(RenderingComponent* component);
	void DrawDInstance(std::string meshName, InstanceData* components, unsigned int count);


private:
	bool prevWireStatus;
};
