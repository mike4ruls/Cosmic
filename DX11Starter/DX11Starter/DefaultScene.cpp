#include "DefaultScene.h"

DefaultScene::DefaultScene() :Game()
{
	cam = new FreeCamera();
}


DefaultScene::~DefaultScene()
{
}

void DefaultScene::Init()
{
	cam->inputManager = inputManager;
	*skyBoxTexture = assetManager->GetSkyBoxTexture("sunny");
}
void DefaultScene::Update(float deltaTime, float totalTime)
{
	CheckInputs(deltaTime);
}
void DefaultScene::CheckInputs(float deltaTime)
{
	if (inputManager->IsKeyPressed(KeyCode::NUMPAD_1))
	{
		SceneManager::LoadScene(SceneManager::_Scene);
	}
	else if (inputManager->IsKeyPressed(KeyCode::NUMPAD_2))
	{
		SceneManager::LoadScene(SceneManager::_Scene2);
	}
	else if (inputManager->IsKeyPressed(KeyCode::NUMPAD_3))
	{
		SceneManager::LoadScene(SceneManager::_Tyrian2000);
	}
	else if (inputManager->IsKeyPressed(KeyCode::NUMPAD_4))
	{
		SceneManager::LoadScene(SceneManager::_FrameRateTestScene);
	}
	else if (inputManager->IsKeyPressed(KeyCode::NUMPAD_5))
	{
		SceneManager::LoadScene(SceneManager::_TetrisScene);
	}
	/*else if (engine->inputManager->IsKeyPressed(101))
	{
		MainHUBWorld* mainHUB = new MainHUBWorld(engine);
		engine->LoadScene(mainHUB);
	}
	else if (engine->inputManager->IsKeyPressed(102))
	{
		GrassHubWorld* grassHUB = new GrassHubWorld(engine);
		engine->LoadScene(grassHUB);
	}*/
}