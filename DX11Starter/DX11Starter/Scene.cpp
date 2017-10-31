#include "Scene.h"

Scene::Scene(CosmicEngine* eng):Game()
{
	engine = eng;
	cam = new Camera();
}


Scene::~Scene()
{
}

void Scene::Init()
{
	// Creating game objects
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;

	gameObjects.push_back(engine->CreateGameObject("Triangle"));

	
	gameObjects.push_back(engine->CreateGameObject("Teapot"));
	gameObjects.push_back(engine->CreateGameObject("Cube"));
	gameObjects.push_back(engine->CreateGameObject("Sphere"));
	gameObjects.push_back(engine->CreateGameObject("Triangle"));
	gameObjects.push_back(engine->CreateGameObject("RainbowRoad"));


	gameObjects[2]->transform.Translate(0, 3, 4);
	gameObjects[3]->transform.Translate(0, -3, 1);
	gameObjects[5]->transform.Scale(0.1f);
	gameObjects[5]->renderingComponent.mat.surfaceReflectance = 0.5f;

	gameObjects[2]->renderingComponent.mat.surfaceColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	gameObjects[1]->renderingComponent.mat.surfaceColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	engine->rend->LoadSkyBox(3);
}
void Scene::Update(float deltaTime, float totalTime)
{
	/////////////////////////
	//UPDATES
	/////////////////////////
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(deltaTime);
	}
	float scale = std::sin(((totalTime / 10)*180.0f) / (2.0f*3.14f));

	gameObjects[1]->transform.Rotate(0.0f, 0.0f, 30.0f * deltaTime);
	gameObjects[2]->transform.Rotate(10.0f * deltaTime, 30.0f * deltaTime, 30.0f * deltaTime);
	gameObjects[3]->transform.scale = { scale,scale,scale };

	CheckInputs(deltaTime);
}

void Scene::CheckInputs(float deltaTime)
{
	if (inputManager->IsKeyDown(VK_LEFT))
	{
		//triangleObj->transform.Translate(-1.0f * deltaTime,0.0f,0.0f);
		gameObjects[0]->rigidBody.ApplyForce(-1.0f, 0.0f, 0.0f);
	}
	if (inputManager->IsKeyDown(VK_RIGHT))
	{
		//triangleObj->transform.Translate(1.0f * deltaTime, 0.0f, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(1.0f, 0.0f, 0.0f);
	}
	if (inputManager->IsKeyDown(VK_UP))
	{
		//triangleObj->transform.Translate(0.0f, 1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, 1.0f, 0.0f);
	}
	if (inputManager->IsKeyDown(VK_DOWN))
	{
		//triangleObj->transform.Translate(0.0f, -1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, -1.0f, 0.0f);
	}
	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Update(deltaTime);
	}

	if (inputManager->IsKeyPressed(VK_RETURN) || inputManager->IsKeyPressed(69))
	{
		//"Triangle"
		//"Square"
		//"Sphere"
		//"Cube"
		//"Cone"
		//"Torus"
		//"Helix"
		//"RayGun"
		//"Plane"
		//"Quad"
		//"Teapot"
		//"HaloSword"

		float distInfront = 2.0f;
		DirectX::XMFLOAT3 spawnPoint = { cam->transform.position.x + (cam->transform.foward.x * distInfront), cam->transform.position.y + (cam->transform.foward.y * distInfront), cam->transform.position.z + (cam->transform.foward.z * distInfront) };
		SpawnGameObject("Cube", spawnPoint, true);
	}
	if (inputManager->IsKeyDown(49))
	{
		engine->dayTime += 1.0f * deltaTime;
	}
	if (inputManager->IsKeyDown(50))
	{
		engine->dayTime -= 1.0f * deltaTime;
	}
	if (inputManager->IsKeyPressed(96))
	{
		DefaultScene* defaultLevel = new DefaultScene(engine);
		engine->rend->LoadSkyBox(1);
		engine->LoadScene(defaultLevel);
	}
	else if (inputManager->IsKeyPressed(98))
	{
		Scene2* level2 = new Scene2(engine);
		engine->LoadScene(level2);
	}
}

void Scene::SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot)
{
	GameEntity* obj = engine->CreateGameObject(meshName);

	obj->renderingComponent.mat.surfaceColor = { (float)(std::rand() % 100) * 0.01f, (float)(std::rand() % 100)* 0.01f, (float)(std::rand() % 100) * 0.01f, 1.0f };
	obj->transform.position = pos;
	obj->transform.rotation = cam->transform.rotation;
	printf("\nRot - %f, %f, %f", cam->transform.rotation.x, cam->transform.rotation.y, cam->transform.rotation.z);

	if (canShoot)
	{
		float bulletSpeed = 40000;
		//obj->rigidBody.applyFriction = false;
		obj->rigidBody.ApplyForce(cam->transform.foward.x * bulletSpeed, cam->transform.foward.y * bulletSpeed, cam->transform.foward.z * bulletSpeed);
	}

	gameObjects.push_back(obj);
	printf("\nNum of '%ss': %d", &meshName[0], engine->rend->assets->meshStorage[meshName]->instances);
	//printf("\nColor - %f, %f, %f", obj->renderingComponent.mat.surfaceColor.x, obj->renderingComponent.mat.surfaceColor.y, obj->renderingComponent.mat.surfaceColor.z);

}


