#include "MainHUBWorld.h"
#include <stdio.h>


MainHUBWorld::MainHUBWorld(CosmicEngine* eng)
{
	engine = eng;
	cam = new FreeCamera();
}


MainHUBWorld::~MainHUBWorld()
{
	if (p1 != nullptr) { delete p1; p1 = nullptr; }

	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		if (bulletPool[i] != nullptr) { delete bulletPool[i]; bulletPool[i] = nullptr; }
	}
}

void MainHUBWorld::Init()
{
	// ========== IMPORTANT ==========//
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;
	SetUpActions();
	InitUI();
	// ========== ====================//

	engine->rend->LoadSkyBox(2);
	engine->lockCamera = true;
	cam->lockCameraPos = true;
	engine->lockSunLight = true;
	//engine->rend->skyBoxOn = false;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 20.0f, 0.0f);

	engine->rend->sunLight->ligComponent->lightDir = { -0.7f, -1.0f, 0.0f };
	engine->rend->instanceThreshold = 11;

	currentState = GameState::Game;
	SetUpLevel();
	worldRotSpeed = 20.0f;

	for (int i = 0; i < 10; i++)
	{
		worlds.push_back(engine->CreateGameObject("Worlds"));
		worlds[i]->transform.Scale(7.0f);
		worlds[i]->rigidBody.myCollider.radius = 4.0f;
		worlds[i]->transform.position = { (-1.0f + (float)i) * 17, p1->player->transform.position.y, 0.0f};
		worlds[i]->transform.Translate(17.0f, 0.0f, 3.0f);

		goldStarPool.push_back(engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("goldStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		goldStarPool[i]->transform.position = worlds[i]->transform.position;
		goldStarPool[i]->lifeTime = 5.0f;
		goldStarPool[i]->isActive = false;
		goldStarPool[i]->startSize = 1.0f;
		goldStarPool[i]->endSize = 1.3f;

		grayStarPool.push_back(engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("grayStar"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Sphere));
		grayStarPool[i]->transform.position = worlds[i]->transform.position;
		grayStarPool[i]->lifeTime = 5.0f;
		grayStarPool[i]->isActive = false;
		grayStarPool[i]->startSize = 1.0f;
		grayStarPool[i]->endSize = 1.3f;

		switch (i) {
		case 0:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture( engine->rend->assets->GetSurfaceTexture("grass"));
			//grayStarPool[i]->isActive = true;
			//goldStarPool[i]->isActive = true;
			break;
		case 1:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("water"));
			break;
		case 2:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("sand"));
			break;
		case 3:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("dry"));
			break;
		case 4:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("brick"));
			break;
		case 5:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("star"));
			break;
		case 6:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("lava"));
			break;
		case 7:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("lavaGround"));
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
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("checker"));
			break;
		case 9:
			worlds[i]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("rainbow"));
			break;
		}

		//worlds[i]->renderingComponent.mat.surfaceColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	}
	worlds[0]->renderingComponent.mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void MainHUBWorld::SetUpLevel()
{
	moveDownHeight = -30;

	//posXConstraint = 114.0f;
	posXConstraint = 164.0f;

	negXConstraint = 84.0f;
	posZConstraint = 31.8f;
	negZConstraint = 32.5f;

	xCamConstraint = 24.0f;
	posZCamConstraint = 11.8f;
	negZCamConstraint = 12.5f;

	curCamPos = {0.0f, 0.0f, 0.0f};

	camSpeed = 25.0f;

	shopUIcount = 0;
	/*xCamConstraint = 1.0f;
	posZCamConstraint = 1.0f;
	negZCamConstraint = 1.0f;*/

	CreatePlayer();
	LoadBulletPool();
}

