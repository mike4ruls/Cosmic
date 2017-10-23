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
	Player(GameEntity* obj, float atkSpd);
	~Player();

	TurnState currentTurnState;
	TurnState previousTurnState;

	float speed;
	float normSpeed;
	float strafeSpeed;

	float atkSpeed;

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

	DirectX::XMFLOAT3 originalRot;

	GameEntity* player = nullptr;

	void Update(float dt);
	void Strafe(float dt);
	void TurnOnStrafe();
	void Tilt(float dt);
	DirectX::XMFLOAT3 FindDistAway(DirectX::XMFLOAT3 dir, float dist);
};

