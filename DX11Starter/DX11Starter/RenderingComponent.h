#pragma once
#include "Material.h"

struct RenderingComponent {
	std::string meshName;
	DirectX::XMFLOAT4X4 worldMat;
	Material mat;
	bool canRender;
};