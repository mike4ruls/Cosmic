#include "FinishLine.h"



FinishLine::FinishLine()
{
}

FinishLine::FinishLine(GameEntity * obj, float spd)
{
	finishLine = obj;
	speed = spd;
	finishLine->SetActive(false);
}




FinishLine::~FinishLine()
{
}

void FinishLine::Update(float dt)
{
	if(finishLine->isActive)
	{
		finishLine->transform.Translate(0.0f, 0.0f, -speed * dt);
	}
}
