#pragma once
#include <vector>
#include "GameEntity.h"
#include "Camera.h"
#include "Renderer.h"


class Game 
{
public:
	Game() {}
	virtual ~Game() 
	{
		if (cam) { delete cam; cam = nullptr; }

		if (gameObjects.size() != 0) {
			for (unsigned int i = 0; i < gameObjects.size(); i++) {
				if (gameObjects[i]) { delete gameObjects[i]; gameObjects[i] = nullptr; }
			}
		}
	}

	virtual void Init() = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;


	std::vector<GameEntity*> gameObjects;
	Camera* cam;
	InputManager* inputManager;

private:
};

