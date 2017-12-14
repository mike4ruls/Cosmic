#include "StarHubWorld.h"
#include <stdio.h>


StarHubWorld::StarHubWorld(CosmicEngine* eng)
{
	engine = eng;
	cam = new FreeCamera();
}


StarHubWorld::~StarHubWorld()
{
	gameManager->SavePlayer(p1);
	if (p1 != nullptr) { delete p1; p1 = nullptr; }

	if (myShop != nullptr) { delete myShop; myShop = nullptr; }

	/*for (unsigned int i = 0; i < p1->frontBulletPool.size(); i++)
	{
		if (p1->frontBulletPool[i] != nullptr) { delete p1->frontBulletPool[i]; p1->frontBulletPool[i] = nullptr; }
	}*/
	for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
	{
		if (backgroundTilePool[i] != nullptr) { delete backgroundTilePool[i]; backgroundTilePool[i] = nullptr; }
	}
}

void StarHubWorld::Init()
{
	// ========== IMPORTANT ==========//
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;
	gameManager = TyrianGameManager::GetInstance();
	gameManager->SetEngine(engine);
	SetUpActions();
	InitUI();
	// ========== ====================//

	engine->rend->LoadSkyBox(2);
	engine->lockCamera = true;
	cam->lockCameraPos = true;
	engine->lockSunLight = true;
	engine->rend->skyBoxOn = false;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 20.0f, 0.0f);

	engine->rend->sunLight->ligComponent->lightDir = { -0.7f, -1.0f, 0.0f };

	currentState = GameState::Game;
	SetUpLevel();
	myShop = new ShopMenu(engine, p1);
	worldRotSpeed = 20.0f;

	levelCount = 4;

	blueStar = engine->CreateGameObject("BackGroundTile");
	blueStar->transform.Scale(0.6f);
	blueStar->rigidBody.myCollider.radius = 4.0f;
	blueStar->transform.position = { 0.0f, p1->player->transform.position.y, -7.0f };
	blueStar->transform.Translate(17.0f, 0.0f, 3.0f);
	blueStar->renderingComponent.mat.materialType = Material::MatType::Transulcent;
	blueStar->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("blueStar"));

	for (int i = 0; i < levelCount; i++)
	{
		levels.push_back(engine->CreateGameObject("BackGroundTile"));
		levels[i]->transform.Scale(0.6f);
		levels[i]->rigidBody.myCollider.radius = 4.0f;
		levels[i]->transform.position = { (-1.0f + (float)i) * 27, p1->player->transform.position.y, 0.0f };
		levels[i]->transform.Translate(17.0f, 0.0f, 3.0f);
		levels[i]->renderingComponent.mat.materialType = Material::MatType::Transulcent;

		switch (i) {
		case 0:
			levels[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("grayStar"));
			//grayStarPool[i]->isActive = true;
			//goldStarPool[i]->isActive = true;
			break;
		case 1:
			levels[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("grayStar"));
			levels[i]->transform.Translate(0.0f, 0.0f, 5.0f);
			break;
		case 2:
			levels[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("grayStar"));
			levels[i]->transform.Translate(0.0f, 0.0f, -2.0f);
			break;
		case 3:
			levels[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("redStar"));
			break;
		}

		goldStarPool.push_back(engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("goldStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
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

		grayStarPool.push_back(engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("grayStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
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


		levels[i]->renderingComponent.mat.surfaceColor = { 0.1f, 0.1f, 0.1f, 1.0f };

	}

	for (int i = 0; i < levelCount - 1; i++)
	{
		DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(levels[i+1]->transform.position.x - levels[i]->transform.position.x,
			0.0f,
			levels[i + 1]->transform.position.z - levels[i]->transform.position.z);
		DirectX::XMFLOAT3 genF = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);

		DirectX::XMVECTOR newForward = DirectX::XMLoadFloat3(&f);
		DirectX::XMVECTOR genForward = DirectX::XMLoadFloat3(&genF);

		float mag = DirectX::XMVectorGetX(DirectX::XMVector3Length(newForward));
		printf("%f", mag);

		newForward = DirectX::XMVector3AngleBetweenVectors(newForward, genForward);

		float turnAngle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(newForward));

		if(f.z > 0.0f)
		{
			turnAngle *= -1;
		}

		GameEntity* line = engine->CreateGameObject("BackGroundTile");
		line->transform.position = levels[i]->transform.position;
		line->transform.Translate(f.x / 2.0f, -0.4f, f.z / 2.0f);
		line->transform.Scale(mag / 10.0f, 1.0f, 0.08f);
		line->transform.Rotate(turnAngle, line->transform.rotation.y, line->transform.rotation.z);
		line->renderingComponent.mat.surfaceColor = { 0.2f, 0.2f, 0.0f, 1.0f };

		lineLevels.push_back(line);
	}
	levels[0]->renderingComponent.mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
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

void StarHubWorld::SetUpLevel()
{
	moveDownHeight = -30;

	tileSize = 8.5f;
	tileDistOffScreen = -50.0f;

	//posXConstraint = 114.0f;
	posXConstraint = 84.0f;

	negXConstraint = 34.0f;
	posZConstraint = 21.8f;
	negZConstraint = 22.5f;

	xCamConstraint = 24.0f;
	posZCamConstraint = 11.8f;
	negZCamConstraint = 12.5f;

	curCamPos = { 0.0f, 0.0f, 0.0f };

	camSpeed = 25.0f;

	/*xCamConstraint = 1.0f;
	posZCamConstraint = 1.0f;
	negZCamConstraint = 1.0f;*/

	CreatePlayer();
	LoadBackgroundTilePool("star");
}

void StarHubWorld::Update(float deltaTime, float totalTime)
{
	UpdateParticlesPos();
	switch (currentState)
	{
	case GameState::StartMenu:

		break;
	case GameState::Game:
		p1->Update(deltaTime);

		healthBar->SetWidth(healthBarBack->GetWidth() * (p1->topDisplayHealth / p1->maxHealth));
		healthBarFade->SetWidth(healthBarBack->GetWidth() * (p1->botDisplayHealth / p1->maxHealth));

		myShop->Update(deltaTime);

		blueStar->transform.Rotate(worldRotSpeed * -deltaTime, 0.0f, 0.0f);
		if (engine->physicEngine->SphereVSphereCollision(p1->player, blueStar))
		{
			blueStar->transform.Rotate(worldRotSpeed * 7 * -deltaTime, 0.0f, 0.0f);
			if (inputManager->IsActionPressed(Actions::Start))
			{
				gameManager->LoadHubWorld();
				return;
			}
		}

		for (unsigned int i = 0; i < p1->frontBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->frontBulletPool[i]->bullet->isActive)
			{
				p1->frontBulletPool[i]->Update(deltaTime);

				//for (unsigned int j = 0; j < enemyPool.size(); j++)
				//{
				//	if (engine->physicEngine->SphereVSphereCollision(bulletPool[i]->bullet, enemyPool[j]->enemyObj))
				//	{
				//		enemyPool[j]->TakeDamage(p1->atkDamage);
				//		bulletPool[i]->Deactivate();
				//		bulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
				//		bulletPool[i]->bullet->SetWorld();
				//	}
				//}
			}
		}
		for (unsigned int i = 0; i < p1->leftBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->leftBulletPool[i]->bullet->isActive)
			{
				p1->leftBulletPool[i]->Update(deltaTime);

				//for (unsigned int j = 0; j < enemyPool.size(); j++)
				//{
				//	if (engine->physicEngine->SphereVSphereCollision(bulletPool[i]->bullet, enemyPool[j]->enemyObj))
				//	{
				//		enemyPool[j]->TakeDamage(p1->atkDamage);
				//		bulletPool[i]->Deactivate();
				//		bulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
				//		bulletPool[i]->bullet->SetWorld();
				//	}
				//}
			}
		}
		for (unsigned int i = 0; i < p1->rightBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->rightBulletPool[i]->bullet->isActive)
			{
				p1->rightBulletPool[i]->Update(deltaTime);

				//for (unsigned int j = 0; j < enemyPool.size(); j++)
				//{
				//	if (engine->physicEngine->SphereVSphereCollision(bulletPool[i]->bullet, enemyPool[j]->enemyObj))
				//	{
				//		enemyPool[j]->TakeDamage(p1->atkDamage);
				//		bulletPool[i]->Deactivate();
				//		bulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
				//		bulletPool[i]->bullet->SetWorld();
				//	}
				//}
			}
		}

		for (unsigned int i = 0; i < levels.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			levels[i]->transform.Rotate(worldRotSpeed * deltaTime, 0.0f, 0.0f);
			if (engine->physicEngine->SphereVSphereCollision(p1->player, levels[i]))
			{
				if((gameManager->GetActiveLevels(Worlds::Star)[i] >= 1))
				{
					if (i == levelCount - 1) 
					{
						levels[i]->transform.Rotate(worldRotSpeed * 2 * deltaTime, 0.0f, 0.0f);
					}
					else
					{
						levels[i]->transform.Rotate(worldRotSpeed * 4 * deltaTime, 0.0f, 0.0f);
					}
					if (inputManager->IsActionPressed(Actions::Start))
					{
						gameManager->LoadLevel(i);
						return;
					}
				}
			}
		}
		CheckOutOfBounds();
		CalculateCamPos(deltaTime);

		if (inputManager->IsControllerConnected())
		{
			CheckControllerInputs(deltaTime);
		}
		else
		{
			CheckInputs(deltaTime);
		}

		break;
	case GameState::Paused:

		break;
	case GameState::EndLevel:

		break;
	}
	if (inputManager->IsKeyDown(96))
	{
		TyrianGameManager::Release();

		DefaultScene* defaultScene = new DefaultScene(engine);
		engine->rend->skyBoxOn = true;
		engine->LoadScene(defaultScene);
	}
}

