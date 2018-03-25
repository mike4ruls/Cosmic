#pragma once
#include <vector>
#include "GameEntity.h"
#include "Image.h"
#include "TextBox.h"
#include "Button.h"
#include "FreeCamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"
#include "Renderer.h"
#include "CosmicPhysic.h"
#include "SceneManager.h"

//skyBoxTexture = assetManager->GetSkyBoxTexture("sunny");
//skyBoxTexture = assetManager->GetSkyBoxTexture("mars");
//skyBoxTexture = assetManager->GetSkyBoxTexture("space");

class Game 
{
protected:
	CosmicPhysic* physicEngine;
	InputManager* inputManager;
	AssetManager* assetManager;
	ID3D11ShaderResourceView** skyBoxTexture;
	Light* sunLight;

	float width;
	float height;

public:
	Game() { inputManager = InputManager::GetInstance(); }
	virtual ~Game() 
	{
		if (cam) { delete cam; cam = nullptr; }
	}

	virtual void Init() = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;
	virtual void Quit(){};
	void SetAssetManager(AssetManager* asset,
						 CosmicPhysic* p, 
						 Light* sunL,
		ID3D11ShaderResourceView** skyBoxT) 
	{
		assetManager = asset; physicEngine = p; sunLight = sunL; skyBoxTexture = skyBoxT;
	}
	void SetWidthHeight(float w, float h) { width = w; height = h; }

	Camera* cam;
private:
};

