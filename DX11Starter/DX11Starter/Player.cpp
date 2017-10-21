#include "Player.h"



Player::Player()
{
}

Player::Player(GameEntity* obj, float spd, float atkSpd)
{
	player = obj;
	speed = spd;
	canAttack = true;
	atkSpeed = atkSpd;
	timer = atkSpeed;
	player->renderingComponent.mat.surfaceColor = { 1.0f,0.0f,0.0f,1.0f };
}


Player::~Player()
{
	if (player != nullptr) { delete player; player = nullptr; }
}

void Player::Update(float dt)
{
	if (!canAttack)
	{
		timer -= dt;
		if (timer <= 0.0f)
		{
			canAttack = true;
			timer = atkSpeed;
		}
	}
	player->Update(dt);
}

DirectX::XMFLOAT3 Player::FindDistAway(DirectX::XMFLOAT3 dir, float dist)
{
	DirectX::XMFLOAT3 pos = { player->transform.position.x + (dir.x * dist), player->transform.position.y + (dir.y * dist), player->transform.position.z + (dir.z * dist) };
	return pos;
}