void StarHubWorld::UpdateParticlesPos()
{
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw((p1->player->transform.rotation.y * 3.1415f) / 180, (p1->player->transform.rotation.x * 3.1415f) / 180, (p1->player->transform.rotation.z * 3.1415f) / 180);

	DirectX::XMVECTOR p1Pos = DirectX::XMLoadFloat3(&p1->player->transform.position);

	DirectX::XMVECTOR exhaustVec = DirectX::XMLoadFloat3(&exhaustPos);
	DirectX::XMVECTOR leftWingVec = DirectX::XMLoadFloat3(&leftWingPos);
	DirectX::XMVECTOR rightWingVec = DirectX::XMLoadFloat3(&rightWingPos);

	DirectX::XMStoreFloat3(&shipExhaust->transform.position, DirectX::XMVectorAdd(p1Pos, DirectX::XMVector3Transform(exhaustVec, rotMat)));
	DirectX::XMStoreFloat3(&leftWing->transform.position, DirectX::XMVectorAdd(p1Pos, DirectX::XMVector3Transform(leftWingVec, rotMat)));
	DirectX::XMStoreFloat3(&rightWing->transform.position, DirectX::XMVectorAdd(p1Pos, DirectX::XMVector3Transform(rightWingVec, rotMat)));

	shipExhaust->transform.foward = { p1->player->transform.foward.x * -1,  p1->player->transform.foward.y * -1, p1->player->transform.foward.z * -1 };
	leftWing->transform.foward = { p1->player->transform.foward.x * -1,  p1->player->transform.foward.y * -1, p1->player->transform.foward.z * -1 };
	rightWing->transform.foward = { p1->player->transform.foward.x * -1,  p1->player->transform.foward.y * -1, p1->player->transform.foward.z * -1 };

	shipExhaust->accelerationDir = shipExhaust->transform.foward;
	leftWing->accelerationDir = leftWing->transform.foward;
	rightWing->accelerationDir = rightWing->transform.foward;
}

