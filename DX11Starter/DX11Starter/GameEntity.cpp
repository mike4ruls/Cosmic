#include "GameEntity.h"



GameEntity::GameEntity()
{
	// Do i need to declare structs?
	transform = Transform();
	rigidBody = RigidBody();
	renderingComponent = RenderingComponent();
	myMesh = new Mesh();
	name = "";
	renderingComponent.canRender = true;
	ResetGameEntity();
}

GameEntity::GameEntity(Mesh * m, Renderer* r)
{
	transform = Transform();
	rigidBody = RigidBody();
	renderingComponent = RenderingComponent();
	myMesh = m;
	rend = r;
	name = "";
	renderingComponent.canRender = true;

	ResetGameEntity();
	rendID = rend->PushToRenderer(myMesh->meshName, &renderingComponent);
}


GameEntity::~GameEntity()
{

}

void GameEntity::ResetGameEntity()
{
	transform.position = { 0.0f, 0.0f, 0.0f };
	transform.rotation = { 0.0f, 0.0f, 0.0f };
	transform.scale = { 1.0f, 1.0f, 1.0f };

	transform.foward = { 0.0f, 0.0f, 1.0f };
	transform.up = { 0.0f, 1.0f, 0.0f };
	transform.right = { 1.0f, 0.0f, 0.0f };

	rigidBody.velocity = { 0.0f, 0.0f, 0.0f };
	rigidBody.acceleration = { 0.0f, 0.0f, 0.0f };
	rigidBody.mass = 1.0f;
	rigidBody.maxSpeed = 1.0f;
	rigidBody.fricStrength = 2.0f;
	rigidBody.applyFriction = true;
	rigidBody.isMoving = false;

	DirectX::XMStoreFloat4x4(&renderingComponent.worldMat, DirectX::XMMatrixIdentity());
}

void GameEntity::SetWorld()
{
	DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(transform.rotation.y, transform.rotation.x, transform.rotation.z);
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);

	DirectX::XMMATRIX world = scaleMat * rotMat * posMat;

	world = DirectX::XMMatrixTranspose(world);

	DirectX::XMStoreFloat4x4(&renderingComponent.worldMat, world);
}

void GameEntity::Update(float dt)
{
	rigidBody.UpdateVelocity(&transform, dt);
	SetWorld();
}
