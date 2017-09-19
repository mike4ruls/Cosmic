#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Renderer.h"


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	std::vector<GameEntity*> gameObjects;

	Camera* cam;
	Renderer* rend;

	bool click;
	bool enterPressed;
	bool fPressed;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry();

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

