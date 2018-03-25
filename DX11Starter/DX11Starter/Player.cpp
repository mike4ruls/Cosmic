#include "Player.h"



Player::Player()
{
}

Player::Player(GameEntity* obj, Mesh* bul, float hlt, float atkSpd, float atkdmg)
{
	bulletMesh = bul;
	player = obj;
	currentTilt = LEFT;
	previousTurnState = LEFT;

	curLevel = 0;
	currency = 0;

	maxHealth = hlt;
	health = maxHealth;
	topDisplayHealth = maxHealth;
	botDisplayHealth = maxHealth;

	maxSheild = 00.0f;
	sheild = maxSheild;
	topDisplaySheild = maxSheild;
	botDisplaySheild = maxSheild;

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

	sheildRechargeRate = 0.1f;
	sheildTimer = 0.0f;

	guidedMissleRechargeRate = 5.0f;
	guidedMissleTimer = 0.0f;
	guidedMissleDamage = 0.0f;

	guidedMissleMax = 3;
	guidedMissleCount = guidedMissleMax;

	frontCanAttack = true;
	leftCanAttack = true;
	rightCanAttack = true;

	canStrafe = false;
	canTilt = true;
	isDead = false;

	strafeImmune = false;
	damageImmune = false;

	canStrafeImmuneTimer = false;
	canDamageImmuneTimer = false;

	leftBlasterBought = false;
	rightBlasterBought = false;
	guidedMissleBought = false;
	sheildComponentBought = false;

	leftBlasterBroken = false;
	rightBlasterBroken = false;
	guidedMissleBroken = false;
	sheildComponentBroken = false;

	strafeImmuneCD = 1.0f;
	damageImmuneCD = 0.01f;

	strafeImmuneTimer = strafeImmuneCD;
	damageImmuneTimer = damageImmuneCD;

	frontAtkSpeed = atkSpd;
	frontAtkDamage = atkdmg;
	frontTimer = frontAtkSpeed;

	leftAtkSpeed = 100;
	leftAtkDamage = 0;
	leftTimer = leftAtkSpeed;

	rightAtkSpeed = 100;
	rightAtkDamage = 0;
	rightTimer = rightAtkSpeed;

	player->renderingComponent->mat.surfaceColor = { 1.0f,0.0f,0.0f,1.0f };
	player->rigidBody.applyFriction = true;
	player->rigidBody.mass = 0.2f;

	LoadBullets();
	InitUI();
}


Player::~Player()
{
	for (unsigned int i = 0; i < frontBulletPool.size(); i++)
	{
		if (frontBulletPool[i] != nullptr) { delete frontBulletPool[i]; frontBulletPool[i]; }
	}
	for (unsigned int i = 0; i < leftBulletPool.size(); i++)
	{
		if (leftBulletPool[i] != nullptr) { delete leftBulletPool[i]; leftBulletPool[i]; }
	}
	for (unsigned int i = 0; i < rightBulletPool.size(); i++)
	{
		if (rightBulletPool[i] != nullptr) { delete rightBulletPool[i]; rightBulletPool[i]; }
	}
}

