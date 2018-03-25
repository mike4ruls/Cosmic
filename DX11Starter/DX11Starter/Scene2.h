#pragma once
#include "Game.h"
#include "DefaultScene.h"
#include "Scene.h"

class Scene2 :
	public Game
{
public:


	Scene2();
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

	Light* pointLight;
	Emitter* particleSystem;
	Button* but;

private:

};