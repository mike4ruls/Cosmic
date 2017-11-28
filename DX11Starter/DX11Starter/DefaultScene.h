#pragma once
#include "CosmicEngine.h"
#include "Game.h"
#include "Scene2.h"
#include "Scene.h"
#include "Tyrian2000.h"
#include "FrameRateTestScene.h"
#include "MainHUBWorld.h"
class DefaultScene :
	public Game
{
public:
	DefaultScene(CosmicEngine* eng);
	~DefaultScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	std::vector<GameEntity*> gameObjects;

	CosmicEngine* engine;
private:

};