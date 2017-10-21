#pragma once
#include "GameEntity.h"

class BackGroundTiles
{
public:
	BackGroundTiles();
	~BackGroundTiles();

	float speed;
	DirectX::XMFLOAT3 moveDir;

	void Update();
	void Teleport(DirectX::XMFLOAT3 pos);

	GameEntity tile;
};

