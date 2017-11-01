#pragma once
#include <vector>
#include "GameEntity.h"
#include "UI.h"
#include "Camera.h"
#include "Renderer.h"


class Game 
{
public:
	Game() {}
	virtual ~Game() 
	{
		if (cam) { delete cam; cam = nullptr; }
	}

	virtual void Init() = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;

	Camera* cam;
	InputManager* inputManager;

private:
};

