#include "ThirdPersonCamera.h"



ThirdPersonCamera::ThirdPersonCamera()
{
}


ThirdPersonCamera::~ThirdPersonCamera()
{

}
void ThirdPersonCamera::Init(unsigned int w, unsigned int h)
{
	lockCameraPos = false;
	ResetCamera();

	rigidBody.fricStrength = 35.0f;

	width = w;
	height = h;
	normSpeed = 20.0f;
	runSpeed = 40.0f;
	camSpeed = normSpeed;
	rotSpeed = 1.0f;


	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	radius = 0.0f;
	maxRadius = 30.0f;

	maxRotY = (45.0f * 3.1415f) / 180.0f;
	minRotY = (-45.0f * 3.1415f) / 180.0f;

	rotDegreesX = 0.0f;
	rotDegreesY = 0.0f;
	rotSpeedTPV = 2.0f;

	drag = true;

	//panaramicTranslation = {6.0f, 2.0, 0.0f};
	panaramicTranslation = { 10.0f, 5.0, 0.0f };
	centerHeight = { 0.0f, 1.0, 0.0f };
	// ================================================================================= //

	//SetMatricies();
}
void ThirdPersonCamera::SetMatricies()
{
	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	radius = maxRadius * std::cosf(rotDegreesY * 0.5f);
	DirectX::XMFLOAT3 rotPos = { radius * std::sin(rotDegreesX), radius * std::sinf(rotDegreesY), radius * std::cos(rotDegreesX) };\

	DirectX::XMVECTOR rotPosVec = DirectX::XMLoadFloat3(&rotPos);
	DirectX::XMVECTOR centerHeightVec = DirectX::XMLoadFloat3(&centerHeight);
	DirectX::XMVECTOR targetVec = DirectX::XMVectorAdd(centerHeightVec,DirectX::XMLoadFloat3(&target->transform.position));

	DirectX::XMVECTOR result = DirectX::XMVectorAdd(targetVec, rotPosVec);

	//DirectX::XMStoreFloat3(&transform.position, result);

	DirectX::XMVECTOR result2 = DirectX::XMVectorSubtract(targetVec, result);
	result2 = DirectX::XMVector3Normalize(result2);

	DirectX::XMStoreFloat3(&transform.foward, result2);

	DirectX::XMVECTOR upVec = DirectX::XMVectorSet(0, 1, 0, 0);
	DirectX::XMVECTOR rightVec = DirectX::XMVector3Cross(upVec, result2);

	DirectX::XMVECTOR newUpVec = DirectX::XMVector3Cross(result2, rightVec);

	result = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSet(panaramicTranslation.x, panaramicTranslation.x, panaramicTranslation.x, 0.0f), rightVec, result);
	result = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSet(panaramicTranslation.y, panaramicTranslation.y, panaramicTranslation.y, 0.0f), newUpVec, result);
	
	if (!drag)
	{
		DirectX::XMStoreFloat3(&transform.position, result);
	}
	else 
	{
		DirectX::XMStoreFloat3(&endPos, result);
	}

	DirectX::XMStoreFloat3(&transform.right, rightVec);
	DirectX::XMStoreFloat3(&transform.up, upVec);
	// ================================================================================= //

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

void ThirdPersonCamera::CheckInputs(float dt)
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
		target->rigidBody.ApplyForce((transform.right.x * -camSpeed) * dt, 0.0f, (transform.right.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(68) || inputManager->GetLeftStickX() > 0.0f) //D
	{
		target->rigidBody.ApplyForce((transform.right.x * camSpeed) * dt, 0.0f, (transform.right.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(87) || inputManager->GetLeftStickY() > 0.0f) //W
	{
		target->rigidBody.ApplyForce((transform.foward.x * camSpeed) * dt, 0.0f, (transform.foward.z * camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(83) || inputManager->GetLeftStickY() < 0.0f) //S
	{
		target->rigidBody.ApplyForce((transform.foward.x * -camSpeed) * dt, 0.0f, (transform.foward.z * -camSpeed) * dt);
	}
	if (inputManager->IsKeyDown(32) || inputManager->IsButtonDown(CosmicInput::BUTTON_CROSS)) // SPACE
	{
		target->rigidBody.ApplyForce(transform.foward.x, 100.0f, transform.foward.z);
		target->rigidBody.applyGravity = true;
	}
	if (inputManager->IsButtonPressed(CosmicInput::BUTTON_R3))
	{
		panaramicTranslation.x *= -1;
	}
	//if (inputManager->IsKeyDown(88) || inputManager->IsButtonDown(CosmicInput::BUTTON_SQUARE)) // X
	//{
	//	rigidBody.ApplyForce(0.0f, -camSpeed * dt, 0.0f);
	//}

	if(target->rigidBody.applyGravity && target->transform.position.y < 1.0f)
	{
		target->rigidBody.applyGravity = false;
		target->rigidBody.velocity = {0.0f, 0.0, 0.0f};
	}

	// ================================================================================= //
	// ============================Third Person Camera Stuff============================ //
	// ================================================================================= //
	if (inputManager->GetRightStickX() > 0.0f)
	{
		rotDegreesX += rotSpeedTPV * dt;
	}
	if (inputManager->GetRightStickX() < 0.0f)
	{
		rotDegreesX -= rotSpeedTPV * dt;
	}
	if (inputManager->GetRightStickY() > 0.0f)
	{
		if (rotDegreesY > minRotY)
		{
			rotDegreesY -= rotSpeedTPV * dt;
		}
	}
	if (inputManager->GetRightStickY() < 0.0f)
	{
		if (rotDegreesY < maxRotY)
		{
			rotDegreesY += rotSpeedTPV * dt;
		}
	}
	// ================================================================================= //
	//DirectX::XMVECTOR endPosVec = DirectX::XMLoadFloat3(&endPos);
	//DirectX::XMVECTOR startPosVec = DirectX::XMLoadFloat3(&transform.position);

	//DirectX::XMVectorLerp(endPosVec, startPosVec,);
	if(drag)
	{
		float magSqr = std::pow(endPos.x - transform.position.x, 2.0f) + std::pow(endPos.y - transform.position.y, 2.0f) + std::pow(endPos.z - transform.position.z, 2.0f);
		if (magSqr > 2.0f*2.0f)
		{
			rigidBody.ApplyForce((endPos.x - transform.position.x)*10.0f, (endPos.y - transform.position.y)*10.0f, (endPos.z - transform.position.z)*10.0f);
		}
	}
	//transform.Translate((endPos.x - transform.position.x)*4.0f*dt, (endPos.y - transform.position.y)*4.0f*dt, (endPos.z - transform.position.z)*4.0f*dt);
}

void ThirdPersonCamera::SetTarget(GameEntity * tar)
{
	target = tar;
}

