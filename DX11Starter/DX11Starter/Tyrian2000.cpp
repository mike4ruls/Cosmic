#include "Tyrian2000.h"



Tyrian2000::Tyrian2000(CosmicEngine* eng)
{
	engine = eng;
	cam = new FreeCamera();
}


Tyrian2000::~Tyrian2000()
{
	if (p1 != nullptr) { delete p1; p1 = nullptr; }

	if (fLine != nullptr) { delete fLine; fLine = nullptr; }

	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		if (bulletPool[i] != nullptr) { delete bulletPool[i]; bulletPool[i] = nullptr; }
	}
	for (unsigned int i = 0; i < enemyPool.size(); i++)
	{
		if (enemyPool[i] != nullptr) { delete enemyPool[i]; enemyPool[i] = nullptr; }
	}
	for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
	{
		if (backgroundTilePool[i] != nullptr) { delete backgroundTilePool[i]; backgroundTilePool[i] = nullptr; }
	}
}

void Tyrian2000::Init()
{
	// ========== IMPORTANT ==========//
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;
	SetUpActions();
	InitUI();
	// ========== ====================//

	engine->lockCamera = true;
	cam->lockCameraPos = true;
	engine->lockSunLight = true;
	engine->rend->skyBoxOn = false;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 5.0f, 0.0f);

	engine->rend->sunLight->ligComponent->lightDir = {0.7f, -0.5f, 0.0f};

	currentState = GameState::StartMenu;
	SetUpLevel();
}

void Tyrian2000::SetUpLevel()
{
	endGame = false;
	waveCount = 3;
	currentWave = 0;
	pauseOption = 0;

	waveSpawnCD = 5.0f;
	waveSpawnTimer = waveSpawnCD;

	moveDownHeight = -30;

	xConstraint = 24.0f;
	posZConstraint = 11.8f;
	negZConstraint = 12.5f;

	tileSize = 4.5f;
	tileDistOffScreen = -50.0f;

	CreatePlayer();
	CreateFinishLine();
	SpawnWaveEnemies();
	SpawnWaveBlockers();
	LoadBulletPool();
	LoadBackgroundTilePool("grass");

}

