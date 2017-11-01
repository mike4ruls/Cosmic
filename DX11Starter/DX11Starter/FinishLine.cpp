#include "FinishLine.h"



FinishLine::FinishLine()
{
}

FinishLine::FinishLine(GameEntity * obj, float spd)
{
	finishLine = obj;
	speed = spd;
	isActive = false;
}




FinishLine::~FinishLine()
{
	if (finishLine != nullptr) { delete finishLine; finishLine = nullptr; }
}

void FinishLine::Update(float dt)
{
	if(isActive)
	{
		finishLine->transform.Translate(0.0f, 0.0f, -speed * dt);
		finishLine->Update(dt);
	}
}
