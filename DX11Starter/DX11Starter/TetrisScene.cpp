#include "TetrisScene.h"
#include "DefaultScene.h"


TetrisScene::TetrisScene()
{
	cam = new FreeCamera();
}


TetrisScene::~TetrisScene()
{
	if (myTetris != nullptr) { delete myTetris; myTetris = nullptr; }
	if (cB != nullptr) { delete cB; cB = nullptr; }
}

void TetrisScene::Init()
{
	myTetris = new Tetris(*(assetManager->GetMesh("Cube")),1,1);
	cB = new ConsoleBuddy(*myTetris, *cam);
	sunLight->ligComponent->lightDir = {0.0f, 0.0f, 1.0f}; 
	*skyBoxTexture = assetManager->GetSkyBoxTexture("space");
}

void TetrisScene::Update(float deltaTime, float totalTime)
{
	/*if (inputManager->IsKeyPressed(KeyCode::Q))
	{
		TetrisBlock* block = myTetris->GenerateBlock();
		myTetris->SetCurrentBlock(block);
	}*/
	if (myTetris->gameStart) {

		if ((inputManager->IsKeyDown(KeyCode::CTRL) && inputManager->IsKeyPressed(KeyCode::C)) || inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_TOUCH_PAD))
		{
			cB->SetTetrisCamera();
		}
		myTetris->UpdateGame(deltaTime, totalTime);
		if (myTetris->tChange)
		{
			myTetris->tChange = false;
		}
		if (myTetris->pChange)
		{
			myTetris->pChange = false;
		}

		// End Game
		if (inputManager->IsKeyPressed(KeyCode::U))
		{
			myTetris->EndGame();
			cB->playerStatus = 2;
		}
		// Reset Game
		if (inputManager->IsKeyPressed(KeyCode::I) || inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_OPTIONS))
		{
			myTetris->ResetGame();
		}
	}
	else
	{
		// Start game
		if (inputManager->IsKeyPressed(KeyCode::Y) || cB->playerStatus == 1 || inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_CROSS))
		{
			myTetris->StartGame(30, 10);
			cB->SetTetrisCamera();
			cB->playerStatus = 1;
		}
	}
	cB->UpdateCB(deltaTime);
	CheckInputs(deltaTime);
}

void TetrisScene::CheckInputs(float deltaTime)
{
	if (inputManager->IsKeyPressed(KeyCode::NUMPAD_0))
	{
		SceneManager::LoadScene(SceneManager::_DefaultScene);
	}
}
