#pragma once
#define MIN_SIZE 1;

#include <DirectXMath.h>
#include "GameEntity.h"
#include <vector>

class CosmicPhysic
{
public:
	CosmicPhysic();
	~CosmicPhysic();

	bool SphereVSphereCollision(GameEntity* obj1, GameEntity* obj2);

	void UpdateTree();
	void BuildTree();
};
class BSPTree
{
public:
	struct BoundingBox {
		DirectX::XMFLOAT3 min;
		DirectX::XMFLOAT3 max;
	}boundingBox;
	std::vector<GameEntity> closeObjects;
	BSPTree* parent;

	BSPTree* left;
	BSPTree* right;

	int maxLifeSpan = 8;
	int curLife = -1;

	BSPTree(BoundingBox region, std::vector<GameEntity> objs)
	{
		boundingBox = region;
		closeObjects = objs;
		curLife = -1;
	};
	BSPTree(BoundingBox region)
	{
		boundingBox = region;
		curLife = -1;
	};
private:
	BSPTree()
	{
		boundingBox.max = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		boundingBox.min = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		curLife = -1;
	};
};

