#include "BackGroundTiles.h"



BackGroundTiles::BackGroundTiles()
{
}

BackGroundTiles::BackGroundTiles(GameEntity * obj, DirectX::XMFLOAT3 vel)
{
	tile = obj;
	moveVelocity = vel;
}


BackGroundTiles::~BackGroundTiles()
{
	if (tile != nullptr) { delete tile; tile = nullptr; }
}

void BackGroundTiles::Update(float dt)
{
	tile->transform.Translate(moveVelocity.x * dt, moveVelocity.y * dt, moveVelocity.z * dt);

	tile->Update(dt);
}

void BackGroundTiles::Teleport(DirectX::XMFLOAT3 pos)
{
	tile->transform.position = { tile->transform.position.x + pos.x, tile->transform.position.y + pos.y, tile->transform.position.z + pos.z};
}
