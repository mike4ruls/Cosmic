#include "FirstPersonCamera.h"



FirstPersonCamera::FirstPersonCamera()
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}

void FirstPersonCamera::SetMatricies()
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(transform.position.x + camShakePos.x, transform.position.y + camShakePos.y, transform.position.z + camShakePos.z, 0);
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
		200.0f);					// Far clip plane distance
	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}

void FirstPersonCamera::Init(unsigned int w, unsigned int h)
{
	lockCameraPos = false;
	ResetCamera();

	width = w;
	height = h;
	normSpeed = 10.0f;
	runSpeed = 30.0f;
	camSpeed = normSpeed;
	rotSpeed = 1.0f;

	SetMatricies();
}

void FirstPersonCamera::CheckInputs(float dt)
{
	/////////////////////////
	//CAMERA MOVEMENT
	/////////////////////////
	if (inputManager->IsKeyDown(16) || inputManager->IsButtonDown(CosmicInput::BUTTON_L3)) // Left Shift
	{
		camSpeed = runSpeed * 1000;
	}
	else
	{
		camSpeed = normSpeed * 1000;
	}

	if (inputManager->IsKeyDown(65) || inputManager->GetLeftStickX() < 0.0f) //A
	{
		rigidBody.ApplyForce((transform.right.x * -camSpeed) * dt, 0.0f, (transform.right.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(68) || inputManager->GetLeftStickX() > 0.0f) //D
	{
		rigidBody.ApplyForce((transform.right.x * camSpeed) * dt, 0.0f, (transform.right.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(87) || inputManager->GetLeftStickY() > 0.0f) //W
	{
		rigidBody.ApplyForce((transform.foward.x * camSpeed) * dt, 0.0f, (transform.foward.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(83) || inputManager->GetLeftStickY() < 0.0f) //S
	{
		rigidBody.ApplyForce((transform.foward.x * -camSpeed) * dt, 0.0f, (transform.foward.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(32) || inputManager->IsButtonDown(CosmicInput::BUTTON_CROSS)) // SPACE
	{
		rigidBody.ApplyForce(0.0f, camSpeed * dt, 0.0f);
	}
	if (inputManager->IsKeyDown(88) || inputManager->IsButtonDown(CosmicInput::BUTTON_SQUARE)) // X
	{
		rigidBody.ApplyForce(0.0f, -camSpeed * dt, 0.0f);
	}

	if (inputManager->GetRightStickX() > 0.0f)
	{
		RotateCamera(50.0f * dt, 0.0f);
	}
	if (inputManager->GetRightStickX() < 0.0f)
	{
		RotateCamera(-50.0f * dt, 0.0f);
	}
	if (inputManager->GetRightStickY() > 0.0f)
	{
		RotateCamera(0.0f, -50.0f * dt);
	}
	if (inputManager->GetRightStickY() < 0.0f)
	{
		RotateCamera(0.0f, 50.0f * dt);
	}
}

