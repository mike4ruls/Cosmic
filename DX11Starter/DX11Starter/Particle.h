#pragma once
#include <DirectXMath.h>

struct Particle{
	DirectX::XMFLOAT3 originalPos;
	DirectX::XMFLOAT3 startingPos;
	DirectX::XMFLOAT3 accelDir;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT4 color;
	float size;
	float lifeTime;
	float age;
	bool isDead;
};