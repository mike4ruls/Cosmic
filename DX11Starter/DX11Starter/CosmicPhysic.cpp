#include "CosmicPhysic.h"



CosmicPhysic::CosmicPhysic()
{
}


CosmicPhysic::~CosmicPhysic()
{
}

bool CosmicPhysic::SphereVSphereCollision(GameEntity* obj1, GameEntity* obj2)
{
	DirectX::XMFLOAT3 distBetweenObj = { obj2->transform.position.x - obj1->transform.position.x, obj2->transform.position.y - obj1->transform.position.y, obj2->transform.position.z - obj1->transform.position.z };

	float radDist = std::pow(obj2->rigidBody.myCollider.radius + obj1->rigidBody.myCollider.radius, 2.0f);

	float dist = std::pow(distBetweenObj.x, 2.0f) + std::pow(distBetweenObj.y, 2.0f) + std::pow(distBetweenObj.z, 2.0f);

	if(dist < radDist)
	{
		return true;
	}
	else
	{
		return false;
	}

}
