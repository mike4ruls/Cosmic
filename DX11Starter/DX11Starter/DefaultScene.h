#pragma once
#include "Game.h"
#include "TetrisScene.h"
#include "Scene2.h"
#include "Scene.h"
#include "Tyrian2000.h"
#include "FrameRateTestScene.h"
class DefaultScene :
	public Game
{
public:
	DefaultScene();
	~DefaultScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	std::vector<GameEntity*> gameObjects;
private:

};