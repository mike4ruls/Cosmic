#include "MainHUBWorld.h"
#include <stdio.h>


MainHUBWorld::MainHUBWorld()
{
	cam = new FreeCamera();
}


MainHUBWorld::~MainHUBWorld()
{
	delete hubEngine;
}

void MainHUBWorld::Init()
{
	hubEngine = new HubWorldEngine(cam, assetManager, sunLight);
	hubEngine->Init();
	*skyBoxTexture = assetManager->GetSkyBoxTexture("mars");
	p1 = hubEngine->p1;
	p1->player->transform.position = gameManager->GetWorldPosistion();
	hubEngine->curCamPos = { p1->player->transform.position.x, 0.0f, p1->player->transform.position.z };
	worldRotSpeed = 20.0f;

	for (int i = 0; i < 10; i++)
	{
		worlds.push_back(new GameEntity(assetManager->GetMesh("Worlds"), false));
		worlds[i]->transform.Scale(7.0f);
		worlds[i]->rigidBody.myCollider.radius = 4.0f;
		worlds[i]->transform.position = { (-1.0f + (float)i) * 17, p1->player->transform.position.y, 0.0f };
		worlds[i]->transform.Translate(17.0f, 0.0f, 3.0f);

		goldStarPool.push_back(new Emitter(100, assetManager->GetSurfaceTexture("goldStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		goldStarPool[i]->transform.position = worlds[i]->transform.position;
		goldStarPool[i]->lifeTime = 5.0f;
		goldStarPool[i]->isActive = false;
		goldStarPool[i]->startSize = 1.0f;
		goldStarPool[i]->endSize = 1.3f;

		grayStarPool.push_back(new Emitter(100, assetManager->GetSurfaceTexture("grayStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		grayStarPool[i]->transform.position = worlds[i]->transform.position;
		grayStarPool[i]->lifeTime = 5.0f;
		grayStarPool[i]->isActive = false;
		grayStarPool[i]->startSize = 1.0f;
		grayStarPool[i]->endSize = 1.3f;

		switch (i) {
		case 0:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("grass"));
			worlds[i]->name = "grass";
			//grayStarPool[i]->isActive = true;
			//goldStarPool[i]->isActive = true;
			break;
		case 1:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("water"));
			worlds[i]->name = "water";
			break;
		case 2:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("sand"));
			worlds[i]->name = "sand";
			break;
		case 3:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("dry"));
			worlds[i]->name = "dry";
			break;
		case 4:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("brick"));
			worlds[i]->name = "brick";
			break;
		case 5:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("star"));
			worlds[i]->name = "star";
			break;
		case 6:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("lava"));
			worlds[i]->name = "lava";
			break;
		case 7:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("lavaGround"));
			worlds[i]->name = "lavaGround";
			worlds[i]->transform.Scale(20.0f);
			worlds[i]->rigidBody.myCollider.radius = 11.0f;
			worlds[i]->transform.position = { -60.0f, p1->player->transform.position.y, worlds[i]->transform.position.z };

			goldStarPool[i]->transform.position = worlds[i]->transform.position;
			goldStarPool[i]->lifeTime = 9.0f;
			goldStarPool[i]->startSize = 1.0f;
			goldStarPool[i]->endSize = 5.3f;

			grayStarPool[i]->transform.position = worlds[i]->transform.position;
			grayStarPool[i]->lifeTime = 9.0f;
			grayStarPool[i]->startSize = 1.0f;
			grayStarPool[i]->endSize = 5.3f;
			break;
		case 8:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("checker"));
			worlds[i]->name = "checker";
			break;
		case 9:
			worlds[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("rainbow"));
			worlds[i]->name = "rainbow";
			break;
		}

		switch (gameManager->GetActiveWorld(i))
		{
		case 0:
			break;
		case 1:
			cam->ShakeCamera(1.0f, 0.1f);
			cam->ShakeCamera(0.75f, 0.1f);
			cam->ShakeCamera(0.70f, 0.1f);
			cam->ShakeCamera(0.65f, 0.1f);
			cam->ShakeCamera(0.50f, 0.1f);
			cam->ShakeCamera(0.40f, 0.1f);
			cam->ShakeCamera(0.30f, 0.3f);
			cam->ShakeCamera(0.25f, 0.4f);
			cam->ShakeCamera(0.20f, 0.5f);
			cam->ShakeCamera(0.1f, 0.6f);

			gameManager->SetActiveLevels(i,2);

			goldStarPool[i]->Reset();
			grayStarPool[i]->Reset();
			break;
		case 2:
			goldStarPool[i]->Reset();
			grayStarPool[i]->Reset();
			break;
		}

		//worlds[i]->renderingComponent->mat.surfaceColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	}
	worlds[0]->renderingComponent->mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void MainHUBWorld::Update(float deltaTime, float totalTime)
{
	hubEngine->Update(deltaTime, totalTime);
	for (unsigned int i = 0; i < worlds.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
	{
		float originalRad = worlds[i]->rigidBody.myCollider.radius;
		worlds[i]->transform.Rotate(worldRotSpeed * deltaTime, 0.0f, 0.0f);

		worlds[i]->rigidBody.myCollider.radius += 2.0f;
		if (physicEngine->SphereVSphereCollision(p1->player, worlds[i]))
		{
			gameManager->SetTileLoadName(&worlds[i]->name[0]);
			if (inputManager->IsActionPressed(Actions::Start))
			{
				gameManager->StoreWorldPosistion({ worlds[i]->transform.position.x, p1->player->transform.position.y,worlds[i]->transform.position.z - 1.0f });
				gameManager->LoadWorld();
				return;
			}
		}
		worlds[i]->rigidBody.myCollider.radius = originalRad;
		if (physicEngine->SphereVSphereCollision(p1->player, worlds[i]))
		{
			DirectX::XMFLOAT3 dir = { (p1->player->transform.position.x - worlds[i]->transform.position.x) * 30.0f ,
				(p1->player->transform.position.y - worlds[i]->transform.position.y) * 30.0f ,
				(p1->player->transform.position.z - worlds[i]->transform.position.z) * 30.0f };

			p1->player->rigidBody.ApplyForce(dir);
		}
	}

}
void MainHUBWorld::Quit()
{
	TyrianGameManager::Release();
}

		