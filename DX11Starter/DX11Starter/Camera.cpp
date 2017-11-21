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
}

