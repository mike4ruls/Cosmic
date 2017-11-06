#pragma once
#include <DirectXMath.h>
#include <algorithm>
#include "InputManager.h"
#include "Transform.h"
#include "RigidBody.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	InputManager* inputManager;

	Transform transform;
	RigidBody rigidBody;

	float normSpeed;
	float runSpeed;
	float camSpeed;

	float rotSpeed;

	// Size of the window's client area
	unsigned int width;
	unsigned int height;

	bool lockCameraPos;

	void Update(float dt);
	void SetMatricies();
	void Init(unsigned int w, unsigned int h);
	void CheckInputs(float dt);
	void RotateCamera(float x, float y);

private:

};

