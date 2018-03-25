#pragma once
#include <DirectXMath.h>
#include <algorithm>
#include "InputManager.h"
#include "Transform.h"
#include "RigidBody.h"

struct CamShakeInfo
{
	float timerCD;
	float shakeAmmount;
	CamShakeInfo(float shkTime, float shkAm)
	{
		timerCD = shkTime;
		shakeAmmount = shkAm;
	}
};

class Camera
{
public:
	Camera();
	~Camera();

	void Update(float dt);
	void UpdateCamShake(float dt);
	void ShakeCamera(CamShakeInfo newInfo);
	void ShakeCamera(float shkTime, float shkAmnt);
	virtual void SetMatricies() = 0;
	virtual void Init(unsigned int w, unsigned int h) = 0;
	virtual void CheckInputs(float dt) = 0;
	void RotateCamera(float x, float y);
	void ResetCamera();

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

	bool lockCameraRot = false;
	bool lockCameraPos = false;

private:

protected:
	// Camera Shake Stuff
	std::vector<CamShakeInfo> shakeList;
	float totalShakeAmmount = 0.0f;
	DirectX::XMFLOAT3 camShakePos = {};
};

