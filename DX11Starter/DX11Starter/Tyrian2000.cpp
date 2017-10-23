#include "Tyrian2000.h"



Tyrian2000::Tyrian2000(CosmicEngine* eng)
{
	engine = eng;
	cam = new Camera();
}


Tyrian2000::~Tyrian2000()
{
	if (p1 != nullptr) { delete p1; p1 = nullptr; }

	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		if (bulletPool[i] != nullptr) { delete bulletPool[i]; bulletPool[i] = nullptr; }
	}

	for (unsigned int i = 0; i < backgroundTilePool.size(); i++)
	{
		if (backgroundTilePool[i] != nullptr) { delete backgroundTilePool[i]; backgroundTilePool[i] = nullptr; }
	}
}

void Tyrian2000::Init()
{
	//engine->lockCamera = true;
	engine->lockSunLight = true;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 5.0f, 0.0f);

	engine->rend->sunLight->ligComponent->lightDir = {0.7f, -0.5f, 0.0f};

	moveDownHeight = -30;

	xConstraint = 22.0f;
	posZConstraint = 8.0f;
	negZConstraint = 15.5f;

	tileSize = 4.5f;
	tileDistOffScreen = -50.0f;

	CreatePlayer();
	LoadBulletPool();
	LoadBackgroundTilePool("grass");
}

void Tyrian2000::Update(float deltaTime, float totalTime)
{
	p1->Update(deltaTime);

	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(deltaTime);
	}
	for (unsigned int i = 0; i < bulletPool.size(); i++)
	{
		bulletPool[i]->Update(deltaTime);
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
	CheckInputs(deltaTime);
}

void Tyrian2000::CheckInputs(float dt)
{
	bool butCheckX = false;
	bool butCheckY = false;
	if (engine->IsKeyDown(65)) //A
	{
		butCheckX = true;
		p1->currentTurnState = Player::LEFT;
		p1->previousTurnState = Player::LEFT;
		p1->player->transform.Translate((p1->player->transform.right.x * -p1->speed) * dt, 0.0f, (p1->player->transform.right.z * -p1->speed) * dt);
	}
	if (engine->IsKeyDown(68)) //D
	{
		butCheckX = true;
		p1->currentTurnState = Player::RIGHT;
		p1->previousTurnState = Player::RIGHT;
		p1->player->transform.Translate((p1->player->transform.right.x * p1->speed) * dt, 0.0f, (p1->player->transform.right.z * p1->speed) * dt);
	}
	if (engine->IsKeyDown(87)) //W
	{
		butCheckY = true;
		p1->player->transform.Translate((p1->player->transform.foward.x * p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * p1->speed) * dt);
	}
	if (engine->IsKeyDown(83)) //S
	{
		butCheckY = true;
		p1->player->transform.Translate((p1->player->transform.foward.x * -p1->speed) * dt, 0.0f, (p1->player->transform.foward.z * -p1->speed) * dt);
	}
	if (!butCheckX)
	{
		p1->currentTurnState = Player::STRAIGHT;
	}
	if (engine->IsKeyDown(VK_RETURN) && p1->canAttack)
	{
		Shoot();
	}
	if (engine->IsKeyPressed(VK_SPACE) && !p1->canStrafe)
	{
		//p1->rotLeft = rotLeft;
		p1->TurnOnStrafe();
		if(p1->currentTurnState == Player::STRAIGHT && butCheckY)
		{
			p1->speed = p1->normSpeed;
		}
		else if(p1->previousTurnState == Player::LEFT)
		{
			p1->player->rigidBody.ApplyForce(-p1->strafeForce, 0.0f, 0.0f);
		}
		else if (p1->previousTurnState == Player::RIGHT)
		{
			p1->player->rigidBody.ApplyForce(p1->strafeForce, 0.0f, 0.0f);
		}
	}
	if (engine->IsKeyDown(96))
	{
		DefaultScene* defaultScene = new DefaultScene(engine);
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

void Tyrian2000::CreatePlayer()
{
	p1 = new Player(engine->CreateGameObject("FighterShip"), 0.1f);
	p1->player->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("fighterShipSur"));
	p1->player->transform.Scale(0.005f);
	p1->player->transform.Translate(0.0f, 4.0f + moveDownHeight, 0.0f);
}

void Tyrian2000::LoadBulletPool()
{
	unsigned int numOfBullets = 40;

	for (unsigned int i = 0; i < numOfBullets; i++) 
	{
		Bullet* newBullet = new Bullet(engine->CreateGameObject("Sphere"));
		newBullet->bullet->transform.Translate(0.0f, 10.0f, 0.0f);

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
		newTile->tile->transform.Scale(tileSize + 1);
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
		if (!bulletPool[i]->isActive)
		{
			bulletPool[i]->Activate(p1->FindDistAway({ 0.0f, 0.0f, 1.0f }, 2.0f), {0.0f, 0.0f, 80.0f}, Bullet::Regular);
			p1->canAttack = false;
			break;
		}
	}
}
