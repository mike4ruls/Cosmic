#include "Player.h"



Player::Player()
{
}

Player::Player(GameEntity* obj, float hlt, float atkSpd, float atkdmg)
{
	player = obj;
	currentTilt = LEFT;
	previousTurnState = LEFT;

	maxHealth = hlt;
	health = maxHealth;
	topDisplayHealth = maxHealth;
	botDisplayHealth = maxHealth;

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

	strafeImmune = false;
	damageImmune = false;

	canStrafeImmuneTimer = false;
	canDamageImmuneTimer = false;

	strafeImmuneCD = 1.0f;
	damageImmuneCD = 0.01f;

	strafeImmuneTimer = strafeImmuneCD;
	damageImmuneTimer = damageImmuneCD;

	atkSpeed = atkSpd;
	atkDamage = atkdmg;
	timer = atkSpeed;
	player->renderingComponent.mat.surfaceColor = { 1.0f,0.0f,0.0f,1.0f };
	player->rigidBody.applyFriction = true;
	player->rigidBody.mass = 0.2f;
}


Player::~Player()
{
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
	if (canTilt) 
	{
		Tilt(dt);
	}
	if(canDamageImmuneTimer)
	{
		DamageImmuneCD(dt);
	}
	if (canStrafeImmuneTimer)
	{
		StrafeImmuneCD(dt);
	}

	if (health < botDisplayHealth)
	{
		botDisplayHealth -= 20.0f * dt;
		if(botDisplayHealth < health)
		{
			botDisplayHealth = health;
		}
	}
	if(health > topDisplayHealth)
	{
		topDisplayHealth += 20.0f * dt;
		if (topDisplayHealth > health)
		{
			topDisplayHealth = health;
		}
	}
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
	player->transform.SetRotatation(player->transform.rotation.x, player->transform.rotation.y, currentRot);

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
		player->transform.SetRotatation(player->transform.rotation.x, player->transform.rotation.y, currentTilt);
	}
	else if (currentTurnState == RIGHT)
	{
		if (currentTilt > -tiltDegrees) {
			currentTilt -= tiltSpeed * dt;
		}
		player->transform.SetRotatation(player->transform.rotation.x, player->transform.rotation.y, currentTilt);
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
		player->transform.SetRotatation(player->transform.rotation.x, player->transform.rotation.y, currentTilt);
	}
}

void Player::GainHealth(float hlt)
{
	health += hlt;
	if(health > maxHealth)
	{
		health = maxHealth;
	}
	botDisplayHealth = health;
}

void Player::TakeDamage(float dmg)
{
	if(!damageImmune && !strafeImmune && !isDead)
	{
		health -= dmg;
		if (health <= 0.0f)
		{
			health = 0.0f;
			isDead = true;
			player->renderingComponent.canRender = false;
		}
		topDisplayHealth = health;
		damageImmune = true;
		canDamageImmuneTimer = true;
	}
}

void Player::StrafeImmuneCD(float dt)
{
	strafeImmuneTimer -= 1.0f *dt;
	if (strafeImmuneTimer <= 0.0f)
	{
		strafeImmune = false;
		canStrafeImmuneTimer = false;
		strafeImmuneTimer = strafeImmuneCD;
	}
}

void Player::DamageImmuneCD(float dt)
{
	damageImmuneTimer -= 1.0f *dt;
	if (damageImmuneTimer <= 0.0f)
	{
		damageImmune = false;
		canDamageImmuneTimer = false;
		damageImmuneTimer = damageImmuneCD;
	}
}

DirectX::XMFLOAT3 Player::FindDistAway(DirectX::XMFLOAT3 dir, float dist)
{
	DirectX::XMFLOAT3 pos = { player->transform.position.x + (dir.x * dist), player->transform.position.y + (dir.y * dist), player->transform.position.z + (dir.z * dist) };
	return pos;
}
