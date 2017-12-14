#pragma once
#include <d3d11.h>
#include "AssetManager.h"
#include "Emitter.h"
#include "Camera.h"
#include "Light.h"
#include "SimpleShader.h"
#include"InstanceData.h"

class Renderer
{
public:
	Renderer();
	Renderer(Camera* c, ID3D11Device* dev, ID3D11DeviceContext* con, ID3D11RenderTargetView* backB, ID3D11DepthStencilView* depthS, IDXGISwapChain* sw);
	~Renderer();

	AssetManager* assets = nullptr;

	/*std::map<std::string, Mesh*> meshStorage;
	std::map<std::string, ID3D11ShaderResourceView*> surTextStorage;
	std::map<std::string, ID3D11ShaderResourceView*> norTextStorage;
	std::map<std::string, ID3D11ShaderResourceView*> skyTextStorage;*/

	std::vector<RenderingComponent*> transRendComponents;
	std::vector<RenderingComponent*> canvasRendComponents;
	std::vector<Light*> allLights;

	std::vector<Light::LightComponent> directionalLights;
	std::vector<Light::LightComponent> pointLights;
	std::vector<Light::LightComponent> spotLights;

	std::vector<Emitter*> particleEmitters;

	Camera* cam;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swap;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11RenderTargetView* colorRTV;
	ID3D11RenderTargetView* worldRTV;
	ID3D11RenderTargetView* normalRTV;

	ID3D11RasterizerState* wireFrame = nullptr;
	ID3D11RasterizerState* fillFrame = nullptr;


	SimpleVertexShader* vertexFShader;
	SimpleVertexShader* instanceFVShader;
	SimpleVertexShader* vertexDShader;
	SimpleVertexShader* instanceDVShader;
	SimpleVertexShader* skyVShader;
	SimpleVertexShader* quadVShader;
	SimpleVertexShader* canvasVShader;
	SimpleVertexShader* particleVShader;


	SimplePixelShader* pixelFShader;
	SimplePixelShader* pixelFSShader;
	SimplePixelShader* pixelFSNShader;
	SimplePixelShader* pixelDShader;
	SimplePixelShader* pLightingShader;
	SimplePixelShader* skyPShader;
	SimplePixelShader* bloomShader;
	SimplePixelShader* hdrShader;
	SimplePixelShader* canvasPShader;
	SimplePixelShader* canvasPTShader;
	SimplePixelShader* particlePShader;

	ID3D11SamplerState* textureSample = nullptr;

	ID3D11ShaderResourceView* skyBoxSVR = nullptr;
	ID3D11RasterizerState* skyRast = nullptr;
	ID3D11DepthStencilState* skyDepth = nullptr;
	ID3D11SamplerState* sample = nullptr;

	ID3D11DepthStencilState* particleDepthState;
	ID3D11BlendState* addBlendState;
	ID3D11BlendState* subBlendState;
	ID3D11BlendState* cutBlendState;

	bool instanceRenderingOn;
	bool defferedRenderingOn;
	bool HdrOn;
	bool BloomOn;
	bool wireFrameOn;
	bool skyBoxOn;

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
	void CreateRenderTargets();
	/*void LoadMesh(Mesh* newMesh);
	Mesh* GetMesh(std::string name);*/
	void PushToRenderer(RenderingComponent* com);
	void PushToTranslucent(RenderingComponent* com);
	void PushToCanvas(RenderingComponent* com);
	void PushToEmitter(Emitter* emit);
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
	void RemoveFromCanvas(unsigned int Id);
	void RemoveFromEmitter(unsigned int Id);
	void LoadShaders();
	void SetWireFrame();
	void ToggleWireFrame();

	void CompileLights();
	void SetLights(SimplePixelShader* pixel);

	void DrawForwardPass(RenderingComponent* component);
	void DrawFInstance(std::string meshName, InstanceData* components, unsigned int count);

	void DrawDefferedPass(RenderingComponent* component);
	void DrawDInstance(std::string meshName, InstanceData* components, unsigned int count);

	void InitSkyBox();
	void ToggleSkyBox();
	void LoadSkyBox(int skyNum);
	void DrawSkyBox();

	void DrawEmiters();

	void DrawCanvas();


private:
	bool prevWireStatus;
};
