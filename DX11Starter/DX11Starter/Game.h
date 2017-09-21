#pragma once
#include <vector>
#include "GameEntity.h"
#include "Camera.h"
#include "Renderer.h"


class Game 
{
public:
	Game() {}
	virtual ~Game() {}

	virtual void Init() = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;


	std::vector<GameEntity*> gameObjects;
	Camera* cam;

private:
};

