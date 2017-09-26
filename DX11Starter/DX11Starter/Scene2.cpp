#include "Scene2.h"

Scene2::Scene2(CosmicEngine* eng) :Game()
{
	engine = eng;
	cam = new Camera();
}


Scene2::~Scene2()
{

}

void Scene2::Init()
{
	// Creating game objects
	gameObjects.push_back(engine->CreateGameObject("Cube"));
	gameObjects.push_back(engine->CreateGameObject("Sphere"));
	//gameObjects.push_back(engine->CreateGameObject("RainbowRoad"));

	engine->rend->LoadSkyBox(2);
}
void Scene2::Update(float deltaTime, float totalTime)
{
	/////////////////////////
	//UPDATES
	/////////////////////////
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update(deltaTime);
	}
	float scale = std::sin(((totalTime / 10)*180.0f) / (2.0f*3.14f));

	CheckInputs(deltaTime);
}

void Scene2::CheckInputs(float deltaTime)
{
	if (engine->IsKeyDown(VK_LEFT))
	{
		//triangleObj->transform.Translate(-1.0f * deltaTime,0.0f,0.0f);
		gameObjects[0]->rigidBody.ApplyForce(-1.0f, 0.0f, 0.0f);
	}
	if (engine->IsKeyDown(VK_RIGHT))
	{
		//triangleObj->transform.Translate(1.0f * deltaTime, 0.0f, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(1.0f, 0.0f, 0.0f);
	}
	if (engine->IsKeyDown(VK_UP))
	{
		//triangleObj->transform.Translate(0.0f, 1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, 1.0f, 0.0f);
	}
	if (engine->IsKeyDown(VK_DOWN))
	{
		//triangleObj->transform.Translate(0.0f, -1.0f * deltaTime, 0.0f);
		gameObjects[0]->rigidBody.ApplyForce(0.0f, -1.0f, 0.0f);
	}
	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Update(deltaTime);
	}

	/////////////////////////
	//CAMERA MOVEMENT
	/////////////////////////
	if (engine->IsKeyDown(VK_LSHIFT)) //A
	{
		cam->camSpeed = cam->runSpeed;
	}
	else
	{
		cam->camSpeed = cam->normSpeed;
	}

	if (engine->IsKeyDown(65)) //A
	{
		cam->rigidBody.ApplyForce(cam->transform.right.x * -cam->camSpeed, cam->transform.right.y * -cam->camSpeed, cam->transform.right.z * -cam->camSpeed);
	}
	if (engine->IsKeyDown(68)) //D
	{
		cam->rigidBody.ApplyForce(cam->transform.right.x * cam->camSpeed, cam->transform.right.y * cam->camSpeed, cam->transform.right.z * cam->camSpeed);
	}
	if (engine->IsKeyDown(87)) //W
	{
		cam->rigidBody.ApplyForce(cam->transform.foward.x * cam->camSpeed, cam->transform.foward.y * cam->camSpeed, cam->transform.foward.z * cam->camSpeed);
	}
	if (engine->IsKeyDown(83)) //S
	{
		cam->rigidBody.ApplyForce(cam->transform.foward.x * -cam->camSpeed, -cam->transform.foward.y * -cam->camSpeed, cam->transform.foward.z * -cam->camSpeed);
	}
	if (engine->IsKeyDown(VK_SPACE))
	{
		cam->rigidBody.ApplyForce(0.0f, cam->camSpeed, 0.0f);
	}
	if (engine->IsKeyDown(88))
	{
		cam->rigidBody.ApplyForce(0.0f, -cam->camSpeed, 0.0f);
	}

	if (engine->IsKeyPressed(VK_RETURN) || engine->IsKeyPressed(69))
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
		SpawnGameObject("RayGun", spawnPoint, false);
	}
	if (engine->IsKeyDown(49))
	{
		engine->dayTime += 1.0f * deltaTime;
	}
	if (engine->IsKeyDown(50))
	{
		engine->dayTime -= 1.0f * deltaTime;
	}
	if (engine->IsKeyDown(96))
	{
		DefaultScene* defaultLevel = new DefaultScene(engine);
		engine->rend->LoadSkyBox(1);
		engine->LoadScene(defaultLevel);
	}
	else if (engine->IsKeyDown(97))
	{
		Scene* level1 = new Scene(engine);
		engine->LoadScene(level1);
	}
}

void Scene2::SpawnGameObject(std::string meshName, DirectX::XMFLOAT3 pos, bool canShoot)
{
	GameEntity* obj = engine->CreateGameObject(meshName);

	obj->renderingComponent.mat.surfaceColor = { (float)(std::rand() % 100) * 0.01f, (float)(std::rand() % 100)* 0.01f, (float)(std::rand() % 100) * 0.01f, 1.0f };
	obj->transform.position = pos;
	obj->transform.rotation = cam->transform.rotation;

	if (canShoot)
	{
		float bulletSpeed = 40000;
		//obj->rigidBody.applyFriction = false;
		obj->rigidBody.ApplyForce(cam->transform.foward.x * bulletSpeed, cam->transform.foward.y * bulletSpeed, cam->transform.foward.z * bulletSpeed);
	}

	gameObjects.push_back(obj);
	printf("\nNum of '%ss': %d", &meshName[0], engine->rend->meshStorage[meshName]->instances);
	//printf("\nColor - %f, %f, %f", obj->renderingComponent.mat.surfaceColor.x, obj->renderingComponent.mat.surfaceColor.y, obj->renderingComponent.mat.surfaceColor.z);

}