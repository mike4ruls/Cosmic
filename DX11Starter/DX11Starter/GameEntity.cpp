#include "GameEntity.h"
#include "EngineManager.h"


GameEntity::GameEntity()
{
	// Do i need to declare structs?
	transform = Transform();
	rigidBody = RigidBody();
	renderingComponent = new RenderingComponent();
	myMesh = new Mesh();
	name = "";
	renderingComponent->canRender = true;
	ResetGameEntity();
	isActive = true;
}

GameEntity::GameEntity(Mesh * m, bool ui)
{
	transform = Transform();
	rigidBody = RigidBody();
	renderingComponent = new RenderingComponent();
	renderingComponent->mat = Material();
	myMesh = m;
	name = "";
	renderingComponent->canRender = true;
	renderingComponent->mat.materialType = Material::Opaque;
	renderingComponent->mat.surfaceColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	renderingComponent->mat.surfaceReflectance = 0.0f;
	renderingComponent->meshName = myMesh->meshName;

	prevMatType = renderingComponent->mat.materialType;
	isTranslucent = false;
	isUI = ui;
	isActive = true;

	EngineManager* manager = EngineManager::GetInstance();
	manager->RegistarGameObject(*this);

	ResetGameEntity();
}

GameEntity::~GameEntity()
{
	if (renderingComponent != nullptr) { delete renderingComponent, renderingComponent = nullptr; };
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
	rigidBody.applyGravity = false;
	rigidBody.isMoving = false;

	rigidBody.myCollider.colliderType = Collider::ColliderType::SphereCollider;
	rigidBody.myCollider.center = {0.0f, 0.0f, 0.0f};
	rigidBody.myCollider.radius = 1.0f;

	DirectX::XMStoreFloat4x4(&renderingComponent->worldMat, DirectX::XMMatrixIdentity());
}

void GameEntity::SetWorld()
{
	DirectX::XMFLOAT3 determinate = { 1,1,1 };
	DirectX::XMVECTOR deter = DirectX::XMLoadFloat3(&determinate);

	DirectX::XMMATRIX posMat = DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw((transform.rotation.y * 3.1415f) / 180, (transform.rotation.x * 3.1415f) / 180, (transform.rotation.z * 3.1415f) / 180);
	//DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw((transform.rotation.y * 3.1415f) / 180, (transform.rotation.x * 3.1415f) / 180, (transform.rotation.z* 3.1415f) / 180);
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);

	DirectX::XMMATRIX world = scaleMat * rotMat * posMat;
	//DirectX::XMMATRIX world = posMat * scaleMat * rotMat;

	world = DirectX::XMMatrixTranspose(world);

	DirectX::XMMATRIX invTrans = DirectX::XMMatrixInverse(&deter, DirectX::XMMatrixTranspose(world));

	DirectX::XMStoreFloat4x4(&renderingComponent->worldMat, world);
	DirectX::XMStoreFloat4x4(&renderingComponent->worldInvTrans, invTrans);
}

void GameEntity::Update(float dt)
{
	if(isActive)
	{
		rigidBody.UpdateVelocity(&transform, dt);
		ChangeMatType();
		SetWorld();
	}
}

void GameEntity::ChangeMatType()
{
	if(prevMatType != renderingComponent->mat.materialType)
	{
		EngineManager* manager = EngineManager::GetInstance();
		if(renderingComponent->mat.materialType == Material::Transulcent)
		{
			manager->GetRender()->PushToTranslucent(renderingComponent);
			isTranslucent = true;
		}
		else if(isTranslucent)
		{
			manager->GetRender()->RemoveFromTranslucent(renderingComponent->mat.translucentID);
			isTranslucent = false;
		}
		prevMatType = renderingComponent->mat.materialType;
	}
}

void GameEntity::ToggleActive()
{
	isActive = isActive ? false : true;
}

void GameEntity::SetActive(bool act)
{
	isActive = act;
}

void GameEntity::ToggleVisibility()
{
	renderingComponent->canRender = renderingComponent->canRender ? false : true;
}

void GameEntity::SetVisibility(bool act)
{
	renderingComponent->canRender = act;
}

void GameEntity::Destroy()
{
	
	EngineManager* manager = EngineManager::GetInstance();
	manager->DeleteGameObject(*this);
}
