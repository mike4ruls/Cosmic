#include "StarHubWorld.h"
#include <stdio.h>


StarHubWorld::StarHubWorld()
{
	cam = new FreeCamera();
}


StarHubWorld::~StarHubWorld()
{
	delete hubEngine;
	for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
	{
		if (backgroundTilePool[i] != nullptr) { delete backgroundTilePool[i]; backgroundTilePool[i] = nullptr; }
	}
}

void StarHubWorld::Init()
{
	hubEngine = new HubWorldEngine(cam, assetManager, sunLight);
	hubEngine->Init();

	p1 = hubEngine->p1;

	moveDownHeight = -30;
	worldRotSpeed = 20.0f;
	tileSize = 8.5f;
	tileDistOffScreen = -50.0f;

	hubEngine->posXConstraint = 84.0f;
	hubEngine->negXConstraint = 34.0f;
	hubEngine->posZConstraint = 21.8f;
	hubEngine->negZConstraint = 22.5f;
	hubEngine->xCamConstraint = 24.0f;
	hubEngine->posZCamConstraint = 11.8f;
	hubEngine->negZCamConstraint = 12.5f;

	LoadBackgroundTilePool("star");
	//engine->rend->sunLight->ligComponent->lightDir = { -0.7f, -1.0f, 0.0f };

	levelCount = 4;

	blueStar = new GameEntity(assetManager->GetMesh("BackGroundTile"), false);
	blueStar->transform.Scale(0.6f);
	blueStar->rigidBody.myCollider.radius = 4.0f;
	blueStar->transform.position = { 0.0f, p1->player->transform.position.y, -7.0f };
	blueStar->transform.Translate(17.0f, 0.0f, 3.0f);
	blueStar->renderingComponent->mat.materialType = Material::MatType::Transulcent;
	blueStar->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("blueStar"));

	for (int i = 0; i < levelCount; i++)
	{
		levels.push_back(new GameEntity(assetManager->GetMesh("BackGroundTile"), false));
		levels[i]->transform.Scale(0.6f);
		levels[i]->rigidBody.myCollider.radius = 4.0f;
		levels[i]->transform.position = { (-1.0f + (float)i) * 27, p1->player->transform.position.y, 0.0f };
		levels[i]->transform.Translate(17.0f, 0.0f, 3.0f);
		levels[i]->renderingComponent->mat.materialType = Material::MatType::Transulcent;

		switch (i) {
		case 0:
			levels[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("grayStar"));
			//grayStarPool[i]->isActive = true;
			//goldStarPool[i]->isActive = true;
			break;
		case 1:
			levels[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("grayStar"));
			levels[i]->transform.Translate(0.0f, 0.0f, 5.0f);
			break;
		case 2:
			levels[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("grayStar"));
			levels[i]->transform.Translate(0.0f, 0.0f, -2.0f);
			break;
		case 3:
			levels[i]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("redStar"));
			break;
		}

		goldStarPool.push_back(new Emitter(100, assetManager->GetSurfaceTexture("goldStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		goldStarPool[i]->transform.position = levels[i]->transform.position;
		goldStarPool[i]->transform.Rotate(0.0f, 90.0f, 0.0f);
		goldStarPool[i]->startRadius = 0.5f;
		goldStarPool[i]->endRadius = 30.5f;
		//goldStarPool[i]->endColor = goldStarPool[i]->startColor;
		goldStarPool[i]->lifeTime = 3.0f;
		goldStarPool[i]->emitterAcceleration = 1.0f;
		goldStarPool[i]->emissionRate = 0.5f;
		goldStarPool[i]->isActive = false;
		goldStarPool[i]->startSize = 2.0f;
		goldStarPool[i]->endSize = 3.3f;

		grayStarPool.push_back(new Emitter(100, assetManager->GetSurfaceTexture("grayStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		grayStarPool[i]->transform.position = levels[i]->transform.position;
		grayStarPool[i]->transform.Rotate(0.0f, 90.0f, 0.0f);
		grayStarPool[i]->startRadius = 0.5f;
		grayStarPool[i]->endRadius = 30.5f;
		//grayStarPool[i]->endColor = grayStarPool[i]->startColor;
		grayStarPool[i]->lifeTime = 3.0f;
		grayStarPool[i]->emitterAcceleration = 1.0f;
		grayStarPool[i]->emissionRate = 0.5f;
		grayStarPool[i]->isActive = false;
		grayStarPool[i]->startSize = 2.0f;
		grayStarPool[i]->endSize = 3.3f;


		levels[i]->renderingComponent->mat.surfaceColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	}

	for (int i = 0; i < levelCount - 1; i++)
	{
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(levels[i + 1]->transform.position.x - levels[i]->transform.position.x,
			0.0f,
			levels[i + 1]->transform.position.z - levels[i]->transform.position.z);
		DirectX::XMFLOAT3 genF = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

		DirectX::XMVECTOR newForward = DirectX::XMLoadFloat3(&f);
		DirectX::XMVECTOR genForward = DirectX::XMLoadFloat3(&genF);

		float mag = DirectX::XMVectorGetX(DirectX::XMVector3Length(newForward));
		printf("%f", mag);

		newForward = DirectX::XMVector3AngleBetweenVectors(newForward, genForward);

		float turnAngle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(newForward));

		if (f.z > 0.0f)
		{
			turnAngle *= -1;
		}

		GameEntity* line = new GameEntity(assetManager->GetMesh("BackGroundTile"), false);
		line->transform.position = levels[i]->transform.position;
		line->transform.Translate(f.x / 2.0f, -0.4f, f.z / 2.0f);
		line->transform.Scale(mag / 10.0f, 1.0f, 0.08f);
		line->transform.Rotate(turnAngle, line->transform.rotation.y, line->transform.rotation.z);
		line->renderingComponent->mat.surfaceColor = { 0.2f, 0.2f, 0.0f, 1.0f };

		lineLevels.push_back(line);
	}
	levels[0]->renderingComponent->mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	levels[3]->transform.Scale(2.0f);

	int* levelActive = gameManager->GetActiveLevels(Worlds::Star);

	for (int i = 0; i< levelCount; i++)
	{
		if (levelActive[i] == 2)
		{
			CompletedLevel(i);
		}
	}
}

void StarHubWorld::Update(float deltaTime, float totalTime)
{
	blueStar->transform.Rotate(worldRotSpeed * -deltaTime, 0.0f, 0.0f);
	if (physicEngine->SphereVSphereCollision(p1->player, blueStar))
	{
		blueStar->transform.Rotate(worldRotSpeed * 7 * -deltaTime, 0.0f, 0.0f);
		if (inputManager->IsActionPressed(Actions::Start) || inputManager->IsKeyPressed(KeyCode::E))
		{
			gameManager->LoadHubWorld();
			return;
		}
	}

	for (unsigned int i = 0; i < levels.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
	{
		levels[i]->transform.Rotate(worldRotSpeed * deltaTime, 0.0f, 0.0f);
		if (physicEngine->SphereVSphereCollision(p1->player, levels[i]))
		{
			if ((gameManager->GetActiveLevels(Worlds::Star)[i] >= 1))
			{
				if (i == levelCount - 1)
				{
					levels[i]->transform.Rotate(worldRotSpeed * 2 * deltaTime, 0.0f, 0.0f);
				}
				else
				{
					levels[i]->transform.Rotate(worldRotSpeed * 4 * deltaTime, 0.0f, 0.0f);
				}
				if (inputManager->IsActionPressed(Actions::Start) || inputManager->IsKeyPressed(KeyCode::E))
				{
					gameManager->StoreLevelPosistion(p1->player->transform.position);
					gameManager->LoadLevel(i);
					return;
				}
			}
		}
	}
	hubEngine->Update(deltaTime, totalTime);
}
void StarHubWorld::LoadBackgroundTilePool(std::string textureName)
{
	unsigned int numOfTiles = 4;
	float tileDist = 12.2222221f * tileSize;

	for (unsigned int i = 0; i < numOfTiles; i++)
	{
		BackGroundTiles* newTile = new BackGroundTiles(new GameEntity(assetManager->GetMesh("BackGroundTile"), false), { 0,0,-1.0f * 5.0f });
		newTile->tile->transform.Scale(tileSize + 2, tileSize + 1, tileSize + 1);
		newTile->tile->transform.Translate((i * tileDist), moveDownHeight, 0.0f);
		newTile->tile->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture(textureName));
		newTile->tile->renderingComponent->mat.uvXOffSet = 5.0f;
		newTile->tile->renderingComponent->mat.uvYOffSet = 5.0f;

		backgroundTilePool.push_back(newTile);
	}
}
void StarHubWorld::CompletedLevel(int index)
{
	levels[index]->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("goldStar"));
	if (index + 1 < levelCount)
	{
		levels[index + 1]->renderingComponent->mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		lineLevels[index]->renderingComponent->mat.surfaceColor = { 0.7f, 0.7f, 0.0f, 1.0f };
	}
	goldStarPool[index]->Reset();
	grayStarPool[index]->Reset();
}
void StarHubWorld::Quit()
{
	TyrianGameManager::Release();
}