void Player::InitUI()
{
	healthBar = new Image();
	healthBar->SetUIColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	healthBar->SetAlignment(UI::Alignment::Left);
	healthBar->posX = 0.3f;
	healthBar->posY = -1.0f;
	healthBar->SetWidth(0.6f);
	healthBar->SetHeight(0.02f);

	healthBarFade = new Image();
	healthBarFade->SetUIColor({ 0.3f, 0.0f, 0.0f, 1.0f });
	healthBarFade->SetAlignment(healthBar->GetAlignment());
	healthBarFade->posX = healthBar->posX;
	healthBarFade->posY = healthBar->posY;
	healthBarFade->SetWidth(healthBar->GetWidth());
	healthBarFade->SetHeight(healthBar->GetHeight());

	healthBarBack = new Image();
	healthBarBack->SetUIColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	healthBarBack->posX = healthBar->posX + healthBar->GetXOffSet();
	healthBarBack->posY = healthBar->posY + healthBar->GetYOffSet();
	healthBarBack->SetWidth(healthBar->GetWidth());
	healthBarBack->SetHeight(healthBar->GetHeight());

	healthBarBorder = new Image();
	healthBarBorder->SetUIColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	healthBarBorder->posX = healthBar->posX + healthBar->GetXOffSet();
	healthBarBorder->posY = healthBar->posY + healthBar->GetYOffSet();
	healthBarBorder->SetWidth(healthBar->GetWidth() + 0.04f);
	healthBarBorder->SetHeight(healthBar->GetHeight() + 0.04f);

	sheildBar = new Image();
	sheildBar->SetUIColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	//sheildBar->SetAlignment(UI::Alignment::Left);
	sheildBar->posX = healthBar->posX - 0.4f;
	sheildBar->posY = healthBar->posY + 0.1f;
	sheildBar->SetWidth(0.5f);
	sheildBar->SetHeight(0.018f);

	sheildBarFade = new Image();
	sheildBarFade->SetUIColor({ 0.0f, 0.0f, 0.3f, 1.0f });
	sheildBarFade->SetAlignment(sheildBar->GetAlignment());
	sheildBarFade->posX = sheildBar->posX;
	sheildBarFade->posY = sheildBar->posY;
	sheildBarFade->SetWidth(sheildBar->GetWidth());
	sheildBarFade->SetHeight(sheildBar->GetHeight());

	sheildBarBack = new Image();
	sheildBarBack->SetUIColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	sheildBarBack->posX = sheildBar->posX + sheildBar->GetXOffSet();
	sheildBarBack->posY = sheildBar->posY + sheildBar->GetYOffSet();
	sheildBarBack->SetWidth(sheildBar->GetWidth());
	sheildBarBack->SetHeight(sheildBar->GetHeight());

	sheildBarBorder = new Image();
	sheildBarBorder->SetUIColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	sheildBarBorder->posX = sheildBar->posX + sheildBar->GetXOffSet();
	sheildBarBorder->posY = sheildBar->posY + sheildBar->GetYOffSet();
	sheildBarBorder->SetWidth(sheildBar->GetWidth() + 0.02f);
	sheildBarBorder->SetHeight(sheildBar->GetHeight() + 0.02f);

	healthBar->SetVisibility(false);
	healthBarFade->SetVisibility(false);
	healthBarBack->SetVisibility(false);
	healthBarBorder->SetVisibility(false);
	sheildBar->SetVisibility(false);
	sheildBarFade->SetVisibility(false);
	sheildBarBack->SetVisibility(false);
	sheildBarBorder->SetVisibility(false);
}

