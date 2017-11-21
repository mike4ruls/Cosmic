#pragma once
#include "Camera.h"
class FirstPersonCamera :
	public Camera
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	void SetMatricies();
	void Init(unsigned int w, unsigned int h);
	void CheckInputs(float dt);
};

