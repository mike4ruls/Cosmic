#include "FrameRateTestScene.h"

FrameRateTestScene::FrameRateTestScene(CosmicEngine* eng) :Game()
{
	engine = eng;
	cam = new FreeCamera();
}


FrameRateTestScene::~FrameRateTestScene()
{
	if (p1 != nullptr) { delete p1; p1 = nullptr; }
}

void FrameRateTestScene::Init()
{
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;

	p1 = new Player(engine->CreateGameObject("FighterShip"), 4.0f, 0.1f, 1.0f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, -26.0f, 0.0f);
	p1->player->SetWorld();
}
void FrameRateTestScene::Update(float deltaTime, float totalTime)
{
	CheckInputs(deltaTime);
}
void FrameRateTestScene::CheckInputs(float deltaTime)
{
	if (inputManager->IsKeyDown(96))
	{
		DefaultScene* deafaultScene = new DefaultScene(engine);
		engine->LoadScene(deafaultScene);
	}
}