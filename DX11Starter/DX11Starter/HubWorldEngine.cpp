#include "HubWorldEngine.h"
#include <stdio.h>


HubWorldEngine::HubWorldEngine(Camera* c, AssetManager* a, Light* sunL)
{
	assetManager = a;
	cam = c;
	sunLight = sunL;
	inputManager = InputManager::GetInstance();
}

HubWorldEngine::~HubWorldEngine()
{
	gameManager->SavePlayer(p1);
	if (p1 != nullptr) { delete p1; p1 = nullptr; }
	if (myShop != nullptr) { delete myShop; myShop = nullptr; }

	/*for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
	if (bulletPool[i] != nullptr) { delete bulletPool[i]; bulletPool[i] = nullptr; }
	}*/
}

void HubWorldEngine::Init()
{
	// ========== IMPORTANT ==========//
	gameManager = TyrianGameManager::GetInstance();
	SetUpActions();
	// ========== ====================//

	cam->lockCameraRot = true;
	cam->lockCameraPos = true;
	//engine->rend->skyBoxOn = true;
	cam->transform.Translate(0.0f, 20.0f, 0.0f);
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);

	sunLight->ligComponent->lightDir = { -0.7f, -1.0f, 0.0f };

	currentState = GameState::Game;
	SetUpLevel();
	InitUI();
	myShop = new ShopMenu(assetManager, p1);

}

void HubWorldEngine::SetUpLevel()
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

	curCamPos = { gameManager->GetLevelPosistion().x, 0.0f, gameManager->GetLevelPosistion().z };

	camSpeed = 25.0f;

	CreatePlayer();
}

void HubWorldEngine::Update(float deltaTime, float totalTime)
{
	UpdateParticlesPos();
	switch (currentState)
	{
	case GameState::StartMenu:

		break;
	case GameState::Game:
		p1->Update(deltaTime);
		p1->RechargeSheild(deltaTime);
		myShop->Update(deltaTime);

		for (unsigned int i = 0; i < p1->frontBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->frontBulletPool[i]->bullet->isActive)
			{
				p1->frontBulletPool[i]->Update(deltaTime);
			}
		}
		for (unsigned int i = 0; i < p1->leftBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->leftBulletPool[i]->bullet->isActive)
			{
				p1->leftBulletPool[i]->Update(deltaTime);
			}
		}
		for (unsigned int i = 0; i < p1->rightBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->rightBulletPool[i]->bullet->isActive)
			{
				p1->rightBulletPool[i]->Update(deltaTime);
			}
		}
		CheckOutOfBounds();
		CalculateCamPos(deltaTime);

		CheckControllerInputs(deltaTime);

		break;
	case GameState::Paused:

		break;
	case GameState::EndLevel:

		break;
	}
	if (inputManager->IsKeyDown(96))
	{
		TyrianGameManager::Release();

		SceneManager::LoadScene(SceneManager::_DefaultScene);
		//engine->rend->skyBoxOn = true;<------------------------------------------------
	}
}

void HubWorldEngine::UpdateParticlesPos()
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

