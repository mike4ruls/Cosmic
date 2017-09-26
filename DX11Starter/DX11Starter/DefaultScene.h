#pragma once
#include "CosmicEngine.h"
#include "Game.h"
#include "Scene2.h"
#include "Scene.h"

class DefaultScene :
	public Game
{
public:
	DefaultScene(CosmicEngine* eng);
	~DefaultScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	CosmicEngine* engine;
private:

};