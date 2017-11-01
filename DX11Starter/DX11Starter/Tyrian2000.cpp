#include "Tyrian2000.h"



Tyrian2000::Tyrian2000(CosmicEngine* eng)
{
	engine = eng;
	cam = new Camera();
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
	// ========== ====================//

	engine->lockCamera = true;
	cam->lockCameraPos = true;
	engine->lockSunLight = true;
	engine->rend->skyBoxOn = false;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 5.0f, 0.0f);

	engine->rend->sunLight->ligComponent->lightDir = {0.7f, -0.5f, 0.0f};

	endGame = false;
	waveCount = 3;
	currentWave = 0;

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
	InitUI();
	SpawnWaveEnemies();
	SpawnWaveBlockers();
	LoadBulletPool();
	LoadBackgroundTilePool("grass");
}

void Tyrian2000::Update(float deltaTime, float totalTime)
{
	p1->Update(deltaTime);

	healthBar->SetWidth(healthBarBack->GetWidth() * (p1->topDisplayHealth / p1->maxHealth));
	healthBarFade->SetWidth(healthBarBack->GetWidth() * (p1->botDisplayHealth / p1->maxHealth));

	if(waveSpawnTimer <= 0.0f)
	{
		if (!fLine->finishLine->isActive)
		{
			SpawnWaveEnemies();
			SpawnWaveBlockers();
			currentWave++;
		}

		waveSpawnTimer = waveSpawnCD;
	}

	if(currentWave == waveCount)
	{
		fLine->finishLine->SetActive(true);
		currentWave++;
	}
	if(fLine->finishLine->isActive && p1->player->transform.position.z >= fLine->finishLine->transform.position.z)
	{
		fLine->finishLine->SetActive(false);
		ChooseEndPanelText();
	}
	if(p1->isDead && !endGame)
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
		if (enemyPool[i]->isDead || enemyPool[i]->enemyObj->transform.position.z <= -negZConstraint -2) {
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
			backgroundTilePool[i]->Teleport({ 0.0f, 0.0f, (12.2222221f * tileSize) *  backgroundTilePool.size()});
		}
		backgroundTilePool[i]->Update(deltaTime);
	}

	CheckOutOfBounds();
	CalculateCamPos();

	if(inputManager->IsControllerConnected())
	{
		CheckControllerInputs(deltaTime);
	}
	else 
	{
		CheckInputs(deltaTime);
	}
	waveSpawnTimer -= deltaTime;
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
	if (inputManager->IsActionPressed(Actions::Restart))
	{
		Tyrian2000* tyrian = new Tyrian2000(engine);
		engine->LoadScene(tyrian);
	}
	if (inputManager->IsKeyDown(96))
	{
		DefaultScene* defaultScene = new DefaultScene(engine);
		engine->rend->skyBoxOn = true;
		engine->LoadScene(defaultScene);
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

		if (inputManager->IsActionDown(Actions::ButtonLeft) || LX < -0.2f) //A
		{
			butCheckX = true;
			p1->currentTurnState = Player::LEFT;
			p1->previousTurnState = Player::LEFT;
			p1->player->transform.Translate((p1->player->transform.right.x * -p1->speed) * dt, 0.0f, (p1->player->transform.right.z * -p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonRight) || LX > 0.2f) //D
		{
			butCheckX = true;
			p1->currentTurnState = Player::RIGHT;
			p1->previousTurnState = Player::RIGHT;
			p1->player->transform.Translate((p1->player->transform.right.x * p1->speed) * dt, 0.0f, (p1->player->transform.right.z * p1->speed) * dt);
		}

		if (inputManager->IsActionDown(Actions::ButtonUp) || LY > 0.2f) //W
		{
			butCheckY = true;
			p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);
		}
		if (inputManager->IsActionDown(Actions::ButtonDown) || LY < -0.2f) //S
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
	if (inputManager->IsActionPressed(Actions::Restart))
	{
		Tyrian2000* tyrian = new Tyrian2000(engine);
		engine->LoadScene(tyrian);
	}
	if (inputManager->IsKeyDown(96))
	{
		DefaultScene* defaultScene = new DefaultScene(engine);
		engine->rend->skyBoxOn = true;
		engine->LoadScene(defaultScene);
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
	healthBar = engine->CreateCanvasElement();
	healthBar->obj->renderingComponent.mat.surfaceColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	healthBar->posY = -1.0f;
	//healthBar->SetAlignment(UI::Alignment::Left);
	healthBar->SetWidth(0.6f);
	healthBar->SetHeight(0.02f);

	healthBarFade = engine->CreateCanvasElement();
	healthBarFade->obj->renderingComponent.mat.surfaceColor = { 0.3f, 0.0f, 0.0f, 1.0f };
	healthBarFade->SetAlignment(healthBar->GetAlignment());
	healthBarFade->posY = healthBar->posY;
	healthBarFade->SetWidth(healthBar->GetWidth());
	healthBarFade->SetHeight(healthBar->GetHeight());

	healthBarBack = engine->CreateCanvasElement();
	healthBarBack->obj->renderingComponent.mat.surfaceColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	healthBarBack->posY = healthBar->posY;
	healthBarBack->SetWidth(healthBar->GetWidth());
	healthBarBack->SetHeight(healthBar->GetHeight());

	healthBarBorder = engine->CreateCanvasElement();
	healthBarBorder->obj->renderingComponent.mat.surfaceColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	healthBarBorder->posY = healthBar->posY;
	healthBarBorder->SetWidth(healthBar->GetWidth() + 0.04f);
	healthBarBorder->SetHeight(healthBar->GetHeight() + 0.04f);

	endGamePanel = engine->CreateCanvasElement();
	endGamePanel->SetWidth(0.8f);
	endGamePanel->SetHeight(0.8f);
	endGamePanel->obj->SetVisibility(false);
}

void Tyrian2000::CreatePlayer()
{
	p1 = new Player(engine->CreateGameObject("FighterShip"), 100.0f, 0.1f, 1.0f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->renderingComponent.mat.surfaceReflectance = 0.3f;
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, 0.0f);
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
		Enemy* newEnemy = new Enemy(engine->CreateGameObject("Cone"), 4.0f, -3.0f, 1.0f);
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
		Enemy* newEnemy = new Enemy(engine->CreateGameObject("Sphere"), 15.0f, -2.0f, 3.0f);
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
	inputManager->AddActionBinding(Actions::Restart, { 99, CosmicInput::ControllerButton::BUTTON_OPTIONS });
	//inputManager->AddActionBinding("Button4", { 13, CosmicInput::ControllerButton::BUTTON_SQUARE });
}

void Tyrian2000::ChooseEndPanelText()
{
	endGame = true;
	if(p1->isDead)
	{
		endGamePanel->obj->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("failed2"));
	}
	else 
	{
		endGamePanel->obj->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("complete1"));
	}
	endGamePanel->obj->SetVisibility(true);
}
