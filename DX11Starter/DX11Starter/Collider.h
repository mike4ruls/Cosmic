#pragma once
#include <DirectXMath.h>

struct Collider {
	enum ColliderType {
		NONE,
		BoxCollider,
		SphereCollider
	}colliderType;
	bool isTrigger;
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 size;
	float radius;

};
