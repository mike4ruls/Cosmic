#include "Tyrian2000.h"



Tyrian2000::Tyrian2000()
{
	cam = new FreeCamera();
}

Tyrian2000::Tyrian2000(char * tile)
{
	cam = new FreeCamera();
	tileName = tile;
}


Tyrian2000::~Tyrian2000()
{
	gameManager->SavePlayer(p1);
	if (p1 != nullptr) { delete p1; p1 = nullptr; }

	if (fLine != nullptr) { delete fLine; fLine = nullptr; }
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
	inputManager = InputManager::GetInstance();
	cam->inputManager = inputManager;
	gameManager = TyrianGameManager::GetInstance();
	SetUpActions();
	// ========== ====================//

	cam->lockCameraRot = true;
	cam->lockCameraPos = true;
	//engine->rend->skyBoxOn = false; <-------------------------------------------------
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 5.0f, 0.0f);

	sunLight->ligComponent->lightDir = { -0.7f, -0.5f, 0.0f };

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
	InitUI();
	CreateFinishLine();
	SpawnWaveEnemies();
	SpawnWaveBlockers();

	if (tileName == nullptr) {
		float choice = ((float)rand() / RAND_MAX) * 100;

		if (choice > 90.0f) {
			LoadBackgroundTilePool("grass");
		}
		else if (choice > 80.0f) {
			LoadBackgroundTilePool("water");
		}
		else if (choice > 70.0f) {
			LoadBackgroundTilePool("brick");
		}
		else if (choice > 60.0f) {
			LoadBackgroundTilePool("checker");
		}
		else if (choice > 50.0f) {
			LoadBackgroundTilePool("dry");
		}
		else if (choice > 40.0f) {
			LoadBackgroundTilePool("rainbow");
		}
		else if (choice > 30.0f) {
			LoadBackgroundTilePool("lava");
		}
		else if (choice > 20.0f) {
			LoadBackgroundTilePool("sand");
		}
		else if (choice > 10.0f) {
			LoadBackgroundTilePool("lavaGround");
		}
		else {
			LoadBackgroundTilePool("star");
		}
	}
	else
	{
		LoadBackgroundTilePool(tileName);

		startButton->SetVisibility(false);
		startButton->SetActive(false);

		Tyrian2000Logo->SetVisibility(false);

		p1->healthBar->SetVisibility(true);
		p1->healthBarFade->SetVisibility(true);
		p1->healthBarBack->SetVisibility(true);
		p1->healthBarBorder->SetVisibility(true);

		if(p1->sheildComponentBought)
		{ 
			p1->sheildBar->SetVisibility(true);
			p1->sheildBarFade->SetVisibility(true);
			p1->sheildBarBack->SetVisibility(true);
			p1->sheildBarBorder->SetVisibility(true);
		}

		currentState = GameState::Game;
	}
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


			gameManager->LoadHubWorld();
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
			shipExhaust->isLooping = false;
			leftWing->isLooping = false;
			rightWing->isLooping = false;

			ChooseEndPanelText();
		}
		fLine->Update(deltaTime);
		for (unsigned int i = 0; i < p1->frontBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->frontBulletPool[i]->bullet->isActive)
			{
				p1->frontBulletPool[i]->Update(deltaTime);

				for (unsigned int j = 0; j < enemyPool.size(); j++)
				{
					if (physicEngine->SphereVSphereCollision(p1->frontBulletPool[i]->bullet, enemyPool[j]->enemyObj))
					{
						enemyPool[j]->TakeDamage(p1->frontAtkDamage);
						p1->frontBulletPool[i]->Deactivate();
						p1->frontBulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
						p1->frontBulletPool[i]->bullet->SetWorld();
					}
				}
			}
		}
		for (unsigned int i = 0; i < p1->leftBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->leftBulletPool[i]->bullet->isActive)
			{
				p1->leftBulletPool[i]->Update(deltaTime);

				for (unsigned int j = 0; j < enemyPool.size(); j++)
				{
					if (physicEngine->SphereVSphereCollision(p1->leftBulletPool[i]->bullet, enemyPool[j]->enemyObj))
					{
						enemyPool[j]->TakeDamage(p1->leftAtkDamage);
						p1->leftBulletPool[i]->Deactivate();
						p1->leftBulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
						p1->leftBulletPool[i]->bullet->SetWorld();
					}
				}
			}
		}
		for (unsigned int i = 0; i < p1->rightBulletPool.size(); i++) // SUPER TIME CONSUMING < ---- cost tons of frames
		{
			if (p1->rightBulletPool[i]->bullet->isActive)
			{
				p1->rightBulletPool[i]->Update(deltaTime);

				for (unsigned int j = 0; j < enemyPool.size(); j++)
				{
					if (physicEngine->SphereVSphereCollision(p1->rightBulletPool[i]->bullet, enemyPool[j]->enemyObj))
					{
						enemyPool[j]->TakeDamage(p1->rightAtkDamage);
						p1->rightBulletPool[i]->Deactivate();
						p1->rightBulletPool[i]->bullet->transform.Translate(0.0f, 100.0f, 0.0f);
						p1->rightBulletPool[i]->bullet->SetWorld();
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
			if (physicEngine->SphereVSphereCollision(p1->player, enemyPool[i]->enemyObj))
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
			gameManager->LoadWorld();
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
				gameManager->CompleteLevels();
			}
		}
		break;
	}
	if (inputManager->IsKeyDown(96))
	{
		TyrianGameManager::Release();

		SceneManager::LoadScene(SceneManager::_DefaultScene);
		//engine->rend->skyBoxOn = true; <----------------------------------
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
		if (inputManager->IsActionDown(Actions::Fire))
		{
			p1->ShootBullets();
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
		if (inputManager->IsActionDown(Actions::Fire))
		{
			p1->ShootBullets();
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

	startButton = new Button();
	startButton->LoadTexture(assetManager->GetSurfaceTexture("startButtontext"));
	startButton->SetWidth(0.3f);
	startButton->SetHeight(0.1f);
	startButton->posY = -0.7f;
	//startButton->SetVisibility(false);

	resumeButton = new Button();
	resumeButton->LoadTexture(assetManager->GetSurfaceTexture("resumeButtontext"));
	resumeButton->SetWidth(0.5f);
	resumeButton->SetHeight(0.1f);
	resumeButton->posY = -pauseButtondist;
	resumeButton->SetVisibility(false);
	resumeButton->SetActive(false);

	retryButton = new Button();
	retryButton->LoadTexture(assetManager->GetSurfaceTexture("retryButtontext"));
	retryButton->SetWidth(0.5f);
	retryButton->SetHeight(0.1f);
	retryButton->posY = -0.4f - pauseButtondist;
	retryButton->SetVisibility(false);
	retryButton->SetActive(false);

	quitButton = new Button();
	quitButton->LoadTexture(assetManager->GetSurfaceTexture("quitButtontext"));
	quitButton->SetWidth(0.5f);
	quitButton->SetHeight(0.1f);
	quitButton->posY = -0.8f - pauseButtondist;
	quitButton->SetVisibility(false);
	quitButton->SetActive(false);

	endRetryButton = new Button();
	endRetryButton->LoadTexture(assetManager->GetSurfaceTexture("retryButtontext"));
	endRetryButton->SetWidth(0.5f);
	endRetryButton->SetHeight(0.1f);
	endRetryButton->posY = -endButtondist;
	endRetryButton->SetVisibility(false);
	endRetryButton->SetActive(false);

	endContinueButton = new Button();
	endContinueButton->LoadTexture(assetManager->GetSurfaceTexture("continueButtontext"));
	endContinueButton->SetWidth(0.5f);
	endContinueButton->SetHeight(0.1f);
	endContinueButton->posY = -endButtondist;
	endContinueButton->SetVisibility(false);
	endContinueButton->SetActive(false);

	endGamePanel = new Image();
	endGamePanel->SetWidth(0.8f);
	endGamePanel->SetHeight(0.8f);

	endGamePanel->SetVisibility(false);

	Tyrian2000Logo = new Image();
	Tyrian2000Logo->LoadTexture(assetManager->GetSurfaceTexture("tyrian2000Logo"));
	Tyrian2000Logo->SetHeight(0.5f);
	Tyrian2000Logo->SetHeight(0.3f);
	Tyrian2000Logo->posY = 0.6f;
	//Tyrian2000Logo->SetVisibility(false);

	pausedLogo = new Image();
	pausedLogo->LoadTexture(assetManager->GetSurfaceTexture("pausedLogo"));
	pausedLogo->SetHeight(0.5f);
	pausedLogo->SetHeight(0.3f);
	pausedLogo->posY = 0.6f;
	pausedLogo->SetVisibility(false);
}

void Tyrian2000::CreatePlayer()
{
	p1 = new Player(new GameEntity(assetManager->GetMesh("FighterShip"),false), assetManager->GetMesh("Sphere"),100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent->mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, -4.0f);

	gameManager->LoadPlayer(p1);

	SetUpParticles();
}

void Tyrian2000::SetUpParticles()
{
	exhaustPos = {0.0f, 0.3f, -1.3f};
	leftWingPos = {-0.7f, 0.3f, -1.3f};
	rightWingPos = { 0.7f, 0.3f, -1.3f};

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
	leftWing->emitterAcceleration = 622.0f;
	leftWing->startSize = 0.2f;
	leftWing->endSize = 0.2f;
	leftWing->startRadius = 0.0f;
	leftWing->endRadius = 0.0f;
	leftWing->emissionRate = 0.002f;
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


	for(int i = 0; i < 20; i++)
	{
		explosionPool.push_back(Emitter::CreateExplosionEmitter(assetManager->GetSurfaceTexture("blackFire")));
		explosionPool[i]->isActive = false;
		explosionPool[i]->startSize = 1.0f;
		explosionPool[i]->endSize = 8.0f;
	}

	for (int i = 0; i < 20; i++)
	{
		explosionStarPool.push_back(Emitter::CreateExplosionEmitter(assetManager->GetSurfaceTexture("orangeFire")));
		explosionStarPool[i]->isActive = false;
		explosionStarPool[i]->startSize = 1.0f;
		explosionStarPool[i]->endSize = 6.0f;

		/*explosionStarPool[i]->isActive = false;
		explosionStarPool[i]->endColor = {1.0f, 1.0f, 1.0f, 0.5f};
		explosionStarPool[i]->startSize = 0.1f;
		explosionStarPool[i]->endSize = 2.0f;
		explosionStarPool[i]->lifeTime = 1.0f;*/
	}
}

void Tyrian2000::CreateFinishLine()
{
	fLine = new FinishLine(new GameEntity(assetManager->GetMesh("Cube"), false), 2.0f);
	fLine->finishLine->transform.Translate(0.0f, p1->player->transform.position.y, 18.0f); //18
	fLine->finishLine->transform.Scale(43.0f, 1.0f, 1.0f);
	fLine->finishLine->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture("checker"));
	fLine->finishLine->renderingComponent->mat.uvXOffSet = 0.25f;
	fLine->finishLine->renderingComponent->mat.uvYOffSet = 10.0f;
	fLine->finishLine->SetWorld();
}

void Tyrian2000::SpawnWaveEnemies()
{
	int numOfEnemies = 10;
	for(int i = 0; i < numOfEnemies; i++)
	{
		Enemy* newEnemy = new Enemy(new GameEntity(assetManager->GetMesh("Cone"), false), 2.5f, -3.0f, 1.0f, Enemy::EnemyType::Regular);
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
		Enemy* newEnemy = new Enemy(new GameEntity(assetManager->GetMesh("Sphere"), false), 15.0f, -2.0f, 3.0f, Enemy::EnemyType::ZigZag);
		newEnemy->enemyObj->transform.Translate(-20.0f + (i * 3), p1->player->transform.position.y, 22.0f);
		newEnemy->enemyObj->transform.Scale(2.0f);
		enemyPool.push_back(newEnemy);
	}
}

void Tyrian2000::LoadBackgroundTilePool(std::string textureName)
{
	unsigned int numOfTiles = 4;
	float tileDist = 12.2222221f * tileSize;

	for (unsigned int i = 0; i < numOfTiles; i++)
	{
		BackGroundTiles* newTile = new BackGroundTiles(new GameEntity(assetManager->GetMesh("BackGroundTile"), false), { 0,0,-1.0f * 5.0f });
		newTile->tile->transform.Scale(tileSize + 2, tileSize + 1, tileSize + 1);
		newTile->tile->transform.Translate(0.0f, moveDownHeight, (i * tileDist));
		newTile->tile->renderingComponent->mat.LoadSurfaceTexture(assetManager->GetSurfaceTexture(textureName));
		newTile->tile->renderingComponent->mat.uvXOffSet = 5.0f;
		newTile->tile->renderingComponent->mat.uvYOffSet = 5.0f;

		backgroundTilePool.push_back(newTile);
	}
}
void Tyrian2000::CalculateCamPos()
{
	cam->transform.position = {p1->player->transform.position.x / 10.0f, cam->transform.position.y, p1->player->transform.position.z / 10.0f};
}

void Tyrian2000::KillEnemy(int pos)
{
	Enemy* deleteEnemy = enemyPool[pos];

	if(deleteEnemy->isDead)
	{
		for (unsigned int i = 0; i < explosionPool.size(); i++)
		{
			if (!explosionPool[i]->isActive)
			{
				explosionPool[i]->transform.position = deleteEnemy->enemyObj->transform.position;
				explosionPool[i]->Reset();
				break;
			}
		}
		for (unsigned int i = 0; i < explosionStarPool.size(); i++)
		{
			if (!explosionStarPool[i]->isActive)
			{
				explosionStarPool[i]->transform.position = deleteEnemy->enemyObj->transform.position;
				explosionStarPool[i]->Reset();
				break;
			}
		}
	}
	if(deleteEnemy->isDead)
	{
		cam->ShakeCamera(0.15f, 0.1f);
	}
	enemyPool.erase(enemyPool.begin() + pos);
	deleteEnemy->Destroy();
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

	p1->healthBar->SetVisibility(false);
	p1->healthBarFade->SetVisibility(false);
	p1->healthBarBack->SetVisibility(false);
	p1->healthBarBorder->SetVisibility(false);


	if(p1->isDead)
	{
		endGamePanel->LoadTexture(assetManager->GetSurfaceTexture("failed2"));
		endRetryButton->SetActive(true);
		endRetryButton->SetVisibility(true);
	}
	else 
	{
		endGamePanel->LoadTexture(assetManager->GetSurfaceTexture("complete1"));
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

	for (unsigned int i = 0; i < p1->frontBulletPool.size(); i++)
	{
		p1->frontBulletPool[i]->bullet->transform.position = {0.0f, 10.0f, 0.0f};
		p1->frontBulletPool[i]->bullet->SetWorld();
	}
	for (unsigned int i = 0; i < enemyPool.size(); i++)
	{
		enemyPool[i]->Destroy();
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

	shipExhaust->Reset();
	shipExhaust->isLooping = true;
	leftWing->Reset();
	leftWing->isLooping = true;
	rightWing->Reset();
	rightWing->isLooping = true;
	
	fLine->finishLine->SetActive(false);
	fLine->finishLine->transform.position = { 0.0f, p1->player->transform.position.y, 18.0f };
	fLine->finishLine->SetWorld();

	endGame = false;
	currentWave = 0;
	
	pauseOption = 0;

	waveSpawnTimer = waveSpawnCD;

	p1->healthBar->SetVisibility(true);
	p1->healthBarFade->SetVisibility(true);
	p1->healthBarBack->SetVisibility(true);
	p1->healthBarBorder->SetVisibility(true);

	SpawnWaveEnemies();
	SpawnWaveBlockers();
}

void Tyrian2000::Quit()
{
	TyrianGameManager::Release();
}
