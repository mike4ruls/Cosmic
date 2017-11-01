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
}

void BackGroundTiles::Update(float dt)
{
	tile->transform.Translate(moveVelocity.x * dt, moveVelocity.y * dt, moveVelocity.z * dt);
}

void BackGroundTiles::Teleport(DirectX::XMFLOAT3 pos)
{
	tile->transform.position = { tile->transform.position.x + pos.x, tile->transform.position.y + pos.y, tile->transform.position.z + pos.z};
}