void MainHUBWorld::Update(float deltaTime, float totalTime)
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

		if (metalTab->IsClicked()) { UITurnedOn = UITurnedOn ? false : true; }

		if (statsButton->IsClicked()) { shopUIcount = 0; }
		if (shopButton->IsClicked()) { shopUIcount = 1; }
		if (upgradesButton->IsClicked()) { shopUIcount = 2; }
		if (abilitiesButton->IsClicked()) { shopUIcount = 3; }

		if (UITurnedOn)
		{
			switch(shopUIcount)
			{
			case 0:
				if(!statsButton->constantHighlight)
				{
					TurnOnStatsUI();
					TurnOffShopUI();
					TurnOffUpgradesUI();
					TurnOffAbilitiesUI();
				}
				break;
			case 1:
				if (!shopButton->constantHighlight)
				{
					TurnOffStatsUI();
					TurnOnShopUI();
					TurnOffUpgradesUI();
					TurnOffAbilitiesUI();
				}
				break;
			case 2:
				if (!upgradesButton->constantHighlight)
				{
					TurnOffStatsUI();
					TurnOffShopUI();
					TurnOnUpgradesUI();
					TurnOffAbilitiesUI();
				}
				break;
			case 3:
				if (!abilitiesButton->constantHighlight)
				{
					TurnOffStatsUI();
					TurnOffShopUI();
					TurnOffUpgradesUI();
					TurnOnAbilitiesUI();
				}
				break;

			}
			if(metalBackGround->posY < -0.2f)
			{
				for(unsigned int i = 0; i < shopUI.size(); i++)
				{
					shopUI[i]->posY += UIMoveSpeed * deltaTime;
				}
			}
		}
		else 
		{
			if (metalBackGround->posY > UIMoveDownHieght)
			{
				for (unsigned int i = 0; i < shopUI.size(); i++)
				{
					shopUI[i]->posY -= UIMoveSpeed * deltaTime;
				}
			}
		}

		for (unsigned int i = 0; i < bulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (bulletPool[i]->bullet->isActive)
			{
				bulletPool[i]->Update(deltaTime);

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
		for (unsigned int i = 0; i < worlds.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			worlds[i]->transform.Rotate( worldRotSpeed * deltaTime, 0.0f,0.0f);
			if (engine->physicEngine->SphereVSphereCollision(p1->player, worlds[i]))
			{
				DirectX::XMFLOAT3 dir = { (p1->player->transform.position.x - worlds[i]->transform.position.x) * 30.0f ,
										  (p1->player->transform.position.y - worlds[i]->transform.position.y) * 30.0f ,
										  (p1->player->transform.position.z - worlds[i]->transform.position.z) * 30.0f  };

				p1->player->rigidBody.ApplyForce(dir);
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
		DefaultScene* defaultScene = new DefaultScene(engine);
		engine->rend->skyBoxOn = true;
		engine->LoadScene(defaultScene);
	}
}

void MainHUBWorld::UpdateParticlesPos()
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

void MainHUBWorld::CheckInputs(float dt)
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
		if (inputManager->IsActionDown(Actions::Fire) && p1->canAttack)
		{
			Shoot();
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
		UITurnedOn = UITurnedOn ? false : true;
	}
}

void MainHUBWorld::CheckControllerInputs(float dt)
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
			//abilitiesButton->posX -= 0.1f * dt;
			//printf("%f\n", abilitiesButton->posX);
		}
		if (inputManager->IsActionDown(Actions::ButtonRight) || LX > 0.0f) //D
		{
			butCheckX = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed)  * dt, 0.0f, (p1->player->transform.foward.z * p1->speed)  * dt);

			//abilitiesButton->posX += 0.1f * dt;
			//printf("%f\n", abilitiesButton->posX);
		}

		if (inputManager->IsActionDown(Actions::ButtonUp) || LY > 0.0f) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			//abilitiesButton->posY += 0.1f * dt;
			//printf("%f\n", abilitiesButton->posY);
		}
		if (inputManager->IsActionDown(Actions::ButtonDown) || LY < 0.0f) //S
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			//abilitiesButton->posY -= 0.1f * dt;
			//printf("%f\n", abilitiesButton->posY);
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
		
			DirectX::XMFLOAT3 f = DirectX::XMFLOAT3(LX, 0.0f, LY);
			DirectX::XMFLOAT3 genF = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

			DirectX::XMVECTOR newForward = DirectX::XMLoadFloat3(&f);
			DirectX::XMVECTOR genForward = DirectX::XMLoadFloat3(&genF);

			newForward = DirectX::XMVector3AngleBetweenVectors(newForward, genForward);

			float turnAngle = DirectX::XMConvertToDegrees(DirectX::XMVectorGetX(newForward));

			if (LX < 0) {
				turnAngle *= -1;
			}

			p1->player->transform.rotation = { turnAngle, p1->player->transform.rotation.y, p1->player->transform.rotation .z};
			p1->originalRot = { turnAngle, 0.0f, 0.0f };
		}
		else {
			if(!p1->canStrafe)
			{
				shipExhaust->stopEmitting = true;
				leftWing->stopEmitting = true;
				rightWing->stopEmitting = true;
			}
		}
		if (inputManager->IsActionDown(Actions::Fire) && p1->canAttack)
		{
			Shoot();
		}
		if (inputManager->IsActionPressed(Actions::LB))
		{
			if (shopUIcount > 0)
			{
				shopUIcount--;
			}
		}
		if (inputManager->IsActionPressed(Actions::RB))
		{
			if (shopUIcount < 3)
			{
				shopUIcount++;
			}
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
		UITurnedOn = UITurnedOn ? false : true;
	}
}