void Tyrian2000::Update(float deltaTime, float totalTime)
{
	UpdateParticlesPos();
	switch (currentState)
	{
	case GameState::StartMenu:
		if(startButton->IsClicked() || inputManager->IsActionPressed(Actions::Start) || inputManager->IsActionPressed(Actions::Strafe))
		{
			startButton->SetVisibility(false);
			startButton->SetActive(false);

			Tyrian2000Logo->SetVisibility(false);

			healthBar->SetVisibility(true);
			healthBarFade->SetVisibility(true);
			healthBarBack->SetVisibility(true);
			healthBarBorder->SetVisibility(true);


			currentState = GameState::Game;
		}
		else
		{
			for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
			{
				if (backgroundTilePool[i]->tile->transform.position.z <= tileDistOffScreen)
				{
					backgroundTilePool[i]->Teleport({ 0.0f, 0.0f, (12.2222221f * tileSize) *  backgroundTilePool.size() });
				}
				backgroundTilePool[i]->Update(deltaTime);
			}
			CalculateCamPos();
		}

		break;
	case GameState::Game:
		p1->Update(deltaTime);

		healthBar->SetWidth(healthBarBack->GetWidth() * (p1->topDisplayHealth / p1->maxHealth));
		healthBarFade->SetWidth(healthBarBack->GetWidth() * (p1->botDisplayHealth / p1->maxHealth));

		if (waveSpawnTimer <= 0.0f && currentWave < waveCount)
		{
			if (!fLine->finishLine->isActive)
			{
				SpawnWaveEnemies();
				SpawnWaveBlockers();
				currentWave++;
			}

			waveSpawnTimer = waveSpawnCD;
		}

		if (currentWave == waveCount)
		{
			fLine->finishLine->SetActive(true);
			currentWave++;
		}
		if (fLine->finishLine->isActive && p1->player->transform.position.z >= fLine->finishLine->transform.position.z)
		{
			fLine->finishLine->SetActive(false);
			ChooseEndPanelText();
		}
		if (p1->isDead && !endGame)
		{
			ChooseEndPanelText();
		}
		fLine->Update(deltaTime);
		for (unsigned int i = 0; i < bulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (bulletPool[i]->bullet->isActive)
			{
				bulletPool[i]->Update(deltaTime);

				for (unsigned int j = 0; j < enemyPool.size(); j++)
				{
					if (engine->physicEngine->SphereVSphereCollision(bulletPool[i]->bullet, enemyPool[j]->enemyObj))
					{
						enemyPool[j]->TakeDamage(p1->atkDamage);
						bulletPool[i]->Deactivate();
						bulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
						bulletPool[i]->bullet->SetWorld();
					}
				}
			}
		}
		for (unsigned int i = 0; i < enemyPool.size(); i++)
		{
			enemyPool[i]->Update(deltaTime);
			if (enemyPool[i]->isDead || enemyPool[i]->enemyObj->transform.position.z <= -negZConstraint - 2) {
				KillEnemy(i);
				continue;
			}
			if (engine->physicEngine->SphereVSphereCollision(p1->player, enemyPool[i]->enemyObj))
			{
				p1->TakeDamage(enemyPool[i]->damage);
			}

		}
		for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
		{
			if (backgroundTilePool[i]->tile->transform.position.z <= tileDistOffScreen)
			{
				backgroundTilePool[i]->Teleport({ 0.0f, 0.0f, (12.2222221f * tileSize) *  backgroundTilePool.size() });
			}
			backgroundTilePool[i]->Update(deltaTime);
		}

		CheckOutOfBounds();
		CalculateCamPos();

		if (inputManager->IsControllerConnected())
		{
			CheckControllerInputs(deltaTime);
		}
		else
		{
			CheckInputs(deltaTime);
		}
		waveSpawnTimer -= deltaTime;
		break;
	case GameState::Paused:
		if(inputManager->IsActionPressed(Actions::ButtonDown))
		{
			if(pauseOption < 3)
			{
				pauseOption++;

				switch (pauseOption)
				{
				case 1:
					resumeButton->constantHighlight = true;
					retryButton->constantHighlight = false;
					quitButton->constantHighlight = false;
					break;
				case 2:
					resumeButton->constantHighlight = false;
					retryButton->constantHighlight = true;
					quitButton->constantHighlight = false;
					break;
				case 3:
					resumeButton->constantHighlight = false;
					retryButton->constantHighlight = false;
					quitButton->constantHighlight = true;
					break;
				}
			}
		}
		if (inputManager->IsActionPressed(Actions::ButtonUp))
		{
			if (pauseOption > 1)
			{
				pauseOption--;

				switch (pauseOption)
				{
				case 1:
					resumeButton->constantHighlight = true;
					retryButton->constantHighlight = false;
					quitButton->constantHighlight = false;
					break;
				case 2:
					resumeButton->constantHighlight = false;
					retryButton->constantHighlight = true;
					quitButton->constantHighlight = false;
					break;
				case 3:
					resumeButton->constantHighlight = false;
					retryButton->constantHighlight = false;
					quitButton->constantHighlight = true;
					break;
				}
			}
		}


		if (inputManager->IsActionPressed(Actions::Start) || resumeButton->IsClicked() || (pauseOption == 1 && inputManager->IsActionDown(Actions::Strafe)))
		{
			currentState = GameState::Game;

			pauseOption = 0;
			resumeButton->constantHighlight = false;
			retryButton->constantHighlight = false;
			quitButton->constantHighlight = false;

			TogglePauseMenu();
		}
		else if(retryButton->IsClicked() || (pauseOption == 2 && inputManager->IsActionDown(Actions::Strafe)))
		{
			currentState = GameState::Game;

			resumeButton->constantHighlight = false;
			retryButton->constantHighlight = false;
			quitButton->constantHighlight = false;

			ResetLevel();
			TogglePauseMenu();
		}
		else if (quitButton->IsClicked() || (pauseOption == 3 && inputManager->IsActionDown(Actions::Strafe)))
		{
			Tyrian2000* tyrian = new Tyrian2000(engine);
			engine->LoadScene(tyrian);
		}
		break;
	case GameState::EndLevel:
		if (inputManager->IsActionPressed(Actions::Start) || inputManager->IsActionPressed(Actions::Strafe) || endRetryButton->IsClicked() || endContinueButton->IsClicked())
		{
			if(p1->isDead)
			{
				currentState = GameState::Game;
				endRetryButton->SetActive(false);
				endRetryButton->SetVisibility(false);
				endGamePanel->SetVisibility(false);
				ResetLevel();
			}
			else
			{
				Tyrian2000* tyrian = new Tyrian2000(engine);
				engine->LoadScene(tyrian);
			}
		}
		break;
	}
	if (inputManager->IsKeyDown(96))
	{
		DefaultScene* defaultScene = new DefaultScene(engine);
		engine->rend->skyBoxOn = true;
		engine->LoadScene(defaultScene);
	}
}
void Tyrian2000::UpdateParticlesPos()
{
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw((p1->player->transform.rotation.y * 3.1415f) / 180, (p1->player->transform.rotation.x * 3.1415f) / 180, (p1->player->transform.rotation.z * 3.1415f) / 180);

	DirectX::XMVECTOR p1Pos = DirectX::XMLoadFloat3(&p1->player->transform.position);

	DirectX::XMVECTOR exhaustVec = DirectX::XMLoadFloat3(&exhaustPos);
	DirectX::XMVECTOR leftWingVec = DirectX::XMLoadFloat3(&leftWingPos);
	DirectX::XMVECTOR rightWingVec = DirectX::XMLoadFloat3(&rightWingPos);

	DirectX::XMStoreFloat3(&shipExhaust->transform.position, DirectX::XMVectorAdd(p1Pos,DirectX::XMVector3Transform(exhaustVec, rotMat)));
	DirectX::XMStoreFloat3(&leftWing->transform.position, DirectX::XMVectorAdd(p1Pos, DirectX::XMVector3Transform(leftWingVec, rotMat)));
	DirectX::XMStoreFloat3(&rightWing->transform.position, DirectX::XMVectorAdd(p1Pos, DirectX::XMVector3Transform(rightWingVec, rotMat)));
}