void StarHubWorld::CheckInputs(float dt)
{
	if (!p1->isDead) {
		bool butCheckX = false;
		bool butCheckY = false;

		if (inputManager->IsActionDown(Actions::ButtonLeft)) //A
		{
			butCheckX = true;
			p1->currentTurnState = Player::LEFT;
			p1->previousTurnState = Player::LEFT;
			p1->player->transform.Translate((p1->player->transform.right.x * -p1->speed) * dt, 0.0f, (p1->player->transform.right.z * -p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonRight)) //D
		{
			butCheckX = true;
			p1->currentTurnState = Player::RIGHT;
			p1->previousTurnState = Player::RIGHT;
			p1->player->transform.Translate((p1->player->transform.right.x * p1->speed) * dt, 0.0f, (p1->player->transform.right.z * p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonUp)) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonDown)) //S
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * -p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * -p1->speed) * dt);
		}
		if (butCheckX || butCheckY)
		{

			if (shipExhaust->stopEmitting == true)
			{
				shipExhaust->Reset();
			}
			if (leftWing->stopEmitting == true)
			{
				leftWing->Reset();
			}
			if (rightWing->stopEmitting == true)
			{
				rightWing->Reset();
			}

			DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			DirectX::XMFLOAT3 genF = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

			DirectX::XMVECTOR newForward = DirectX::XMLoadFloat3(&f);
			DirectX::XMVECTOR genForward = DirectX::XMLoadFloat3(&genF);

			newForward = DirectX::XMVector3AngleBetweenVectors(newForward, genForward);

			float turnAngle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(newForward));

			if (0 < 0) {
				turnAngle *= -1;
			}

			p1->player->transform.rotation = { turnAngle, p1->player->transform.rotation.y, p1->player->transform.rotation.z };
			p1->originalRot = { turnAngle, 0.0f, 0.0f };
		}
		else {
			if (!p1->canStrafe)
			{
				shipExhaust->stopEmitting = true;
				leftWing->stopEmitting = true;
				rightWing->stopEmitting = true;
			}
		}
		if (inputManager->IsActionDown(Actions::Fire))
		{
			p1->ShootBullets();
		}
		if (inputManager->IsActionPressed(Actions::Strafe) && !p1->canStrafe)
		{
			p1->currentTurnState = Player::STRAIGHT;
			//p1->GainHealth(20.0f);
			//p1->rotLeft = rotLeft;
			p1->TurnOnStrafe();
			p1->speed = p1->normSpeed * 2;

			if (leftWing->stopEmitting == true)
			{
				leftWing->Reset();
			}
			if (rightWing->stopEmitting == true)
			{
				rightWing->Reset();
			}
		}
	}
	if (inputManager->IsActionPressed(Actions::Select))
	{
		myShop->UITurnedOn = myShop->UITurnedOn ? false : true;
	}
}

