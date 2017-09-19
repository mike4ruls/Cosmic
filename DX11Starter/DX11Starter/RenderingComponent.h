#pragma once

struct RenderingComponent {
	DirectX::XMFLOAT4X4 worldMat;
	bool canRender;
};