void Tyrian2000::CheckInputs(float dt)
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
		if (!butCheckX)
		{
			p1->currentTurnState = Player::STRAIGHT;
		}
		if (inputManager->IsActionDown(Actions::Fire) && p1->canAttack)
		{
			Shoot();
		}
		if (inputManager->IsActionPressed(Actions::Strafe) && !p1->canStrafe)
		{
			p1->GainHealth(20.0f);
			//p1->rotLeft = rotLeft;
			p1->TurnOnStrafe();
			if (p1->currentTurnState == Player::STRAIGHT && butCheckY)
			{
				p1->speed = p1->normSpeed;
			}
			else if (p1->previousTurnState == Player::LEFT)
			{
				p1->player->rigidBody.ApplyForce(-p1->strafeForce, 0.0f, 0.0f);
			}
			else if (p1->previousTurnState == Player::RIGHT)
			{
				p1->player->rigidBody.ApplyForce(p1->strafeForce, 0.0f, 0.0f);
			}
		}
	}
	if (inputManager->IsActionPressed(Actions::Start))
	{
		currentState = GameState::Paused;
		TogglePauseMenu();
	}
	/*if(engine->IsKeyPressed(VK_CONTROL) && engine->IsKeyDown(67))
	{
		cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	}*/
}

void Tyrian2000::CheckControllerInputs(float dt)
{
	if (!p1->isDead) {
		bool butCheckX = false;
		bool butCheckY = false;

		float LX = inputManager->GetLeftStickX();
		float LY = inputManager->GetLeftStickY();

		if (inputManager->IsActionDown(Actions::ButtonLeft) || LX < 0.0f) //A
		{
			butCheckX = true;
			p1->currentTurnState = Player::LEFT;
			p1->previousTurnState = Player::LEFT;
			p1->player->transform.Translate((p1->player->transform.right.x * -p1->speed) * dt, 0.0f, (p1->player->transform.right.z * -p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonRight) || LX > 0.0f) //D
		{
			butCheckX = true;
			p1->currentTurnState = Player::RIGHT;
			p1->previousTurnState = Player::RIGHT;
			p1->player->transform.Translate((p1->player->transform.right.x * p1->speed) * dt, 0.0f, (p1->player->transform.right.z * p1->speed) * dt);
		}

		if (inputManager->IsActionDown(Actions::ButtonUp) || LY > 0.0f) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonDown) || LY < 0.0f) //S
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * -p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * -p1->speed) * dt);
		}
		if (!butCheckX)
		{
			p1->currentTurnState = Player::STRAIGHT;
		}
		if (inputManager->IsActionDown(Actions::Fire) && p1->canAttack)
		{
			Shoot();
		}
		if (inputManager->IsActionPressed(Actions::Strafe) && !p1->canStrafe)
		{
			//p1->GainHealth(20.0f);
			//p1->rotLeft = rotLeft;
			p1->TurnOnStrafe();
			if (p1->currentTurnState == Player::STRAIGHT && butCheckY)
			{
				p1->speed = p1->normSpeed;
			}
			else if (p1->previousTurnState == Player::LEFT)
			{
				p1->player->rigidBody.ApplyForce(-p1->strafeForce, 0.0f, 0.0f);
			}
			else if (p1->previousTurnState == Player::RIGHT)
			{
				p1->player->rigidBody.ApplyForce(p1->strafeForce, 0.0f, 0.0f);
			}
		}
	}
	if (inputManager->IsActionPressed(Actions::Start))
	{
		currentState = GameState::Paused;
		TogglePauseMenu();
	}
	/*if(engine->IsKeyPressed(VK_CONTROL) && engine->IsKeyDown(67))
	{
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	}*/
}

