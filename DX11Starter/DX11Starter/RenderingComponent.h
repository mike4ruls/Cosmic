#pragma once
#include "Material.h"

struct RenderingComponent {
	std::string meshName;
	DirectX::XMFLOAT4X4 worldMat;
	DirectX::XMFLOAT4X4 worldInvTrans;
	Material mat;
	unsigned int rendID;
	bool canRender;
};