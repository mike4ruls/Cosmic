#include "Bullet.h"



Bullet::Bullet()
{
	timer = 0.0f;
}

Bullet::Bullet(GameEntity * obj)
{
	bullet = obj;
	bullet->renderingComponent->mat.surfaceColor = {0.0f, 1.0f, 1.0f, 1.0f};
	bullet->SetActive(false);
	bullet = obj;
}


Bullet::~Bullet()
{
}

void Bullet::Update(float dt)
{
	if (bullet->isActive)
	{
		bullet->transform.Translate(bulletVelocity.x * dt, bulletVelocity.y * dt, bulletVelocity.z * dt);
		DeactivateTimer(dt);
	}
}

void Bullet::Activate(DirectX::XMFLOAT3 bulletPos, DirectX::XMFLOAT3 bulletVelo, BulletType type)
{
	bullet->transform.position = bulletPos;
	bulletVelocity = bulletVelo;
	bullet->SetActive(true);
	timer = DEACTIVATE_CD;
	bulletType = type;
}

void Bullet::DeactivateTimer(float dt)
{
	timer -= dt; // 1 * deltaTime
	if (timer <= 0.0f) 
	{
		Deactivate();
	}
}

void Bullet::Deactivate()
{
	bullet->transform.position = {0.0f, 60.0f, 0.0f};
	bullet->SetWorld();
	bullet->SetActive(false);
}
