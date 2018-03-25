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
#include "TextBox.h"
#include "Button.h"
#include "Image.h"
#include "Camera.h"
#include "Renderer.h"
#include "Game.h"


class CosmicEngine
	: public DXCore
{

public:
	//struct MouseScreen
	//{
	//	float x;
	//	float y;
	//};
	CosmicEngine(HINSTANCE hInstance);
	~CosmicEngine();

	std::map<int, int> keys;

	std::vector<GameEntity*> allObj;
	std::vector<UI*> allUI;
	std::vector<Emitter*> allEmitter;

	Renderer* rend;
	Camera* cam;
	Game* currentScene = nullptr;

	bool pauseGame;
	bool initFinished = false;
	bool lockCamera = false;

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void UpdateObjects(float dt);
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	void PushGameObject(GameEntity* gameobject);
	void PushUI(UI* newUI);
	void PushEmitter(Emitter* newEmitter);

	// Loading new scenes
	void LoadDefaultScene(Game* newScene);
	void LoadScene(Game* newScene);
	void QuitLevel();
	void Flush();
	void DestroyGameObject(GameEntity* obj);
	void DestroyUIObject(UI* obj);
	void DestroyEmitter(Emitter* obj);

	ID3D11Device* GetDevice();

	CosmicPhysic* physicEngine;
	InputManager* inputManager;

	//MouseScreen currentMousePos;
	POINT prevMousePos;

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y, float dt);
	void OnMouseWheel(float wheelDelta, int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void CreateBasicGeometry();


};
//CosmicEngine