void StarHubWorld::CheckControllerInputs(float dt)
{
	if (!p1->isDead) {
		bool butCheckX = false;
		bool butCheckY = false;

		float LX = inputManager->GetLeftStickX();
		float LY = inputManager->GetLeftStickY();

		if (inputManager->IsActionDown(Actions::ButtonLeft) || LX < 0.0f) //A
		{
			butCheckX = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed)  * dt, 0.0f, (p1->player->transform.foward.z * p1->speed)  * dt);

			//statsButton
			//shopButton
			//upgradesButton
			//abilitiesButton
			//buyButton->posX -= 0.1f * dt;
			//printf("X: %f\n", buyButton->posX);
		}
		if (inputManager->IsActionDown(Actions::ButtonRight) || LX > 0.0f) //D
		{
			butCheckX = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed)  * dt, 0.0f, (p1->player->transform.foward.z * p1->speed)  * dt);

			//buyButton->posX += 0.1f * dt;
			//printf("X: %f\n", buyButton->posX);
		}

		if (inputManager->IsActionDown(Actions::ButtonUp) || LY > 0.0f) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			//buyButton->posY += 0.1f * dt;
			//printf("Y%f\n", buyButton->posY);
		}
		if (inputManager->IsActionDown(Actions::ButtonDown) || LY < 0.0f) //S
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			//buyButton->posY -= 0.1f * dt;
			//printf("Y: %f\n", buyButton->posY);
		}
		//if (inputManager->IsKeyDown(81)) //Q
		//{
		//	buyButton->SetWidth(buyButton->GetWidth() + (0.1f * dt));
		//	printf("Width: %f\n", buyButton->GetWidth());
		//}
		//if (inputManager->IsKeyDown(69)) //Q
		//{
		//	buyButton->SetWidth(buyButton->GetWidth() - (0.1f * dt));
		//	printf("Width: %f\n", buyButton->GetWidth());
		//}
		if (butCheckX || butCheckY)
		{

			if (shipExhaust->stopEmitting == true)
			{
				shipExhaust->Reset();
			}
			if (leftWing->stopEmitting == true)
			{
				leftWing->Reset();
			}
			if (rightWing->stopEmitting == true)
			{
				rightWing->Reset();
			}

			DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(LX, 0.0f, LY);
			DirectX::XMFLOAT3 genF = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

			DirectX::XMVECTOR newForward = DirectX::XMLoadFloat3(&f);
			DirectX::XMVECTOR genForward = DirectX::XMLoadFloat3(&genF);

			newForward = DirectX::XMVector3AngleBetweenVectors(newForward, genForward);

			float turnAngle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(newForward));

			if (LX < 0) {
				turnAngle *= -1;
			}

			p1->player->transform.rotation = { turnAngle, p1->player->transform.rotation.y, p1->player->transform.rotation.z };
			p1->originalRot = { turnAngle, 0.0f, 0.0f };
		}
		else {
			if (!p1->canStrafe)
			{
				shipExhaust->stopEmitting = true;
				leftWing->stopEmitting = true;
				rightWing->stopEmitting = true;
			}
		}
		if (inputManager->IsActionDown(Actions::Fire))
		{
			p1->ShootBullets();
		}
		if (inputManager->IsActionPressed(Actions::Strafe) && !p1->canStrafe)
		{
			p1->currentTurnState = Player::STRAIGHT;
			//p1->GainHealth(20.0f);
			//p1->rotLeft = rotLeft;
			p1->TurnOnStrafe();
			p1->speed = p1->normSpeed * 2;

			if (leftWing->stopEmitting == true)
			{
				leftWing->Reset();
			}
			if (rightWing->stopEmitting == true)
			{
				rightWing->Reset();
			}
		}
	}
	if (inputManager->IsActionPressed(Actions::Select))
	{
		myShop->UITurnedOn = myShop->UITurnedOn ? false : true;
	}
}

