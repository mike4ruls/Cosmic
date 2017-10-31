#pragma once
#include "CosmicEngine.h"
#include "DefaultScene.h"
#include "Player.h"

class FrameRateTestScene :
	public Game
{
public:
	FrameRateTestScene(CosmicEngine* eng);
	~FrameRateTestScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	CosmicEngine* engine;
	Player* p1;

private:

};