void Player::Update(float dt)
{
	healthBar->SetWidth(healthBarBack->GetWidth() * (topDisplayHealth / maxHealth));
	healthBarFade->SetWidth(healthBarBack->GetWidth() * (botDisplayHealth / maxHealth));
	if (sheildComponentBought)
	{
		sheildBar->SetWidth(sheildBarBack->GetWidth() * (topDisplaySheild / maxSheild));
		sheildBarFade->SetWidth(sheildBarBack->GetWidth() * (botDisplaySheild / maxSheild));
	}

	if (!frontCanAttack)
	{
		frontTimer -= dt;
		if (frontTimer <= 0.0f)
		{
			frontCanAttack = true;
			frontTimer = frontAtkSpeed;
		}
	}


	if (!leftCanAttack)
	{
		leftTimer -= dt;
		if (leftTimer <= 0.0f)
		{
			leftCanAttack = true;
			leftTimer = leftAtkSpeed;
		}
	}

	if (!rightCanAttack)
	{
		rightTimer -= dt;
		if (rightTimer <= 0.0f)
		{
			rightCanAttack = true;
			rightTimer = rightAtkSpeed;
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

	if (sheild < botDisplaySheild)
	{
		botDisplaySheild -= 20.0f * dt;
		if(botDisplaySheild < sheild)
		{
			botDisplaySheild = sheild;
		}
	}
	if(sheild > topDisplaySheild)
	{
		topDisplaySheild += 20.0f * dt;
		if (topDisplaySheild > sheild)
		{
			topDisplaySheild = sheild;
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
		if(sheild == 0)
		{
			health -= dmg;
			if (health <= 0.0f)
			{
				health = 0.0f;
				isDead = true;
				player->renderingComponent->canRender = false;
			}
			topDisplayHealth = health;
		}
		else
		{
			sheild -= dmg;
			if (sheild <= 0.0f)
			{
				sheild = 0.0f;
			}
			topDisplaySheild = sheild;
		}

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

void Player::LoadBullets()
{
	unsigned int numOfBullets = 60;

	for (unsigned int i = 0; i < numOfBullets; i++)
	{
		Bullet* newBullet = new Bullet(new GameEntity(bulletMesh, false));
		newBullet->bullet->transform.Translate(0.0f, 60.0f, 0.0f);
		newBullet->bullet->SetWorld();
		//newBullet->bullet->renderingComponent->mat.surfaceReflectance = 0.7f;

		frontBulletPool.push_back(newBullet);
	}

	for (unsigned int i = 0; i < numOfBullets; i++)
	{
		Bullet* newBullet = new Bullet(new GameEntity(bulletMesh, false));
		newBullet->bullet->transform.Translate(0.0f, 60.0f, 0.0f);
		newBullet->bullet->SetWorld();
		//newBullet->bullet->renderingComponent->mat.surfaceReflectance = 0.7f;

		leftBulletPool.push_back(newBullet);
	}

	for (unsigned int i = 0; i < numOfBullets; i++)
	{
		Bullet* newBullet = new Bullet(new GameEntity(bulletMesh, false));
		newBullet->bullet->transform.Translate(0.0f, 60.0f, 0.0f);
		newBullet->bullet->SetWorld();
		//newBullet->bullet->renderingComponent->mat.surfaceReflectance = 0.7f;

		rightBulletPool.push_back(newBullet);
	}

}

void Player::ShootBullets()
{
	if (frontCanAttack)
	{
		FrontShoot();
	}
	if (leftBlasterBought && !leftBlasterBroken && leftCanAttack)
	{
		LeftShoot();
	}
	if (rightBlasterBought && !rightBlasterBroken && rightCanAttack)
	{
		RightShoot();
	}
}

void Player::FrontShoot()
{
	for (unsigned int i = 0; i < frontBulletPool.size(); i++)
	{
		if (!frontBulletPool[i]->bullet->isActive)
		{
			frontBulletPool[i]->Activate(FindDistAway(player->transform.foward, 2.0f), { player->transform.foward.x * 80.0f, player->transform.foward.y * 80.0f, player->transform.foward.z * 80.0f }, Bullet::Regular);
			frontCanAttack = false;
			break;
		}
	}
}
void Player::LeftShoot()
{
	for (unsigned int i = 0; i < leftBulletPool.size(); i++)
	{
		if (!leftBulletPool[i]->bullet->isActive)
		{
			leftBulletPool[i]->Activate(FindDistAway(player->transform.right, -2.0f), { player->transform.foward.x * 80.0f, player->transform.foward.y * 80.0f, player->transform.foward.z * 80.0f }, Bullet::Regular);
			leftCanAttack = false;
			break;
		}
	}
}
void Player::RightShoot()
{
	for (unsigned int i = 0; i < rightBulletPool.size(); i++)
	{
		if (!rightBulletPool[i]->bullet->isActive)
		{
			rightBulletPool[i]->Activate(FindDistAway(player->transform.right, 2.0f), { player->transform.foward.x * 80.0f, player->transform.foward.y * 80.0f, player->transform.foward.z * 80.0f }, Bullet::Regular);
			rightCanAttack = false;
			break;
		}
	}
}
void Player::RechargeSheild(float dt)
{
	if(sheild != maxSheild && sheildComponentBought)
	{
		sheildTimer += dt;
		if (sheildTimer >= sheildRechargeRate)
		{
			sheild += 0.2f;
			sheildTimer = 0.0f;

			if (sheild >= maxSheild)
			{
				sheild = maxSheild;
			}
		}
	}
}
