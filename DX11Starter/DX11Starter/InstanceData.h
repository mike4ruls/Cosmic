#pragma once
#include <DirectXMath.h>

struct InstanceData {
	DirectX::XMFLOAT4X4 worldMat;
	DirectX::XMFLOAT4X4 invTrans;
	DirectX::XMFLOAT4 color;
};
