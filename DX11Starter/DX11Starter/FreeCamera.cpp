#include "FreeCamera.h"



FreeCamera::FreeCamera()
{
}


FreeCamera::~FreeCamera()
{
}

void FreeCamera::SetMatricies()
{
	//if (transform.foward.y < ((-90 * 3.1415f) / 180.0f))
	//{
	//	transform.foward.y = ((-90 * 3.1415f) / 180.0f);
	//}
	//else if (transform.foward.y >((90 * 3.1415f) / 180.0f))
	//{
	//	transform.foward.y = ((90 * 3.1415f) / 180.0f);
	//}

	//printf("%f, %f, %f\n", transform.foward.x, transform.foward.y, transform.foward.z );


	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	//radius = maxRadius * std::cosf(rotDegreesY * 0.5f);
	//DirectX::XMFLOAT3 rotPos = { radius * std::sin(rotDegreesX), radius * std::sinf(rotDegreesY), radius * std::cos(rotDegreesX) };\

	//DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&rotPos);
	//DirectX::XMVECTOR targetVec = DirectX::XMLoadFloat3(&target);
	////DirectX::XMVECTOR panaVec = DirectX::XMLoadFloat3(&panaramicTranslation);

	//DirectX::XMVECTOR result = DirectX::XMVectorAdd(targetVec, rotPosVec);

	//DirectX::XMStoreFloat3(&transform.position, result);

	//DirectX::XMVECTOR result2 = DirectX::XMVectorSubtract(targetVec, result);
	//result2 = DirectX::XMVector3Normalize(result2);

	//DirectX::XMStoreFloat3(&transform.foward, result2);

	//DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0, 1, 0, 0);
	//DirectX::XMVECTOR rightVec = DirectX::XMVector3Cross(upVec, result2);

	//DirectX::XMVECTOR newUpVec = DirectX::XMVector3Cross(result2, rightVec);

	//result = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSet(panaramicTranslation.x, panaramicTranslation.x, panaramicTranslation.x, 0.0f), rightVec, DirectX::XMLoadFloat3(&transform.position));
	//result = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSet(panaramicTranslation.y, panaramicTranslation.y, panaramicTranslation.y, 0.0f), newUpVec, result);
	//DirectX::XMStoreFloat3(&transform.position, result);
	// ================================================================================= //

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
		200.0f);					// Far clip plane distance
	DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}

void FreeCamera::Init(unsigned int w, unsigned int h)
{
	lockCameraPos = false;
	ResetCamera();

	width = w;
	height = h;
	normSpeed = 10.0f;
	runSpeed = 30.0f;
	camSpeed = normSpeed;
	rotSpeed = 1.0f;


	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	//radius = 0.0f;
	//maxRadius = 20.0f;

	//maxRotY = (45.0f * 3.1415f) / 180.0f;
	//minRotY = (-45.0f * 3.1415f) / 180.0f;

	//rotDegreesX = 0.0f;
	//rotDegreesY = 0.0f;
	//rotSpeedTPV = 2.0f;

	//target = { 0.0f, 0.0, 0.0f };
	//panaramicTranslation = {6.0f, 2.0, 0.0f};
	// ================================================================================= //

	SetMatricies();
}

void FreeCamera::CheckInputs(float dt)
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
		rigidBody.ApplyForce((transform.right.x * -camSpeed) * dt, (transform.right.y * -camSpeed) * dt, (transform.right.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(68) || inputManager->GetLeftStickX() > 0.0f) //D
	{
		rigidBody.ApplyForce((transform.right.x * camSpeed) * dt, (transform.right.y * camSpeed) * dt, (transform.right.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(87) || inputManager->GetLeftStickY() > 0.0f) //W
	{
		rigidBody.ApplyForce((transform.foward.x * camSpeed) * dt, (transform.foward.y * camSpeed) * dt, (transform.foward.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(83) || inputManager->GetLeftStickY() < 0.0f) //S
	{
		rigidBody.ApplyForce((transform.foward.x * -camSpeed) * dt, (transform.foward.y * -camSpeed) * dt, (transform.foward.z * -camSpeed) * dt);
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
	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	//if (inputManager->GetRightStickX() > 0.0f)
	//{
	//	rotDegreesX += rotSpeedTPV * dt;
	//}
	//if (inputManager->GetRightStickX() < 0.0f)
	//{
	//	rotDegreesX -= rotSpeedTPV * dt;
	//}
	//if (inputManager->GetRightStickY() > 0.0f)
	//{
	//	if (rotDegreesY > minRotY)
	//	{
	//		rotDegreesY -= rotSpeedTPV * dt;
	//	}
	//}
	//if (inputManager->GetRightStickY() < 0.0f)
	//{
	//	if (rotDegreesY < maxRotY)
	//	{
	//		rotDegreesY += rotSpeedTPV * dt;
	//	}
	//}
	// ================================================================================= //
}
