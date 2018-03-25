#include "FrameRateTestScene.h"

FrameRateTestScene::FrameRateTestScene() :Game()
{
	cam = new FreeCamera();
}


FrameRateTestScene::~FrameRateTestScene()
{
	if (p1 != nullptr) { delete p1; p1 = nullptr; }
}

void FrameRateTestScene::Init()
{
	cam->inputManager = inputManager;

	p1 = new Player(new GameEntity(assetManager->GetMesh("FighterShip"),false), assetManager->GetMesh("Sphere"), 4.0f, 0.1f, 1.0f);
	p1->player->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("fighterShipSur"));
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, -26.0f, 0.0f);
	p1->player->SetWorld();

	*skyBoxTexture = assetManager->GetSkyBoxTexture("sunny");
}
void FrameRateTestScene::Update(float deltaTime, float totalTime)
{
	CheckInputs(deltaTime);
}
void FrameRateTestScene::CheckInputs(float deltaTime)
{
	if (inputManager->IsKeyDown(96))
	{
		SceneManager::LoadScene(SceneManager::_DefaultScene);
	}
}