#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <vector>
#include <random>
#include <ctime>
#include "CosmicPhysic.h"
#include "InputManager.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Renderer.h"
#include "Game.h"


class CosmicEngine
	: public DXCore
{

public:
	CosmicEngine(HINSTANCE hInstance);
	~CosmicEngine();

	std::map<int, int> keys;

	Renderer* rend;
	Camera* cam;
	Game* currentScene = nullptr;

	float dayTime;
	bool click;
	bool initFinished = false;
	bool lockSunLight = false;
	bool lockCamera = false;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	GameEntity* CreateGameObject(std::string name);
	GameEntity* CreateCanvasElement();

	// Loading new scenes
	void LoadDefaultScene(Game* newScene);
	void LoadScene(Game* newScene);
	void QuitLevel();

	CosmicPhysic* physicEngine;
	InputManager* inputManager;

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry();

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};
//CosmicEngine