void StarHubWorld::CheckOutOfBounds()
{
	if (p1->player->transform.position.x <= -negXConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { -negXConstraint, pos.y, pos.z };
	}
	if (p1->player->transform.position.x >= posXConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { posXConstraint, pos.y, pos.z };
	}
	if (p1->player->transform.position.z >= posZConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { pos.x, pos.y, posZConstraint };
	}
	if (p1->player->transform.position.z <= -negZConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { pos.x, pos.y, -negZConstraint };
	}
}

void StarHubWorld::InitUI()
{
	healthBar = engine->CreateCanvasImage();
	healthBar->SetUIColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	healthBar->SetAlignment(UI::Alignment::Left);
	healthBar->posX = 0.3f;
	healthBar->posY = -1.0f;
	healthBar->SetWidth(0.6f);
	healthBar->SetHeight(0.02f);

	healthBarFade = engine->CreateCanvasImage();
	healthBarFade->SetUIColor({ 0.3f, 0.0f, 0.0f, 1.0f });
	healthBarFade->SetAlignment(healthBar->GetAlignment());
	healthBarFade->posX = healthBar->posX;
	healthBarFade->posY = healthBar->posY;
	healthBarFade->SetWidth(healthBar->GetWidth());
	healthBarFade->SetHeight(healthBar->GetHeight());

	healthBarBack = engine->CreateCanvasImage();
	healthBarBack->SetUIColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	healthBarBack->posX = healthBar->posX + healthBar->GetXOffSet();
	healthBarBack->posY = healthBar->posY + healthBar->GetYOffSet();
	healthBarBack->SetWidth(healthBar->GetWidth());
	healthBarBack->SetHeight(healthBar->GetHeight());

	healthBarBorder = engine->CreateCanvasImage();
	healthBarBorder->SetUIColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	healthBarBorder->posX = healthBar->posX + healthBar->GetXOffSet();
	healthBarBorder->posY = healthBar->posY + healthBar->GetYOffSet();
	healthBarBorder->SetWidth(healthBar->GetWidth() + 0.04f);
	healthBarBorder->SetHeight(healthBar->GetHeight() + 0.04f);
}

