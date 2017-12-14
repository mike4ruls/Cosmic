#pragma once
#include "CosmicEngine.h"
#include "DefaultScene.h"
#include "Tyrian2000.h"
#include "TyrianGameManager.h"
#include "Game.h"
#include "Bullet.h"
#include "BackGroundTiles.h"
#include "Player.h"
#include "ShopMenu.h"
class DryHubWorld :
	public Game
{
public:
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

	DryHubWorld(CosmicEngine* eng);
	~DryHubWorld();
	CosmicEngine* engine;
	TyrianGameManager* gameManager = 0;

	void Init();
	void SetUpLevel();
	void Update(float deltaTime, float totalTime);
	void UpdateParticlesPos();
	void CheckInputs(float dt);
	void CheckControllerInputs(float dt);
	void CheckOutOfBounds();
	void InitUI();
	void LoadBackgroundTilePool(std::string textureName);
	void CreatePlayer();
	void SetUpParticles();
	void LoadBulletPool();
	void CalculateCamPos(float dt);
	void SetUpActions();
	void CompletedLevel(int index);
	void Quit();

	Player* p1 = nullptr;

	Image* healthBar = nullptr;
	Image* healthBarFade = nullptr;
	Image* healthBarBack = nullptr;
	Image* healthBarBorder = nullptr;

	ShopMenu* myShop;

	Emitter* shipExhaust;
	Emitter* leftWing;
	Emitter* rightWing;

	DirectX::XMFLOAT3 exhaustPos;
	DirectX::XMFLOAT3 leftWingPos;
	DirectX::XMFLOAT3 rightWingPos;

	DirectX::XMFLOAT3 curCamPos;

	GameEntity* blueStar;

	std::vector<GameEntity*> levels;
	std::vector<GameEntity*> lineLevels;
	std::vector<Emitter*> goldStarPool;
	std::vector<Emitter*> grayStarPool;

	std::vector<BackGroundTiles*> backgroundTilePool;

	float posXConstraint;
	float negXConstraint;
	float posZConstraint;
	float negZConstraint;

	float xCamConstraint;
	float posZCamConstraint;
	float negZCamConstraint;

	float moveDownHeight;
	float camSpeed;
	float worldRotSpeed;

	float tileSize;
	float tileDistOffScreen;

	int levelCount;
};

