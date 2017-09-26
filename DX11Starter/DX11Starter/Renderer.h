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
	std::vector<Light*> allLights;

	std::vector<Light::LightComponent> directionalLights;
	std::vector<Light::LightComponent> pointLights;
	std::vector<Light::LightComponent> spotLights;

	Camera* cam;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RasterizerState* wireFrame = nullptr;
	ID3D11RasterizerState* fillFrame = nullptr;


	SimpleVertexShader* vertexFShader;
	SimpleVertexShader* instanceFVShader;
	SimpleVertexShader* vertexDShader;
	SimpleVertexShader* instanceDVShader;
	SimpleVertexShader* skyVShader;
	SimpleVertexShader* quadVShader;


	SimplePixelShader* pixelFShader;
	SimplePixelShader* pixelDShader;
	SimplePixelShader* pLightingShader;
	SimplePixelShader* skyPShader;
	SimplePixelShader* bloomShader;
	SimplePixelShader* hdrShader;

	ID3D11ShaderResourceView* SVR = nullptr;
	ID3D11SamplerState* sample = nullptr;


	ID3D11ShaderResourceView* skyBoxSVR = nullptr;
	ID3D11RasterizerState* skyRast = nullptr;
	ID3D11DepthStencilState* skyDepth = nullptr;

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

	/*Light::DirectionalLight dArr[10];
	Light::PointLight pArr[10];
	Light::SpotLight sArr[10];*/

	int skyBoxNum;
	int maxSize;

	int maxDLights = 10;
	int maxPLights = 10;
	int maxSLights = 10;

	void Init();
	void Render(float dt);
	void LoadMesh(Mesh* newMesh);
	Mesh* GetMesh(std::string name);
	unsigned int PushToRenderer(RenderingComponent* com);
	unsigned int PushToTranslucent(RenderingComponent* com);
	void Flush();

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

	void CompileLights();
	void SetLights();

	void DrawForwardPass(RenderingComponent* component);
	void DrawFInstance(std::string meshName, InstanceData* components, unsigned int count);

	void DrawDefferedPass(RenderingComponent* component);
	void DrawDInstance(std::string meshName, InstanceData* components, unsigned int count);

	void InitSkyBox();
	void ToggleSkyBox();
	void LoadSkyBox(int skyNum);
	void DrawSkyBox();


private:
	bool prevWireStatus;
};