void Tyrian2000::CheckOutOfBounds()
{
	if(p1->player->transform.position.x <= -xConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { -xConstraint, pos.y, pos.z};
	}
	if(p1->player->transform.position.x >= xConstraint)
	{
		DirectX::XMFLOAT3 pos = p1->player->transform.position;
		p1->player->transform.position = { xConstraint, pos.y, pos.z };
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

void Tyrian2000::InitUI()
{
	float pauseButtondist = 0.00f;
	float endButtondist = 1.0f;

	startButton = engine->CreateCanvasButton();
	startButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("startButtontext"));
	startButton->SetWidth(0.3f);
	startButton->SetHeight(0.1f);
	startButton->posY = -0.7f;
	//startButton->SetVisibility(false);

	resumeButton = engine->CreateCanvasButton();
	resumeButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("resumeButtontext"));
	resumeButton->SetWidth(0.5f);
	resumeButton->SetHeight(0.1f);
	resumeButton->posY = -pauseButtondist;
	resumeButton->SetVisibility(false);
	resumeButton->SetActive(false);

	retryButton = engine->CreateCanvasButton();
	retryButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("retryButtontext"));
	retryButton->SetWidth(0.5f);
	retryButton->SetHeight(0.1f);
	retryButton->posY = -0.4f - pauseButtondist;
	retryButton->SetVisibility(false);
	retryButton->SetActive(false);

	quitButton = engine->CreateCanvasButton();
	quitButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("quitButtontext"));
	quitButton->SetWidth(0.5f);
	quitButton->SetHeight(0.1f);
	quitButton->posY = -0.8f - pauseButtondist;
	quitButton->SetVisibility(false);
	quitButton->SetActive(false);

	endRetryButton = engine->CreateCanvasButton();
	endRetryButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("retryButtontext"));
	endRetryButton->SetWidth(0.5f);
	endRetryButton->SetHeight(0.1f);
	endRetryButton->posY = -endButtondist;
	endRetryButton->SetVisibility(false);
	endRetryButton->SetActive(false);

	endContinueButton = engine->CreateCanvasButton();
	endContinueButton->LoadTexture(engine->rend->assets->GetSurfaceTexture("continueButtontext"));
	endContinueButton->SetWidth(0.5f);
	endContinueButton->SetHeight(0.1f);
	endContinueButton->posY = -endButtondist;
	endContinueButton->SetVisibility(false);
	endContinueButton->SetActive(false);

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

	endGamePanel = engine->CreateCanvasImage();
	endGamePanel->SetWidth(0.8f);
	endGamePanel->SetHeight(0.8f);


	healthBar->SetVisibility(false);
	healthBarFade->SetVisibility(false);
	healthBarBack->SetVisibility(false);
	healthBarBorder->SetVisibility(false);
	endGamePanel->SetVisibility(false);

	Tyrian2000Logo = engine->CreateCanvasImage();
	Tyrian2000Logo->LoadTexture(engine->rend->assets->GetSurfaceTexture("tyrian2000Logo"));
	Tyrian2000Logo->SetHeight(0.5f);
	Tyrian2000Logo->SetHeight(0.3f);
	Tyrian2000Logo->posY = 0.6f;
	//Tyrian2000Logo->SetVisibility(false);

	pausedLogo = engine->CreateCanvasImage();
	pausedLogo->LoadTexture(engine->rend->assets->GetSurfaceTexture("pausedLogo"));
	pausedLogo->SetHeight(0.5f);
	pausedLogo->SetHeight(0.3f);
	pausedLogo->posY = 0.6f;
	pausedLogo->SetVisibility(false);
}