void HubWorldEngine::CheckControllerInputs(float dt)
{
	if (!p1->isDead) {
		bool butCheckX = false;
		bool butCheckY = false;

		float LX = 0.0f;
		float LY = 0.0f;

		if (inputManager->IsControllerConnected())
		{
			LX = inputManager->GetLeftStickX();
			LY = inputManager->GetLeftStickY();
		}

		UI* uiToChange = nullptr;
//===================KeyBoard Inputs====================//
		if (inputManager->IsKeyDown(KeyCode::A)) //A
		{
			LX = -1.0f;
			butCheckX = true;
			p1->player->transform.Translate(-p1->speed * dt, 0.0f, 0.0f);
		}
		if (inputManager->IsKeyDown(KeyCode::D)) //D
		{
			LX = 1.0f;
			butCheckX = true;
			p1->player->transform.Translate(p1->speed * dt, 0.0f, 0.0f);
		}
		if (inputManager->IsKeyDown(KeyCode::W)) //W
		{
			LY = 1.0f;
			butCheckY = true;
			p1->player->transform.Translate(0.0f, 0.0f, p1->speed * dt);
		}
		if (inputManager->IsKeyDown(KeyCode::S)) //S
		{
			LY = -1.0f;
			butCheckY = true;
			p1->player->transform.Translate(0.0f, 0.0f, -p1->speed * dt);
		}
//========================================================//

//===================Controller Inputs====================//
		if (LX < 0.0f && !butCheckX) //A
		{
			butCheckX = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed)  * dt, 0.0f, (p1->player->transform.foward.z * p1->speed)  * dt);

			//statsButton
			//shopButton
			//upgradesButton
			//abilitiesButton
			if (uiToChange != nullptr) {
				uiToChange->posX -= 0.1f * dt;
				printf("X: %f\n", uiToChange->posX);
			}
		}
		if ( LX > 0.0f && !butCheckX) //D
		{
			butCheckX = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed)  * dt, 0.0f, (p1->player->transform.foward.z * p1->speed)  * dt);

			if (uiToChange != nullptr) {
				uiToChange->posX += 0.1f * dt;
				printf("X: %f\n", uiToChange->posX);
			}
		}

		if (LY > 0.0f && !butCheckY) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			if (uiToChange != nullptr) {
				uiToChange->posY += 0.1f * dt;
				printf("Y%f\n", uiToChange->posY);
			}
		}
		if (LY < 0.0f && !butCheckY) //S
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);

			if (uiToChange != nullptr) {
				uiToChange->posY -= 0.1f * dt;
				printf("Y: %f\n", uiToChange->posY);
			}
		}
		if (inputManager->IsKeyDown(KeyCode::Q)) //Q
		{
			if (uiToChange != nullptr) {
				/*uiToChange->SetWidth(uiToChange->GetWidth() + (0.1f * dt));
				printf("Width: %f\n", uiToChange->GetWidth());*/

				uiToChange->SetHeight(uiToChange->GetHeight() + (0.1f * dt));
				printf("Width: %f\n", uiToChange->GetHeight());
			}
		}
		if (inputManager->IsKeyDown(KeyCode::E)) //E
		{
			if (uiToChange != nullptr) {
				/*uiToChange->SetWidth(uiToChange->GetWidth() - (0.1f * dt));
				printf("Width: %f\n", uiToChange->GetWidth());*/

				uiToChange->SetHeight(uiToChange->GetHeight() - (0.1f * dt));
				printf("Width: %f\n", uiToChange->GetHeight());
			}
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
//========================================================//


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

void HubWorldEngine::CheckOutOfBounds()
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

void HubWorldEngine::InitUI()
{
	p1->healthBar->SetVisibility(true);
	p1->healthBarFade->SetVisibility(true);
	p1->healthBarBack->SetVisibility(true);
	p1->healthBarBorder->SetVisibility(true);

	if (p1->sheildComponentBought)
	{
		p1->sheildBar->SetVisibility(true);
		p1->sheildBarFade->SetVisibility(true);
		p1->sheildBarBack->SetVisibility(true);
		p1->sheildBarBorder->SetVisibility(true);
	}
}

void HubWorldEngine::CreatePlayer()
{
	p1 = new Player(new GameEntity(assetManager->GetMesh("FighterShip"), false), assetManager->GetMesh("Sphere"), 100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent->mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, -4.0f);
	p1->player->rigidBody.fricStrength = 60.0f;

	gameManager->LoadPlayer(p1);
	p1->player->transform.position = gameManager->GetLevelPosistion();

	SetUpParticles();
}

void HubWorldEngine::SetUpParticles()
{
	exhaustPos = { 0.0f, 0.3f, -1.3f };
	leftWingPos = { -0.7f, 0.3f, -1.3f };
	rightWingPos = { 0.7f, 0.3f, -1.3f };

	shipExhaust = new Emitter(100, assetManager->GetSurfaceTexture("blackFire"), Emitter::BlendingType::CutOut, Emitter::EmitterType::Cone);
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

	leftWing = new Emitter(200, assetManager->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive, Emitter::EmitterType::Cone);
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

	rightWing = new Emitter(200, assetManager->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive, Emitter::EmitterType::Cone);
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

void HubWorldEngine::CalculateCamPos(float dt)
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

void HubWorldEngine::SetUpActions()
{
	inputManager->AddActionBinding(Actions::ButtonUp, { KeyCode::W, CosmicInput::ControllerButton::DPAD_UP });
	inputManager->AddActionBinding(Actions::ButtonDown, { KeyCode::S, CosmicInput::ControllerButton::DPAD_DOWN });
	inputManager->AddActionBinding(Actions::ButtonLeft, { KeyCode::A, CosmicInput::ControllerButton::DPAD_LEFT });
	inputManager->AddActionBinding(Actions::ButtonRight, { KeyCode::D, CosmicInput::ControllerButton::DPAD_RIGHT });
	inputManager->AddActionBinding(Actions::Fire, { KeyCode::ENTER,  CosmicInput::ControllerButton::BUTTON_R2 });
	inputManager->AddActionBinding(Actions::Strafe, { KeyCode::SPACE, CosmicInput::ControllerButton::BUTTON_CROSS });
	inputManager->AddActionBinding(Actions::Start, { KeyCode::DELTE, CosmicInput::ControllerButton::BUTTON_OPTIONS });
	inputManager->AddActionBinding(Actions::Select, { KeyCode::TAB, CosmicInput::ControllerButton::BUTTON_TOUCH_PAD });
	inputManager->AddActionBinding(Actions::LB, { KeyCode::NUM_1, CosmicInput::ControllerButton::BUTTON_L1 });
	inputManager->AddActionBinding(Actions::RB, { KeyCode::NUM_2, CosmicInput::ControllerButton::BUTTON_R1 });
}