void StarHubWorld::LoadBackgroundTilePool(std::string textureName)
{
	unsigned int numOfTiles = 4;
	float tileDist = 12.2222221f * tileSize;

	for (unsigned int i = 0; i < numOfTiles; i++)
	{
		BackGroundTiles* newTile = new BackGroundTiles(engine->CreateGameObject("BackGroundTile"), { 0,0,-1.0f * 5.0f });
		newTile->tile->transform.Scale(tileSize + 2, tileSize + 1, tileSize + 1);
		newTile->tile->transform.Translate((i * tileDist), moveDownHeight, 0.0f);
		newTile->tile->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture(textureName));
		newTile->tile->renderingComponent.mat.uvXOffSet = 5.0f;
		newTile->tile->renderingComponent.mat.uvYOffSet = 5.0f;

		backgroundTilePool.push_back(newTile);
	}
}

void StarHubWorld::CreatePlayer()
{
	p1 = new Player(engine, engine->CreateGameObject("FighterShip"), 100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent.mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, -4.0f);
	p1->player->rigidBody.fricStrength = 60.0f;

	gameManager->LoadPlayer(p1);

	SetUpParticles();
}

void StarHubWorld::SetUpParticles()
{
	exhaustPos = { 0.0f, 0.3f, -1.3f };
	leftWingPos = { -0.7f, 0.3f, -1.3f };
	rightWingPos = { 0.7f, 0.3f, -1.3f };

	shipExhaust = engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("blackFire"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Cone);
	shipExhaust->transform.Rotate(0.0f, 200.0f, 0.0f);
	shipExhaust->transform.position = DirectX::XMFLOAT3(p1->player->transform.position.x + exhaustPos.x, p1->player->transform.position.y + exhaustPos.y, p1->player->transform.position.z + exhaustPos.z);
	shipExhaust->accelerationDir = shipExhaust->transform.foward;
	shipExhaust->emitterAcceleration = 622.0f;
	shipExhaust->startSize = 0.6f;
	shipExhaust->endSize = 0.2f;
	shipExhaust->startRadius = 0.0f;
	shipExhaust->endRadius = 1.0f;
	shipExhaust->emissionRate = 0.005f;
	shipExhaust->lifeTime = 0.15f;
	shipExhaust->localSpace = false;

	leftWing = engine->CreateParticalEmitter(200, engine->rend->assets->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive, Emitter::EmitterType::Cone);
	leftWing->transform.Rotate(0.0f, 180.0f, 0.0f);
	leftWing->transform.position = DirectX::XMFLOAT3(p1->player->transform.position.x + leftWingPos.x, p1->player->transform.position.y + leftWingPos.y, p1->player->transform.position.z + leftWingPos.z);
	leftWing->accelerationDir = leftWing->transform.foward;
	leftWing->emitterAcceleration = 10.0f;
	leftWing->startSize = 0.2f;
	leftWing->endSize = 0.2f;
	leftWing->startRadius = 0.0f;
	leftWing->endRadius = 0.0f;
	leftWing->emissionRate = 0.003f;
	leftWing->lifeTime = 0.5f;
	leftWing->localSpace = false;

	rightWing = engine->CreateParticalEmitter(200, engine->rend->assets->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive, Emitter::EmitterType::Cone);
	rightWing->transform.Rotate(0.0f, 180.0f, 0.0f);
	rightWing->transform.position = DirectX::XMFLOAT3(p1->player->transform.position.x + rightWingPos.x, p1->player->transform.position.y + rightWingPos.y, p1->player->transform.position.z + rightWingPos.z);
	rightWing->accelerationDir = leftWing->accelerationDir;
	rightWing->emitterAcceleration = leftWing->emitterAcceleration;
	rightWing->startSize = leftWing->startSize;
	rightWing->endSize = leftWing->endSize;
	rightWing->startRadius = leftWing->startRadius;
	rightWing->endRadius = leftWing->endRadius;
	rightWing->emissionRate = leftWing->emissionRate;
	rightWing->lifeTime = leftWing->lifeTime;
	rightWing->localSpace = leftWing->localSpace;
}

