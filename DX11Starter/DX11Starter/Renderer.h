#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
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
	std::vector<Light*> directionalLights;
	std::vector<Light*> pointLights;
	std::vector<Light*> spotLights;

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
	Light* sunLight;

	void Init();
	void Render(float dt);
	void LoadMesh(Mesh* newMesh);
	Mesh* GetMesh(std::string name);
	unsigned int PushToRenderer(RenderingComponent* com);
	unsigned int PushToTranslucent(RenderingComponent* com);

	Light* CreateLight(Light::LightType lType);

	Light* CreateDirectionalLight(DirectX::XMFLOAT3 direction);
	Light* CreateDirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 ligColor);
	Light* CreateDirectionalLight(DirectX::XMFLOAT3 direction, DirectX::XMFLOAT4 ligColor, float inten);

	Light* CreatePointLight(DirectX::XMFLOAT3 position);
	Light* CreatePointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor);
	Light* CreatePointLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor, float inten);

	Light* CreateSpotLight(DirectX::XMFLOAT3 position);
	Light* CreateSpotLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor);
	Light* CreateSpotLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 ligColor, float inten);

	void RemoveLight(Light* light);

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
