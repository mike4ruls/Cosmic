#pragma once
#include "DefaultScene.h"
#include "Player.h"

class FrameRateTestScene :
	public Game
{
public:
	FrameRateTestScene();
	~FrameRateTestScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	std::vector<GameEntity*> gameObjects;
	Player* p1;

private:

};