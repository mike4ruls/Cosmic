#pragma once
#include <DirectXMath.h>
struct ParticleVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 color;
	float size;

	ParticleVertex() {
		DirectX::XMFLOAT3 pos = {};
		DirectX::XMFLOAT2 uv = {};
		DirectX::XMFLOAT4 color = {};
		size = 1.0f;
	};
	ParticleVertex(DirectX::XMFLOAT3 p, DirectX::XMFLOAT2 u, DirectX::XMFLOAT4 c) {
		pos = p;
		uv = u;
		color = c;
		size = 1.0f;
	};
};