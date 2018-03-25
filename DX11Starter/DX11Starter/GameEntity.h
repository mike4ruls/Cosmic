#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "RigidBody.h"
#include "renderingComponent.h"

class GameEntity
{
public:
	GameEntity();
	GameEntity(Mesh * m, bool ui);
	~GameEntity();

	std::string name;
	Mesh* myMesh;
	Transform transform;
	RigidBody rigidBody;
	RenderingComponent* renderingComponent;

	void ResetGameEntity();
	void SetWorld();
	void Update(float dt);
	void ChangeMatType();

	void ToggleActive();
	void SetActive(bool act);

	void ToggleVisibility();
	void SetVisibility(bool act);

	void Destroy();

	unsigned int Id;

	bool isActive;
	bool isUI;

private:
	Material::MatType prevMatType;
	bool isTranslucent;
};

