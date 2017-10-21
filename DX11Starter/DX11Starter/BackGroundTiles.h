#pragma once
#include "GameEntity.h"

class BackGroundTiles
{
public:
	BackGroundTiles();
	BackGroundTiles(GameEntity* obj, DirectX::XMFLOAT3 vel);
	~BackGroundTiles();

	DirectX::XMFLOAT3 moveVelocity;

	void Update(float dt);
	void Teleport(DirectX::XMFLOAT3 pos);

	GameEntity* tile;
};

