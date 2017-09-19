#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "RigidBody.h"

class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	Transform transform;
	RigidBody rigidBody;

	float normSpeed;
	float runSpeed;
	float camSpeed;

	// Size of the window's client area
	unsigned int width;
	unsigned int height;

	void Update(float dt);
	void SetMatricies();
	void Init(unsigned int w, unsigned int h);

private:

};

