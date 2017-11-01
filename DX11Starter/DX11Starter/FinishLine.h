#pragma once
#include "GameEntity.h"

class FinishLine
{
public:
	FinishLine();
	FinishLine(GameEntity* obj, float spd);
	~FinishLine();

	GameEntity* finishLine;
	float speed;

	void Update(float dt);
};

