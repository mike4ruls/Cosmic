#pragma once
#include "Image.h"
#include "GameEntity.h"
#include "Bullet.h"

class Player
{
public:
	enum TurnState {
		STRAIGHT,
		LEFT,
		RIGHT
	};

	Player();
	Player(GameEntity* obj, Mesh* bul, float hlt, float atkSpd, float atkdmg);
	~Player();

	void InitUI();
	void Update(float dt);
	void Strafe(float dt);
	void TurnOnStrafe();
	void Tilt(float dt);
	void GainHealth(float hlt);
	void TakeDamage(float dmg);
	void StrafeImmuneCD(float dt);
	void DamageImmuneCD(float dt);
	DirectX::XMFLOAT3 FindDistAway(DirectX::XMFLOAT3 dir, float dist);
	void LoadBullets();

	void ShootBullets();

	void FrontShoot();
	void LeftShoot();
	void RightShoot();

	void RechargeSheild(float dt);

	Mesh* bulletMesh;

	TurnState currentTurnState;
	TurnState previousTurnState;
	TurnState staticCurTurnState;

	std::vector<Bullet*> frontBulletPool;
	std::vector<Bullet*> leftBulletPool;
	std::vector<Bullet*> rightBulletPool;

	Image* healthBar = nullptr;
	Image* healthBarFade = nullptr;
	Image* healthBarBack = nullptr;
	Image* healthBarBorder = nullptr;

	Image* sheildBar = nullptr;
	Image* sheildBarFade = nullptr;
	Image* sheildBarBack = nullptr;
	Image* sheildBarBorder = nullptr;

	float health;
	float topDisplayHealth;
	float botDisplayHealth;
	float maxHealth;

	float sheild;
	float topDisplaySheild;
	float botDisplaySheild;
	float maxSheild;

	float speed;
	float normSpeed;
	float strafeSpeed;

	float frontAtkSpeed;
	float frontAtkDamage;

	float leftAtkSpeed;
	float leftAtkDamage;

	float rightAtkSpeed;
	float rightAtkDamage;

	float sheildRechargeRate;
	float sheildTimer;

	float guidedMissleRechargeRate;
	float guidedMissleTimer;
	float guidedMissleDamage;

	float strafeForce;
	float strafeRotSpeed;
	float currentRot;

	float frontTimer;
	float leftTimer;
	float rightTimer;

	float tiltDegrees;
	float tiltSpeed;
	float currentTilt;

	float strafeImmuneCD;
	float damageImmuneCD;
	float strafeImmuneTimer;
	float damageImmuneTimer;

	int guidedMissleCount;
	int guidedMissleMax;

	int curLevel;
	int currency;

	bool frontCanAttack;
	bool leftCanAttack;
	bool rightCanAttack;


	bool canStrafe;
	bool canTilt;
	bool isDead;

	bool strafeImmune;
	bool damageImmune;

	bool canStrafeImmuneTimer;
	bool canDamageImmuneTimer;

	bool leftBlasterBought;
	bool rightBlasterBought;
	bool guidedMissleBought;
	bool sheildComponentBought;

	bool leftBlasterBroken;
	bool rightBlasterBroken;
	bool guidedMissleBroken;
	bool sheildComponentBroken;

	DirectX::XMFLOAT3 originalRot;

	GameEntity* player = nullptr;
};

