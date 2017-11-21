#include "DefaultScene.h"

DefaultScene::DefaultScene(CosmicEngine* eng) :Game()
{
	engine = eng;
	cam = new FreeCamera();
}


DefaultScene::~DefaultScene()
{
}

void DefaultScene::Init()
{
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;
}
void DefaultScene::Update(float deltaTime, float totalTime)
{
	CheckInputs(deltaTime);
}
void DefaultScene::CheckInputs(float deltaTime)
{
	if (engine->inputManager->IsKeyPressed(97))
	{
		Scene* level1 = new Scene(engine);
		engine->LoadScene(level1);
	}
	else if (engine->inputManager->IsKeyPressed(98))
	{
		Scene2* level2 = new Scene2(engine);
		engine->LoadScene(level2);
	}
	else if (engine->inputManager->IsKeyPressed(99))
	{
		Tyrian2000* tyrian = new Tyrian2000(engine);
		engine->LoadScene(tyrian);
	}
	else if (engine->inputManager->IsKeyPressed(100))
	{
		FrameRateTestScene* frameRate = new FrameRateTestScene(engine);
		engine->LoadScene(frameRate);
	}
}