void Tyrian2000::CreatePlayer()
{
	p1 = new Player(engine->CreateGameObject("FighterShip"), 100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent.mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, -4.0f);

	SetUpParticles();
}

void Tyrian2000::SetUpParticles()
{
	exhaustPos = {0.0f, 0.3f, -1.3f};
	leftWingPos = {-0.7f, 0.3f, -1.3f};
	rightWingPos = { 0.7f, 0.3f, -1.3f};

	shipExhaust = engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("blackFire"), Emitter::BlendingType::CutOut);
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

	leftWing = engine->CreateParticalEmitter(200, engine->rend->assets->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive);
	leftWing->transform.Rotate(0.0f, 180.0f, 0.0f);
	leftWing->transform.position = DirectX::XMFLOAT3(p1->player->transform.position.x + leftWingPos.x, p1->player->transform.position.y + leftWingPos.y, p1->player->transform.position.z + leftWingPos.z);
	leftWing->accelerationDir = leftWing->transform.foward;
	leftWing->emitterAcceleration = 622.0f;
	leftWing->startSize = 0.2f;
	leftWing->endSize = 0.2f;
	leftWing->startRadius = 0.0f;
	leftWing->endRadius = 0.0f;
	leftWing->emissionRate = 0.002f;
	leftWing->lifeTime = 0.5f;
	leftWing->localSpace = false;

	rightWing = engine->CreateParticalEmitter(200, engine->rend->assets->GetSurfaceTexture("whiteSmoke"), Emitter::BlendingType::Additive);
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

void Tyrian2000::CreateFinishLine()
{
	fLine = new FinishLine(engine->CreateGameObject("Cube"), 2.0f);
	fLine->finishLine->transform.Translate(0.0f, p1->player->transform.position.y, 18.0f); //18
	fLine->finishLine->transform.Scale(43.0f, 1.0f, 1.0f);
	fLine->finishLine->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("checker"));
	fLine->finishLine->renderingComponent.mat.uvXOffSet = 0.25f;
	fLine->finishLine->renderingComponent.mat.uvYOffSet = 10.0f;
	fLine->finishLine->SetWorld();
}

