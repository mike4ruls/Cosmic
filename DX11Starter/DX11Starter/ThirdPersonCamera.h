#pragma once
#include "Camera.h"
#include "GameEntity.h"
class ThirdPersonCamera :
	public Camera
{
public:
	ThirdPersonCamera();
	~ThirdPersonCamera();

	void SetMatricies();
	void Init(unsigned int w, unsigned int h);
	void CheckInputs(float dt);

	void SetTarget(GameEntity* tar);

	 // ================================================================================= //
	 // ============================Third Person Camera Stuff============================ //
	 // ================================================================================= //
	float radius;
	float maxRadius;

	float maxRotY;
	float minRotY;


	float rotDegreesX;
	float rotDegreesY;

	float rotSpeedTPV;

	bool drag;

	DirectX::XMFLOAT3 endPos;
	DirectX::XMFLOAT3 centerHeight;
	DirectX::XMFLOAT3 panaramicTranslation;
	// ================================================================================= //

	GameEntity* target;
};

