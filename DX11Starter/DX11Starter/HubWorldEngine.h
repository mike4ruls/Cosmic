#pragma once

#include "DefaultScene.h"
#include "TyrianGameManager.h"
#include "ShopMenu.h"
#include "Bullet.h"
#include "Player.h"

class HubWorldEngine
{
public:
	HubWorldEngine(Camera* c, AssetManager* a, Light* skyL);
	~HubWorldEngine();
	enum Actions {
		ButtonUp,
		ButtonDown,
		ButtonLeft,
		ButtonRight,
		LB,
		RB,
		Fire,
		Strafe,
		Start,
		Select
	};
	enum GameState {
		StartMenu,
		Game,
		Paused,
		EndLevel
	}currentState;
	AssetManager* assetManager;
	Camera* cam;
	InputManager* inputManager;
	TyrianGameManager* gameManager = 0;
	ShopMenu* myShop;
	Light* sunLight;

	void Init();
	void SetUpLevel();
	void Update(float deltaTime, float totalTime);
	void UpdateParticlesPos();
	void CheckControllerInputs(float dt);
	void CheckOutOfBounds();
	void InitUI();
	void CreatePlayer();
	void SetUpParticles();
	void LoadBulletPool();
	void CalculateCamPos(float dt);
	void SetUpActions();

	Player* p1 = nullptr;

	Emitter* shipExhaust;
	Emitter* leftWing;
	Emitter* rightWing;

	DirectX::XMFLOAT3 exhaustPos;
	DirectX::XMFLOAT3 leftWingPos;
	DirectX::XMFLOAT3 rightWingPos;

	DirectX::XMFLOAT3 curCamPos;

	std::vector<GameEntity*> worlds;
	std::vector<Emitter*> goldStarPool;
	std::vector<Emitter*> grayStarPool;

	float posXConstraint;
	float negXConstraint;
	float posZConstraint;
	float negZConstraint;

	float xCamConstraint;
	float posZCamConstraint;
	float negZCamConstraint;

	float moveDownHeight;
	float camSpeed;
};

