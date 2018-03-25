#pragma once
#include "DefaultScene.h"
#include "Tyrian2000.h"
#include "TyrianGameManager.h"
#include "Game.h"
#include "Bullet.h"
#include "BackGroundTiles.h"
#include "Player.h"
#include "ShopMenu.h"
#include "HubWorldEngine.h"

class BrickHubWorld :
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

	BrickHubWorld();
	~BrickHubWorld();
	TyrianGameManager* gameManager = 0;
	HubWorldEngine* hubEngine;

	void Init();
	void Update(float deltaTime, float totalTime);
	void LoadBackgroundTilePool(std::string textureName);
	void CompletedLevel(int index);
	void Quit();

	Player* p1 = nullptr;

	GameEntity* blueStar;

	std::vector<GameEntity*> levels;
	std::vector<GameEntity*> lineLevels;
	std::vector<Emitter*> goldStarPool;
	std::vector<Emitter*> grayStarPool;

	std::vector<BackGroundTiles*> backgroundTilePool;

	float worldRotSpeed;

	float tileSize;
	float tileDistOffScreen;
	float moveDownHeight;

	int levelCount;
};

