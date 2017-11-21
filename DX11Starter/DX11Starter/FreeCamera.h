#pragma once
#include "Camera.h"
class FreeCamera :
	public Camera
{
public:
	FreeCamera();
	~FreeCamera();

	void SetMatricies();
	void Init(unsigned int w, unsigned int h);
	void CheckInputs(float dt);
};

