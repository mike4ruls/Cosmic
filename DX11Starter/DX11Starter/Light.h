#pragma once
#include "Transform.h"
#include <DirectXMath.h>

struct Light {

	enum LightType {
		Directional,
		Point,
		Spot
	}lightType;
	Transform transform;
	DirectX::XMFLOAT4 lightColor;
	DirectX::XMFLOAT4 lightAmb;
	DirectX::XMFLOAT3 lightDir;
	float lightIntensity;
	unsigned int lightID;
	bool lightOn;

};
