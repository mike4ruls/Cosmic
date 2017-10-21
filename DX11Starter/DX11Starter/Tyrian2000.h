#pragma once
#include "CosmicEngine.h"
#include "DefaultScene.h"
#include "Player.h"
#include "Bullet.h"
#include "BackGroundTiles.h"
#include "Game.h"

class Tyrian2000 :
	public Game
{
public:
	Tyrian2000(CosmicEngine* eng);
	~Tyrian2000();
	CosmicEngine* engine;

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float dt);
	void CreatePlayer();
	void LoadBulletPool();
	void LoadBackgroundTilePool(std::string textureName);
	void Shoot();

	Player* p1;

	std::vector<Bullet*> bulletPool;
	std::vector<BackGroundTiles*> backgroundTilePool;

	// Game Values
	float xConstraint;
	float posZConstraint;
	float negZConstraint;

	float tileSize;
	float tileDistOffScreen;

	float moveDownHeight;
};

