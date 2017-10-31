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

	bool canAttack;
	bool canStrafe;
	bool canTilt;
	bool isDead;

	DirectX::XMFLOAT3 originalRot;

	GameEntity* player = nullptr;

	void Update(float dt);
	void Strafe(float dt);
	void TurnOnStrafe();
	void Tilt(float dt);
	void TakeDamage(float dmg);
	DirectX::XMFLOAT3 FindDistAway(DirectX::XMFLOAT3 dir, float dist);
};

