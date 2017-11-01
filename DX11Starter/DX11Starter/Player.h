#pragma once
#include "GameEntity.h"

class Player
{
public:
	enum TurnState {
		STRAIGHT,
		LEFT,
		RIGHT
	};

	Player();
	Player(GameEntity* obj, float hlt, float atkSpd, float atkdmg);
	~Player();

	TurnState currentTurnState;
	TurnState previousTurnState;
	TurnState staticCurTurnState;

	float health;
	float topDisplayHealth;
	float botDisplayHealth;
	float maxHealth;

	float speed;
	float normSpeed;
	float strafeSpeed;

	float atkSpeed;
	float atkDamage;

	float strafeForce;
	float strafeRotSpeed;
	float currentRot;
	float timer;

	float tiltDegrees;
	float tiltSpeed;
	float currentTilt;

	float strafeImmuneCD;
	float damageImmuneCD;
	float strafeImmuneTimer;
	float damageImmuneTimer;

	bool canAttack;
	bool canStrafe;
	bool canTilt;
	bool isDead;

	bool strafeImmune;
	bool damageImmune;

	bool canStrafeImmuneTimer;
	bool canDamageImmuneTimer;

	DirectX::XMFLOAT3 originalRot;

	GameEntity* player = nullptr;

	void Update(float dt);
	void Strafe(float dt);
	void TurnOnStrafe();
	void Tilt(float dt);
	void GainHealth(float hlt);
	void TakeDamage(float dmg);
	void StrafeImmuneCD(float dt);
	void DamageImmuneCD(float dt);
	DirectX::XMFLOAT3 FindDistAway(DirectX::XMFLOAT3 dir, float dist);
};

