#pragma once
#include "Game.h"
#include "DefaultScene.h"
#include "Scene2.h"
class Scene :
	public Game
{
public:
	Scene();
	~Scene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);
	void SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot);

	std::vector<GameEntity*> gameObjects;
	float dayTime;
private:

};

