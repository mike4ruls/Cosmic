#pragma once
#include <DirectXMath.h>

struct Transform {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT3 foward;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 right;

	Transform()
	{
		position = {0.0f, 0.0f, 0.0f};
		rotation = { 0.0f, 0.0f, 0.0f };
		scale = { 1.0f, 1.0f, 1.0f };

		foward = { 0.0f, 0.0f, 1.0f };
		up = { 0.0f, 1.0f, 0.0f };
		right = {1.0f, 0.0f, 0.0f };
	}

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

		CalculateDirections();
	}
	void Rotate(float x, float y, float z) {
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;

		CalculateDirections();
	}
	void SetRotatation(DirectX::XMFLOAT3 rot) {
		rotation.x = rot.x;
		rotation.y = rot.y;
		rotation.z = rot.z;

		CalculateDirections();
	}
	void SetRotatation(float x, float y, float z) {
		rotation.x = x;
		rotation.y = y;
		rotation.z = z;

		CalculateDirections();
	}
	void Scale(DirectX::XMFLOAT3 sca) {
		scale.x = sca.x;
		scale.y = sca.y;
		scale.z = sca.z;
	}
	void Scale(float x, float y, float z) {
		scale.x = x;
		scale.y = y;
		scale.z = z;
	}
	void Scale(float sca) {
		scale.x = sca;
		scale.y = sca;
		scale.z = sca;
	}
	void CalculateDirections(){
		DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw((rotation.y * 3.1415f) / 180, (rotation.x * 3.1415f) / 180, (rotation.z * 3.1415f) / 180);

		DirectX::XMFLOAT3 genF = { 0.0f, 0.0f, 1.0f };
		DirectX::XMFLOAT3 genU = { 0.0f, 1.0f, 0.0f };
		DirectX::XMFLOAT3 genR = { 1.0f, 0.0f, 0.0f };

		DirectX::XMVECTOR f = DirectX::XMLoadFloat3(&genF);
		DirectX::XMVECTOR u = DirectX::XMLoadFloat3(&genU);
		DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&genR);

		f = DirectX::XMVector3Transform(f, rotMat);
		u = DirectX::XMVector3Transform(u, rotMat);
		r = DirectX::XMVector3Transform(r, rotMat);

		DirectX::XMStoreFloat3(&foward, DirectX::XMVector3Normalize(f));
		DirectX::XMStoreFloat3(&up, DirectX::XMVector3Normalize(u));
		DirectX::XMStoreFloat3(&right, DirectX::XMVector3Normalize(r));
	}
};
