#pragma once
#include "Mesh.h"
#include "Renderer.h"
#include "Transform.h"
#include "RigidBody.h"
#include "RenderingComponent.h"

class GameEntity
{
public:
	GameEntity();
	GameEntity(Mesh* m, Renderer* r, bool ui);
	~GameEntity();

	std::string name;
	Mesh* myMesh;
	Renderer* rend;
	Transform transform;
	RigidBody rigidBody;
	RenderingComponent renderingComponent;

	void ResetGameEntity();
	void SetWorld();
	void Update(float dt);
	void CheckMatType();

	bool isUI;

private:
	Material::MatType prevMatType;
	bool isTranslucent;
};