void MainHUBWorld::CheckOutOfBounds()
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

void MainHUBWorld::InitUI()
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

	UIMoveDownHieght = -2.510f;

	yellowTriangle = engine->CreateCanvasImage();
	yellowTriangle->SetAlignment(UI::Bottom);
	yellowTriangle->SetUIColor({1.0f, 0.0f, 0.0f, 1.0f});
	yellowTriangle->SetWidth(0.12f);
	yellowTriangle->SetHeight(0.04f);
	//metalTab->posY = -1.149f;
	yellowTriangle->posY = -0.213f;

	shopUI.push_back(yellowTriangle);

	metalTab = engine->CreateCanvasButton();
	metalTab->SetAlignment(UI::Bottom);
	metalTab->LoadTexture(engine->rend->assets->GetSurfaceTexture("metalTab"));
	metalTab->SetWidth(0.18f);
	metalTab->SetHeight(0.08f);
	//metalTab->posY = -1.149f;
	metalTab->posY = -0.259f;

	shopUI.push_back(metalTab);

	metalTopBar = engine->CreateCanvasImage();
	metalTopBar->LoadTexture(engine->rend->assets->GetSurfaceTexture("metalBack"));
	metalTopBar->SetAlignment(UI::Bottom);
	metalTopBar->obj->renderingComponent.mat.uvXOffSet = 3.0f;
	metalTopBar->obj->renderingComponent.mat.uvYOffSet = 0.2f;
	metalTopBar->SetWidth(2.1f);
	metalTopBar->SetHeight(0.04f);
	metalTopBar->posY = -0.668f;

	shopUI.push_back(metalTopBar);

	float buttonHieght = -0.584f;

	statsButton = engine->CreateCanvasButton();
	statsButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("statsText"));
	statsButton->SetAlignment(UI::Bottom);
	statsButton->SetWidth(0.525f);
	statsButton->SetHeight(0.1f);
	statsButton->posX = -1.569f;
	statsButton->posY = buttonHieght;

	shopUI.push_back(statsButton);

	shopButton = engine->CreateCanvasButton();
	shopButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("shopText"));
	shopButton->SetAlignment(UI::Bottom);
	shopButton->SetWidth(0.525f);
	shopButton->SetHeight(0.1f);
	shopButton->posX = -0.516f;
	shopButton->posY = buttonHieght;

	shopUI.push_back(shopButton);

	upgradesButton = engine->CreateCanvasButton();
	upgradesButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("upgradesText"));
	upgradesButton->SetAlignment(UI::Bottom);
	upgradesButton->SetWidth(0.525f);
	upgradesButton->SetHeight(0.1f);
	upgradesButton->posX = 0.536f;
	upgradesButton->posY = buttonHieght;

	shopUI.push_back(upgradesButton);

	abilitiesButton = engine->CreateCanvasButton();
	abilitiesButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("abilitiesText"));
	abilitiesButton->SetAlignment(UI::Bottom);
	abilitiesButton->SetWidth(0.525f);
	abilitiesButton->SetHeight(0.1f);
	abilitiesButton->posX = 1.57f;
	abilitiesButton->posY = buttonHieght;

	shopUI.push_back(abilitiesButton);


	metalForeGround = engine->CreateCanvasImage();
	metalForeGround->SetUIColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	metalForeGround->SetWidth(2.1f);
	metalForeGround->SetHeight(0.90f);
	metalForeGround->posY = UIMoveDownHieght + 0.23f;

	shopUI.push_back(metalForeGround);

	metalBackGround = engine->CreateCanvasImage();
	metalBackGround->LoadTexture(engine->rend->assets->GetSurfaceTexture("metalBack")); 
	metalBackGround->SetWidth(2.22f);
	metalBackGround->SetHeight(1.25f);
	metalBackGround->obj->renderingComponent.mat.uvXOffSet = 3.5f;
	metalBackGround->obj->renderingComponent.mat.uvYOffSet = 3.0f;
	metalBackGround->posY = UIMoveDownHieght;

	shopUI.push_back(metalBackGround);


	UIMoveSpeed = 5.0f;
	UITurnedOn = false;
}

