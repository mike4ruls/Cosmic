#pragma once
#include "Transform.h"
#include <DirectXMath.h>

class Light {
public:
	Light();
	~Light();

	static Light* CreateDirectionalLight();
	static Light* CreatePointLight();
	static Light* CreateSpotLight();

	enum LightType {
		Directional,
		Point,
		Spot
	}lightType;
	unsigned int lightID;
	unsigned int compID;
	bool lightOn;
	
	struct LightComponent {
		DirectX::XMFLOAT4 lightColor;
		DirectX::XMFLOAT4 lightAmb;
		DirectX::XMFLOAT3 lightPos;
		float lightIntensity;
		DirectX::XMFLOAT3 lightDir;
		float pad;

	}* ligComponent;
};

//struct PointLight {
//	DirectX::XMFLOAT4 lightColor;
//	DirectX::XMFLOAT4 lightAmb;
//	DirectX::XMFLOAT3 lightPos;
//	float lightIntensity;
//};
//
//struct SpotLight {
//	DirectX::XMFLOAT4 lightColor;
//	DirectX::XMFLOAT4 lightAmb;
//	DirectX::XMFLOAT3 lightPos;
//	float lightIntensity;
//};

//enum LightType {
//	Directional,
//	Point,
//	Spot
//}lightType;
//Transform transform;
//DirectX::XMFLOAT4 lightColor;
//DirectX::XMFLOAT4 lightAmb;
//DirectX::XMFLOAT3 lightDir;
//float lightIntensity;
//unsigned int lightID;
//bool lightOn;
