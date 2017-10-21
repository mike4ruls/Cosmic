#pragma once
#include "GameEntity.h"

class Player
{
public:
	Player();
	Player(GameEntity* obj, float spd, float atkSpd);
	~Player();

	float speed;
	float atkSpeed;
	float timer;
	bool canAttack;

	GameEntity* player = nullptr;

	void Update(float dt);
	DirectX::XMFLOAT3 FindDistAway(DirectX::XMFLOAT3 dir, float dist);
};

