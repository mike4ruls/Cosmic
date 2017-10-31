#include "Camera.h"



Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(unsigned int w, unsigned int h)
{
	lockCameraPos = false;

	transform.position = { 0.0f, 0.0f, -5.0f };
	transform.rotation = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };

	transform.foward = {0.0f, 0.0f, 1.0f};
	transform.up = { 0.0f, 1.0f, 0.0f };
	transform.right = { 1.0f, 0.0f, 0.0f };

	rigidBody.velocity = { 0.0f, 0.0f, 0.0f };
	rigidBody.acceleration = { 0.0f, 0.0f, 0.0f };
	rigidBody.mass = 1.0f;
	rigidBody.maxSpeed = 1.0f;
	rigidBody.fricStrength = 3.0f;
	rigidBody.applyFriction = true;
	rigidBody.isMoving = false;

	width = w;
	height = h;
	normSpeed = 10.0f;
	runSpeed = 30.0f;
	camSpeed = normSpeed;
	SetMatricies();
}

void Camera::Update(float dt)
{
	rigidBody.UpdateVelocity(&transform, dt);
	transform.CalculateDirections();
	SetMatricies();
	if (!lockCameraPos) {
		CheckInputs(dt);
	}
}

void Camera::CheckInputs(float dt)
{
	/////////////////////////
	//CAMERA MOVEMENT
	/////////////////////////
	if (inputManager->IsKeyDown(16)) // Left Shift
	{
		camSpeed = runSpeed *1000;
	}
	else
	{
		camSpeed = normSpeed * 1000;
	}

	if (inputManager->IsKeyDown(65)) //A
	{
		rigidBody.ApplyForce((transform.right.x * -camSpeed) * dt, (transform.right.y * -camSpeed) * dt, (transform.right.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(68)) //D
	{
		rigidBody.ApplyForce((transform.right.x * camSpeed) * dt, (transform.right.y * camSpeed) * dt, (transform.right.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(87)) //W
	{
		rigidBody.ApplyForce((transform.foward.x * camSpeed) * dt, (transform.foward.y * camSpeed) * dt, (transform.foward.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(83)) //S
	{
		rigidBody.ApplyForce((transform.foward.x * -camSpeed) * dt, (-transform.foward.y * -camSpeed) * dt, (transform.foward.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(32)) // SPACE
	{
		rigidBody.ApplyForce(0.0f, camSpeed * dt, 0.0f);
	}
	if (inputManager->IsKeyDown(88)) // X
	{
		rigidBody.ApplyForce(0.0f, -camSpeed * dt, 0.0f);
	}
}
void Camera::SetMatricies()
{
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 0);
	DirectX::XMVECTOR dir = DirectX::XMVectorSet(transform.foward.x, transform.foward.y, transform.foward.z, 0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

														// Create the Projection matrix
														// - This should match the window's aspect ratio, and also update anytime
														//    the window resizes (which is already happening in OnResize() below)
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}

