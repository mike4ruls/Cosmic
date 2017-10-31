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

	originalSurColor = enemyObj->renderingComponent.mat.surfaceColor;
}

Enemy::~Enemy()
{
	if (enemyObj != nullptr) { delete enemyObj; enemyObj = nullptr; }
}

void Enemy::Update(float dt)
{
	enemyObj->transform.Translate(0.0f, 0.0f, speed * dt);
	if(canFlash)
	{
		UpdateFlashRed(dt);
	}
	enemyObj->Update(dt);
}

void Enemy::FlashRed()
{
	enemyObj->renderingComponent.mat.surfaceColor = {1.0f,0.0f,0.0f,1.0f};
	canFlash = true;
}
void Enemy::UpdateFlashRed(float dt)
{
	if (flashTimer <= 0.0f)
	{
		canFlash = false;
		enemyObj->renderingComponent.mat.surfaceColor = originalSurColor;
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
