#include "DefaultScene.h"

DefaultScene::DefaultScene(CosmicEngine* eng) :Game()
{
	engine = eng;
	cam = new Camera();
}


DefaultScene::~DefaultScene()
{
}

void DefaultScene::Init()
{
}
void DefaultScene::Update(float deltaTime, float totalTime)
{
	CheckInputs(deltaTime);
}
void DefaultScene::CheckInputs(float deltaTime)
{
	if (engine->IsKeyDown(97))
	{
		Scene* level1 = new Scene(engine);
		engine->LoadScene(level1);
	}
	else if (engine->IsKeyDown(98))
	{
		Scene2* level2 = new Scene2(engine);
		engine->LoadScene(level2);
	}
}