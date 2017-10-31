#include "Player.h"



Player::Player()
{
}

Player::Player(GameEntity* obj, float hlt, float atkSpd, float atkdmg)
{
	player = obj;
	currentTilt = LEFT;
	previousTurnState = LEFT;

	health = hlt;

	normSpeed = 20.0f;
	strafeSpeed = 0.0f;
	speed = normSpeed;

	strafeForce = 10000.0f;
	strafeRotSpeed = 1060.0f;
	currentRot = 0.0f;

	tiltDegrees = 30.0f;
	tiltSpeed = 200.0f;
	currentTilt = 0.0f;

	originalRot = obj->transform.rotation;

	canAttack = true;
	canStrafe = false;
	canTilt = true;
	isDead = false;
	atkSpeed = atkSpd;
	atkDamage = atkdmg;
	timer = atkSpeed;
	player->renderingComponent.mat.surfaceColor = { 1.0f,0.0f,0.0f,1.0f };
	player->rigidBody.applyFriction = true;
	player->rigidBody.mass = 0.2f;
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
	if (canStrafe)
	{
		Strafe(dt);
	}
	if (canTilt) {
		Tilt(dt);
	}

	player->Update(dt);
}

void Player::Strafe(float dt)
{
	if (staticCurTurnState == LEFT)
	{
		currentRot += strafeRotSpeed * dt;
	}
	else if(staticCurTurnState == RIGHT)
	{
		currentRot -= strafeRotSpeed * dt;
	}
	player->transform.SetRotatation(0.0f, 0.0f, currentRot);

	if (currentRot >= 360.0f || currentRot <= -360.0f)
	{
		canStrafe = false;
		canTilt = true;
		player->transform.SetRotatation(originalRot);
		player->rigidBody.velocity = {0.0f, 0.0f, 0.0f};
		player->rigidBody.acceleration = { 0.0f, 0.0f, 0.0f };
		currentRot = 0.0f;
		speed = normSpeed;
	}
}

void Player::TurnOnStrafe()
{
	canStrafe = true;
	canTilt = false;
	speed = strafeSpeed;
	staticCurTurnState = previousTurnState;
}

void Player::Tilt(float dt)
{
	if(currentTurnState == LEFT)
	{
		if (currentTilt < tiltDegrees) {
			currentTilt += tiltSpeed * dt;
		}
		player->transform.SetRotatation(0.0f, 0.0f, currentTilt);
	}
	else if (currentTurnState == RIGHT)
	{
		if (currentTilt > -tiltDegrees) {
			currentTilt -= tiltSpeed * dt;
		}
		player->transform.SetRotatation(0.0f, 0.0f, currentTilt);
	}
	else {
		if (previousTurnState == LEFT) {
			if(currentTilt > 0.0f)
			{
				currentTilt -= tiltSpeed * dt;
			}
			else {
				currentTilt = 0.0f;
			}
		}
		else if (previousTurnState == RIGHT) {
			if (currentTilt < 0.0f)
			{
				currentTilt += tiltSpeed * dt;
			}
			else {
				currentTilt = 0.0f;
			}
		}
		player->transform.SetRotatation(0.0f, 0.0f, currentTilt);
	}
}

void Player::TakeDamage(float dmg)
{
	health -= dmg;
	if (health <= 0.0f)
	{
		isDead = true;
	}
}

DirectX::XMFLOAT3 Player::FindDistAway(DirectX::XMFLOAT3 dir, float dist)
{
	DirectX::XMFLOAT3 pos = { player->transform.position.x + (dir.x * dist), player->transform.position.y + (dir.y * dist), player->transform.position.z + (dir.z * dist) };
	return pos;
}
