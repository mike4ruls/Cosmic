#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Collider.h"

struct RigidBody {
	Collider myCollider;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 acceleration;
	float mass;
	float maxSpeed;
	float fricStrength;
	bool applyFriction;
	bool applyGravity;
	bool isMoving;

	void ApplyForce(DirectX::XMFLOAT3 force) {
		DirectX::XMVECTOR accel = DirectX::XMLoadFloat3(&acceleration);
		DirectX::XMVECTOR newForce = DirectX::XMLoadFloat3(&force);
		DirectX::XMVECTOR strength = { 0.001f, 0.001f, 0.001f };
		DirectX::XMVECTOR myMass = { mass, mass, mass };

		newForce = DirectX::XMVectorMultiply(newForce, strength);
		newForce = DirectX::XMVectorDivide(newForce, myMass);

		accel = DirectX::XMVectorAdd(accel, newForce);

		DirectX::XMStoreFloat3(&acceleration, accel);
	}
	void ApplyForce(float x, float y, float z) {
		DirectX::XMVECTOR accel = DirectX::XMLoadFloat3(&acceleration);
		DirectX::XMVECTOR newForce = {x, y, z};
		DirectX::XMVECTOR strength = { 0.001f, 0.001f, 0.001f };
		DirectX::XMVECTOR myMass = { mass, mass, mass };

		newForce = DirectX::XMVectorMultiply(newForce, strength);
		newForce = DirectX::XMVectorDivide(newForce, myMass);

		accel = DirectX::XMVectorAdd(accel, newForce);

		DirectX::XMStoreFloat3(&acceleration, accel);
	}
	void ApplyFriction() {
		if (velocity.x == 0.0f && velocity.y == 0.0f && velocity.z == 0.0f) {
			return;
		}
		DirectX::XMVECTOR accel = DirectX::XMLoadFloat3(&acceleration);
		DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&velocity);
		DirectX::XMVECTOR strength = { -0.3f, -0.3f, -0.3f };
		DirectX::XMVECTOR fricStren = { fricStrength ,fricStrength ,fricStrength };

		DirectX::XMVECTOR fricForce = DirectX::XMVectorMultiply(DirectX::XMVectorMultiply(vel, strength), fricStren);
		accel = DirectX::XMVectorAdd(accel, fricForce);

		DirectX::XMStoreFloat3(&acceleration, accel);

		//DirectX::XMFLOAT3 fricForce = { (velocity.x * -1) / fricStrength, (velocity.y * -1) / fricStrength, (velocity.z * -1) / fricStrength };
		/*DirectX::XMFLOAT3 fricForce = { (velocity.x * -0.3f) * fricStrength, (velocity.y * -0.3f) * fricStrength, (velocity.z * -0.3f) * fricStrength };
		acceleration.x += fricForce.x;
		acceleration.y += fricForce.y;
		acceleration.z += fricForce.z;*/
	}
	void UpdateVelocity(Transform* transform, float dt)
	{
		if(applyGravity)
		{
			ApplyForce(0.0f, -9.81f, 0.0f);
		}
		if (applyFriction)
		{
			ApplyFriction();
		}

		velocity.x += acceleration.x * dt;
		velocity.y += acceleration.y * dt;
		velocity.z += acceleration.z * dt;

		acceleration = { 0.0f, 0.0f, 0.0f };

		DirectX::XMVECTOR vel = DirectX::XMLoadFloat3(&velocity);
		vel = DirectX::XMVector3ClampLength(vel, 0, maxSpeed);

		DirectX::XMFLOAT3 speed;
		DirectX::XMVECTOR sp = DirectX::XMVector3LengthSq(vel);
		DirectX::XMStoreFloat3(&speed, sp);

		DirectX::XMStoreFloat3(&velocity, vel);

		/*if (speed.x < 0.005f)
		{
		rigidBody.velocity = {0.0f,0.0f,0.0f};
		rigidBody.isMoving = false;
		}
		else
		{
		rigidBody.isMoving = true;
		}*/
		transform->Translate(velocity);
	}
};