void MainHUBWorld::CreatePlayer()
{
	p1 = new Player(engine->CreateGameObject("FighterShip"), 100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent.mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, -4.0f);
	p1->player->rigidBody.fricStrength = 60.0f;

	SetUpParticles();
}

void MainHUBWorld::SetUpParticles()
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

void MainHUBWorld::LoadBulletPool()
{
	unsigned int numOfBullets = 40;

	for (unsigned int i = 0; i < numOfBullets; i++)
	{
		Bullet* newBullet = new Bullet(engine->CreateGameObject("Sphere"));
		newBullet->bullet->transform.Translate(0.0f, 60.0f, 0.0f);
		newBullet->bullet->SetWorld();
		//newBullet->bullet->renderingComponent.mat.surfaceReflectance = 0.7f;

		bulletPool.push_back(newBullet);
	}
}

void MainHUBWorld::Shoot()
{
	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		if (!bulletPool[i]->bullet->isActive)
		{
			bulletPool[i]->Activate(p1->FindDistAway(p1->player->transform.foward, 2.0f), { p1->player->transform.foward.x * 80.0f, p1->player->transform.foward.y * 80.0f, p1->player->transform.foward.z * 80.0f }, Bullet::Regular);
			p1->canAttack = false;
			break;
		}
	}
}

void MainHUBWorld::CalculateCamPos(float dt)
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

void MainHUBWorld::SetUpActions()
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

void MainHUBWorld::TurnOnStatsUI()
{
	statsButton->constantHighlight = true;
}

void MainHUBWorld::TurnOnShopUI()
{
	shopButton->constantHighlight = true;
}

void MainHUBWorld::TurnOnUpgradesUI()
{
	upgradesButton->constantHighlight = true;
}

void MainHUBWorld::TurnOnAbilitiesUI()
{
	abilitiesButton->constantHighlight = true;
}

void MainHUBWorld::TurnOffStatsUI()
{
	statsButton->constantHighlight = false;
}

void MainHUBWorld::TurnOffShopUI()
{
	shopButton->constantHighlight = false;
}

void MainHUBWorld::TurnOffUpgradesUI()
{
	upgradesButton->constantHighlight = false;
}

void MainHUBWorld::TurnOffAbilitiesUI()
{
	abilitiesButton->constantHighlight = false;
}
