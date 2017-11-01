#pragma once
#include "GameEntity.h"

#define DEACTIVATE_CD 1.0f

class Bullet
{


public:
	enum BulletType {
		Regular,
		Rocket,
		Grenade,
		CottonBall
	}bulletType;

	Bullet();
	Bullet(GameEntity* obj);
	~Bullet();

	DirectX::XMFLOAT3 bulletVelocity;

	float timer;

	GameEntity* bullet;

	void Update(float dt);
	void Activate(DirectX::XMFLOAT3 bulletPos, DirectX::XMFLOAT3 bulletVelo, BulletType type);

	void DeactivateTimer(float dt);
	void Deactivate();

};

