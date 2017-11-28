#pragma once
#include "CosmicEngine.h"
#include "Game.h"
#include "DefaultScene.h"
#include "Scene.h"

class Scene2 :
	public Game
{
public:


	Scene2(CosmicEngine* eng);
	~Scene2();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);
	void SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot);

	std::vector<GameEntity*> gameObjects;

	Emitter* explosion;

	Emitter* goldStars;
	Emitter* grayStars;

	Emitter* blizzard;

	CosmicEngine* engine;
	Light* pointLight;
	Emitter* particleSystem;
	Button* but;

private:

};