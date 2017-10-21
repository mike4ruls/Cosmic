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
}

void Tyrian2000::Init()
{
	engine->lockCamera = true;
	cam->transform.Rotate(0.0f, 89.5f, 0.0f);
	cam->transform.Translate(0.0f, 5.0f, 0.0f);

	moveDownHeight = -30;

	xConstraint = 22.0f;
	posZConstraint = 8.0f;
	negZConstraint = 15.5f;

	gameObjects.push_back(engine->CreateGameObject("Plane"));
	CreatePlayer();
	LoadBulletPool();

	gameObjects[0]->transform.Scale(4.5f);
	gameObjects[0]->transform.Translate(0.0f, moveDownHeight, 0.0f);
	gameObjects[0]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("brick"));
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
	CheckInputs();
}

void Tyrian2000::CheckInputs()
{
	if (engine->IsKeyDown(65) && p1->player->transform.position.x >= -xConstraint) //A
	{
		p1->player->transform.Translate(p1->player->transform.right.x * -p1->speed, 0.0f, p1->player->transform.right.z * -p1->speed);
	}
	if (engine->IsKeyDown(68) && p1->player->transform.position.x <= xConstraint) //D
	{
		p1->player->transform.Translate(p1->player->transform.right.x * p1->speed, 0.0f, p1->player->transform.right.z * p1->speed);
	}
	if (engine->IsKeyDown(87) && p1->player->transform.position.z <= posZConstraint) //W
	{
		p1->player->transform.Translate(p1->player->transform.foward.x * p1->speed, 0.0f, p1->player->transform.foward.z * p1->speed);
	}
	if (engine->IsKeyDown(83) && p1->player->transform.position.z >= -negZConstraint) //S
	{
		p1->player->transform.Translate(p1->player->transform.foward.x * -p1->speed, 0.0f, p1->player->transform.foward.z * -p1->speed);
	}
	if (engine->IsKeyDown(VK_RETURN) && p1->canAttack)
	{
		Shoot();
	}
}

void Tyrian2000::CreatePlayer()
{
	p1 = new Player(engine->CreateGameObject("Cube"), 0.1f, 0.1f);
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
