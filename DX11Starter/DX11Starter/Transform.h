#pragma once
#include <DirectXMath.h>

struct Transform {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 foward;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 right;

	void Translate(DirectX::XMFLOAT3 trans) {

		position.x += trans.x;
		position.y += trans.y;
		position.z += trans.z;
	}
	void Translate(float x, float y, float z) {
		position.x += x;
		position.y += y;
		position.z += z;
	}
	void Rotate(DirectX::XMFLOAT3 rot) {
		rotation.x += rot.x;
		rotation.y += rot.y;
		rotation.z += rot.z;
	}
	void Rotate(float x, float y, float z) {
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}
	void Scale(DirectX::XMFLOAT3 sca) {
		scale.x += sca.x;
		scale.y += sca.y;
		scale.z += sca.z;
	}
	void Scale(float x, float y, float z) {
		scale.x += x;
		scale.y += y;
		scale.z += z;
	}
	void Scale(float sca) {
		scale.x += sca;
		scale.y += sca;
		scale.z += sca;
	}
	void CalculateDirections(){
		DirectX::XMVECTOR f = DirectX::XMLoadFloat3(&foward);
		DirectX::XMVECTOR u = DirectX::XMLoadFloat3(&up);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&right);

		r = DirectX::XMVector3Cross(f, u);
		u = DirectX::XMVector3Cross(r, f);

		r = DirectX::XMVector3Normalize(r);
		u = DirectX::XMVector3Normalize(u);

		DirectX::XMStoreFloat3(&foward, f);
		DirectX::XMStoreFloat3(&up, u);
		DirectX::XMStoreFloat3(&right, r);
	}
};