void Tyrian2000::SpawnWaveEnemies()
{
	int numOfEnemies = 10;
	for(int i = 0; i < numOfEnemies; i++)
	{
		Enemy* newEnemy = new Enemy(engine->CreateGameObject("Cone"), 4.0f, -3.0f, 1.0f, Enemy::EnemyType::Regular);
		newEnemy->enemyObj->transform.Translate(-18.0f + (i*4), p1->player->transform.position.y, 18.0f);
		newEnemy->enemyObj->transform.Scale(1.3f);
		newEnemy->enemyObj->transform.Rotate(0.0f, 0.0f, 90.0f);
		enemyPool.push_back(newEnemy);
	}

}

void Tyrian2000::SpawnWaveBlockers()
{
	int numOfEnemies = 15;
	for (int i = 0; i < numOfEnemies; i++)
	{
		Enemy* newEnemy = new Enemy(engine->CreateGameObject("Sphere"), 15.0f, -2.0f, 3.0f, Enemy::EnemyType::ZigZag);
		newEnemy->enemyObj->transform.Translate(-20.0f + (i * 3), p1->player->transform.position.y, 22.0f);
		newEnemy->enemyObj->transform.Scale(2.0f);
		enemyPool.push_back(newEnemy);
	}
}

void Tyrian2000::LoadBulletPool()
{
	unsigned int numOfBullets = 40;

	for (unsigned int i = 0; i < numOfBullets; i++) 
	{
		Bullet* newBullet = new Bullet(engine->CreateGameObject("Sphere"));
		newBullet->bullet->transform.Translate(0.0f, 10.0f, 0.0f);
		newBullet->bullet->SetWorld();
		//newBullet->bullet->renderingComponent.mat.surfaceReflectance = 0.7f;

		bulletPool.push_back(newBullet);
	}
}

void Tyrian2000::LoadBackgroundTilePool(std::string textureName)
{
	unsigned int numOfTiles = 4;
	float tileDist = 12.2222221f * tileSize;

	for (unsigned int i = 0; i < numOfTiles; i++)
	{
		BackGroundTiles* newTile = new BackGroundTiles(engine->CreateGameObject("Plane"), { 0,0,-1.0f * 5.0f });
		newTile->tile->transform.Scale(tileSize + 2, tileSize + 1, tileSize + 1);
		newTile->tile->transform.Translate(0.0f, moveDownHeight, (i * tileDist));
		newTile->tile->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture(textureName));
		newTile->tile->renderingComponent.mat.uvXOffSet = 5.0f;
		newTile->tile->renderingComponent.mat.uvYOffSet = 5.0f;

		backgroundTilePool.push_back(newTile);
	}
}

void Tyrian2000::Shoot()
{
	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		if (!bulletPool[i]->bullet->isActive)
		{
			bulletPool[i]->Activate(p1->FindDistAway({ 0.0f, 0.0f, 1.0f }, 2.0f), {0.0f, 0.0f, 80.0f}, Bullet::Regular);
			p1->canAttack = false;
			break;
		}
	}
}
void Tyrian2000::CalculateCamPos()
{
	cam->transform.position = {p1->player->transform.position.x / 10.0f, cam->transform.position.y, p1->player->transform.position.z / 10.0f};
}

void Tyrian2000::KillEnemy(int pos)
{
	Enemy* deleteEnemy = enemyPool[pos];
	engine->DestroyGameObject(deleteEnemy->enemyObj);
	enemyPool.erase(enemyPool.begin() + pos);
	delete deleteEnemy;
}

