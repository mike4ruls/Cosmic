#pragma once
#include "DefaultScene.h"
#include "TyrianGameManager.h"
#include "ShopMenu.h"
#include "Game.h"
#include "Bullet.h"
#include "Player.h"
#include "HubWorldEngine.h"

class MainHUBWorld :
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

	MainHUBWorld();
	~MainHUBWorld();
	void Quit();
	TyrianGameManager* gameManager = 0;
	HubWorldEngine* hubEngine;

	void Init();
	void Update(float deltaTime, float totalTime);

	Player* p1;
	float worldRotSpeed;

	std::vector<GameEntity*> worlds;
	std::vector<Emitter*> goldStarPool;
	std::vector<Emitter*> grayStarPool;
};

