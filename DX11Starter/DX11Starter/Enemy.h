#pragma once
#include "GameEntity.h"

class Enemy
{
public:
	enum EnemyType {
		Regular,
		ZigZag,
		Seeking,
		Random
	};

	Enemy();
	Enemy(GameEntity* obj, float hlth, float spd, float dmg);
	Enemy(GameEntity* obj, float hlth, float spd, float dmg, Enemy::EnemyType ty);
	~Enemy();

	GameEntity* enemyObj;
	EnemyType type;
	DirectX::XMFLOAT4 originalSurColor;

	float health;
	float speed;
	float damage;

	float flashCD;
	float flashTimer;

	float zigDist;
	float currentZig;

	bool isDead;
	bool canFlash;

	bool zigTurn;

	void Update(float dt);
	void FlashRed();
	void UpdateFlashRed(float dt);
	void TakeDamage(float dmg);
	void Destroy();
};

