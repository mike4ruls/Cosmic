#include "Camera.h"



Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	rigidBody.UpdateVelocity(&transform, dt);
	SetMatricies();
	if (!lockCameraPos) {
		CheckInputs(dt);
	}
}

void Camera::UpdateCamShake(float dt)
{
	float rangeX = (((((float)rand() * 2) / RAND_MAX) - 1) * totalShakeAmmount);
	float rangeY = (((((float)rand() * 2) / RAND_MAX) - 1) * totalShakeAmmount);
	float rangeZ = (((((float)rand() * 2) / RAND_MAX) - 1) * totalShakeAmmount);
	camShakePos = { rangeX, rangeY, rangeZ };

	for (unsigned int i = 0; i < shakeList.size(); i++)
	{
		CamShakeInfo newInfo = shakeList[i];
		newInfo.timerCD -= dt;

		if (newInfo.timerCD <= 0.0f)
		{
			totalShakeAmmount -= newInfo.shakeAmmount;
			shakeList.erase(shakeList.begin() + i);
			i--;
		}
		else
		{
			shakeList[i] = newInfo;
		}
	}
}

void Camera::ShakeCamera(CamShakeInfo newInfo)
{
	totalShakeAmmount += newInfo.shakeAmmount;
	shakeList.push_back(newInfo);
}

void Camera::ShakeCamera(float shkTime, float shkAmnt)
{
	totalShakeAmmount += shkAmnt;
	shakeList.push_back(CamShakeInfo(shkTime, shkAmnt));
}

void Camera::RotateCamera(float x, float y)
{
	transform.Rotate(x * rotSpeed, y * rotSpeed, 0.0f);
	transform.rotation.y = (std::max<float>(std::min<float>(transform.rotation.y, 90.0f), -90.0f));
	transform.CalculateDirections();
}
void Camera::ResetCamera()
{

	transform.position = { 0.0f, 0.0f, -5.0f };
	transform.rotation = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };

	transform.foward = { 0.0f, 0.0f, 1.0f };
	transform.up = { 0.0f, 1.0f, 0.0f };
	transform.right = { 1.0f, 0.0f, 0.0f };

	rigidBody.velocity = { 0.0f, 0.0f, 0.0f };
	rigidBody.acceleration = { 0.0f, 0.0f, 0.0f };
	rigidBody.mass = 1.0f;
	rigidBody.maxSpeed = 1.0f;
	rigidBody.fricStrength = 3.0f;
	rigidBody.applyFriction = true;
	rigidBody.applyGravity = false;
	rigidBody.isMoving = false;

	shakeList.clear();
}