void StarHubWorld::CalculateCamPos(float dt)
{
	if (p1->player->transform.position.x <= (-xCamConstraint + cam->transform.position.x))
	{
		DirectX::XMFLOAT3 pos = cam->transform.position;
		curCamPos = { curCamPos.x + (-camSpeed * dt), curCamPos.y, curCamPos.z };
	}
	if (p1->player->transform.position.x >= (xCamConstraint + cam->transform.position.x))
	{
		curCamPos = { curCamPos.x + (camSpeed * dt), curCamPos.y, curCamPos.z };
	}
	if (p1->player->transform.position.z >= (posZCamConstraint + cam->transform.position.z))
	{
		curCamPos = { curCamPos.x, curCamPos.y, curCamPos.z + (camSpeed * dt) };
	}
	if (p1->player->transform.position.z <= (-negZCamConstraint + cam->transform.position.z))
	{
		curCamPos = { curCamPos.x, curCamPos.y, curCamPos.z + (-camSpeed * dt) };
	}

	cam->transform.position = { (p1->player->transform.position.x / 10.0f) + curCamPos.x, cam->transform.position.y, (p1->player->transform.position.z / 10.0f) + curCamPos.z };
}

void StarHubWorld::SetUpActions()
{
	inputManager->AddActionBinding(Actions::ButtonUp, { 87, CosmicInput::ControllerButton::DPAD_UP });
	inputManager->AddActionBinding(Actions::ButtonDown, { 83, CosmicInput::ControllerButton::DPAD_DOWN });
	inputManager->AddActionBinding(Actions::ButtonLeft, { 65, CosmicInput::ControllerButton::DPAD_LEFT });
	inputManager->AddActionBinding(Actions::ButtonRight, { 68, CosmicInput::ControllerButton::DPAD_RIGHT });
	inputManager->AddActionBinding(Actions::Fire, { VK_RETURN,  CosmicInput::ControllerButton::BUTTON_R2 });
	inputManager->AddActionBinding(Actions::Strafe, { VK_SPACE, CosmicInput::ControllerButton::BUTTON_CROSS });
	inputManager->AddActionBinding(Actions::Start, { 99, CosmicInput::ControllerButton::BUTTON_OPTIONS });
	inputManager->AddActionBinding(Actions::Select, { VK_TAB, CosmicInput::ControllerButton::BUTTON_TOUCH_PAD });
	inputManager->AddActionBinding(Actions::LB, { 8, CosmicInput::ControllerButton::BUTTON_L1 });
	inputManager->AddActionBinding(Actions::RB, { 8, CosmicInput::ControllerButton::BUTTON_R1 });
}

void StarHubWorld::CompletedLevel(int index)
{
	levels[index]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("goldStar"));
	if(index + 1 < levelCount)
	{
		levels[index + 1]->renderingComponent.mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		lineLevels[index]->renderingComponent.mat.surfaceColor = { 0.7f, 0.7f, 0.0f, 1.0f };
	}
	goldStarPool[index]->Reset();
	grayStarPool[index]->Reset();
}
void StarHubWorld::Quit()
{
	TyrianGameManager::Release();
}