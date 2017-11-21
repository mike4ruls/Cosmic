#pragma once
#include <vector>
#include "GameEntity.h"
#include "Image.h"
#include "TextBox.h"
#include "Button.h"
#include "FreeCamera.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"
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

