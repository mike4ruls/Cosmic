#include "Bullet.h"



Bullet::Bullet()
{
	isActive = false;
	timer = 0.0f;
}

Bullet::Bullet(GameEntity * obj)
{
	bullet = obj;
	bullet->renderingComponent.mat.surfaceColor = {0.0f, 1.0f, 1.0f, 1.0f};
	isActive = false;
	bullet = obj;
}


Bullet::~Bullet()
{
	if (bullet != nullptr) { delete bullet; bullet = nullptr; }
}

void Bullet::Update(float dt)
{
	if (isActive)
	{
		bullet->transform.Translate(bulletVelocity.x * dt, bulletVelocity.y * dt, bulletVelocity.z * dt);
		DeactivateTimer(dt);
	}
	bullet->Update(dt);
}

void Bullet::Activate(DirectX::XMFLOAT3 bulletPos, DirectX::XMFLOAT3 bulletVelo, BulletType type)
{
	bullet->transform.position = bulletPos;
	bulletVelocity = bulletVelo;
	isActive = true;
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
	isActive = false;
}
