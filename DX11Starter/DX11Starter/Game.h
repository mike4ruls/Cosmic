#pragma once
#include "CosmicEngine.h"
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
	virtual void Draw(float deltaTime, float totalTime) = 0;


	std::vector<GameEntity*>* gameObjects;
	Camera* cam;
	Renderer* rend;

private:
};