void Tyrian2000::SetUpActions()
{
	inputManager->AddActionBinding(Actions::ButtonUp, { 87, CosmicInput::ControllerButton::DPAD_UP });
	inputManager->AddActionBinding(Actions::ButtonDown, { 83, CosmicInput::ControllerButton::DPAD_DOWN });
	inputManager->AddActionBinding(Actions::ButtonLeft, { 65, CosmicInput::ControllerButton::DPAD_LEFT });
	inputManager->AddActionBinding(Actions::ButtonRight, { 68, CosmicInput::ControllerButton::DPAD_RIGHT });
	inputManager->AddActionBinding(Actions::Fire, { VK_RETURN,  CosmicInput::ControllerButton::BUTTON_R2 });
	inputManager->AddActionBinding(Actions::Strafe, { VK_SPACE, CosmicInput::ControllerButton::BUTTON_CROSS });
	inputManager->AddActionBinding(Actions::Start, { 99, CosmicInput::ControllerButton::BUTTON_OPTIONS });
	//inputManager->AddActionBinding("Button4", { 13, CosmicInput::ControllerButton::BUTTON_SQUARE });
}

void Tyrian2000::ChooseEndPanelText()
{
	currentState = GameState::EndLevel;
	endGame = true;

	healthBar->SetVisibility(false);
	healthBarFade->SetVisibility(false);
	healthBarBack->SetVisibility(false);
	healthBarBorder->SetVisibility(false);


	if(p1->isDead)
	{
		endGamePanel->LoadTexture(engine->rend->assets->GetSurfaceTexture("failed2"));
		endRetryButton->SetActive(true);
		endRetryButton->SetVisibility(true);
	}
	else 
	{
		endGamePanel->LoadTexture(engine->rend->assets->GetSurfaceTexture("complete1"));
		endContinueButton->SetActive(true);
		endContinueButton->SetVisibility(true); 
	}
	endGamePanel->SetVisibility(true);
}

void Tyrian2000::TogglePauseMenu()
{
	if(currentState == GameState::Paused)
	{
		pausedLogo->SetVisibility(true);

		resumeButton->SetActive(true);
		resumeButton->SetVisibility(true);

		retryButton->SetActive(true);
		retryButton->SetVisibility(true);

		quitButton->SetActive(true);
		quitButton->SetVisibility(true);
	}
	else
	{
		pausedLogo->SetVisibility(false);

		resumeButton->SetActive(false);
		resumeButton->SetVisibility(false);

		retryButton->SetActive(false);
		retryButton->SetVisibility(false);

		quitButton->SetActive(false);
		quitButton->SetVisibility(false);
	}
}

void Tyrian2000::ResetLevel()
{
	float tileDist = 12.2222221f * tileSize;

	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		bulletPool[i]->bullet->transform.position = {0.0f, 10.0f, 0.0f};
		bulletPool[i]->bullet->SetWorld();
	}
	for (unsigned int i = 0; i < enemyPool.size(); i++)
	{
		engine->DestroyGameObject(enemyPool[i]->enemyObj);
		if (enemyPool[i] != nullptr) { delete enemyPool[i]; enemyPool[i] = nullptr; }
	}
	enemyPool.clear();
	for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
	{
		backgroundTilePool[i]->tile->transform.position = { 0.0f, moveDownHeight, (i * tileDist) };
	}

	p1->player->transform.position = { 0.0f, 4.0f + moveDownHeight, -4.0f };
	p1->health = p1->maxHealth;
	p1->topDisplayHealth = p1->maxHealth;
	p1->botDisplayHealth = p1->maxHealth;
	p1->player->SetVisibility(true);
	p1->isDead = false;
	
	fLine->finishLine->SetActive(false);
	fLine->finishLine->transform.position = { 0.0f, p1->player->transform.position.y, 18.0f };
	fLine->finishLine->SetWorld();

	endGame = false;
	currentWave = 0;
	
	pauseOption = 0;

	waveSpawnTimer = waveSpawnCD;

	healthBar->SetVisibility(true);
	healthBarFade->SetVisibility(true);
	healthBarBack->SetVisibility(true);
	healthBarBorder->SetVisibility(true);

	SpawnWaveEnemies();
	SpawnWaveBlockers();
}
