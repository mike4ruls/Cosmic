#include "Enemy.h"



Enemy::Enemy()
{
}

Enemy::Enemy(GameEntity * obj, float hlth, float spd, float dmg)
{
	enemyObj = obj;
	health = hlth;
	speed = spd;
	damage = dmg;
	isDead = false;

	flashCD = 0.04f;
	flashTimer = flashCD;

	type = EnemyType::Random;

	originalSurColor = enemyObj->renderingComponent->mat.surfaceColor;
}

Enemy::Enemy(GameEntity * obj, float hlth, float spd, float dmg, Enemy::EnemyType ty)
{
	enemyObj = obj;
	health = hlth;
	speed = spd;
	damage = dmg;
	isDead = false;

	zigTurn = true;
	zigDist = 1.0f;
	currentZig = 0.0f;

	flashCD = 0.04f;
	flashTimer = flashCD;

	type = ty;

	originalSurColor = enemyObj->renderingComponent->mat.surfaceColor;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
	switch(type)
	{
	case EnemyType::Regular:
		enemyObj->transform.Translate(0.0f, 0.0f, speed * dt);
		break;
	case EnemyType::ZigZag:
		if(zigTurn)
		{
			currentZig += speed * dt;
		}
		else
		{
			currentZig -= speed * dt;
		}

		if(currentZig >= zigDist || currentZig <= -zigDist)
		{
			currentZig = 0.0f;
			zigTurn = zigTurn ? false : true;
		}
		enemyObj->transform.Translate(currentZig, 0.0f, speed * dt);
		break;
	case EnemyType::Seeking:
		break;
	case EnemyType::Random:
		break;
	}
	enemyObj->transform.Translate(0.0f, 0.0f, speed * dt);
	if(canFlash)
	{
		UpdateFlashRed(dt);
	}
}

void Enemy::FlashRed()
{
	enemyObj->renderingComponent->mat.surfaceColor = {1.0f,0.0f,0.0f,1.0f};
	canFlash = true;
}
void Enemy::UpdateFlashRed(float dt)
{
	if (flashTimer <= 0.0f)
	{
		canFlash = false;
		enemyObj->renderingComponent->mat.surfaceColor = originalSurColor;
		flashTimer = flashCD;
		return;
	}
	flashTimer -= dt;
}

void Enemy::TakeDamage(float dmg)
{
	health -= dmg;
	if (health <= 0.0f)
	{
		isDead = true;
	}
	FlashRed();
}

void Enemy::Destroy()
{
	enemyObj->Destroy();
	delete this;
}
