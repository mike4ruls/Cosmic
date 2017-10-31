#pragma once
#include "GameEntity.h"

class Enemy
{
public:
	Enemy();
	Enemy(GameEntity* obj, float hlth, float spd, float dmg);
	~Enemy();

	GameEntity* enemyObj;

	DirectX::XMFLOAT4 originalSurColor;

	float health;
	float speed;
	float damage;

	float flashCD;
	float flashTimer;

	bool isDead;
	bool canFlash;

	void Update(float dt);
	void FlashRed();
	void UpdateFlashRed(float dt);
	void TakeDamage(float dmg);
};

