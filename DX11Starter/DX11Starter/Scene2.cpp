#include "Scene2.h"

Scene2::Scene2(CosmicEngine* eng) :Game()
{
	engine = eng;
	cam = new ThirdPersonCamera();
}


Scene2::~Scene2()
{

}

void Scene2::Init()
{
	inputManager = engine->inputManager;
	cam->inputManager = inputManager;
	ThirdPersonCamera* tempCam = (ThirdPersonCamera*)cam; 

	// Creating game objects
	gameObjects.push_back(engine->CreateGameObject("Cube"));
	gameObjects.push_back(engine->CreateGameObject("Sphere"));
	gameObjects.push_back(engine->CreateGameObject("Plane"));
	//gameObjects.push_back(engine->CreateGameObject("Dragon"));
	//gameObjects.push_back(engine->CreateCanvasElement());

	

	gameObjects[0]->transform.Translate(0.0f, 0.0f, 20.0f);
	gameObjects[0]->transform.Scale(50.0f, 50.0f, 0.3f);
	gameObjects[0]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("harambe"));

	gameObjects[1]->renderingComponent.mat.surfaceReflectance = 0.5f;
	tempCam->SetTarget(gameObjects[1]);
	tempCam->drag = false;
	tempCam->panaramicTranslation = {0.0f,0.0f,0.0f};

	gameObjects[2]->transform.Scale(40.0f);
	gameObjects[2]->transform.Translate(0.0f, -10.0f, 0.0f);
	gameObjects[2]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("brick"));

	//gameObjects[3]->transform.Scale(0.5f);
	/*gameObjects[3]->transform.Rotate(180.0f,-90.0f,0.0f);
	gameObjects[3]->renderingComponent.mat.LoadSurfaceTexture(engine->rend->assets->GetSurfaceTexture("dragonSur")); 
	gameObjects[3]->renderingComponent.mat.surfaceReflectance = 0.5f;*/


	pointLight = engine->rend->CreatePointLight({0,-5,0});
	pointLight->ligComponent->lightColor = {0.0f, 1.0f, 0.0f, 1.0f};

	engine->rend->LoadSkyBox(2);

	goldStars = engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("goldStar"),Emitter::BlendingType::CutOut);
	grayStars = engine->CreateParticalEmitter(100, engine->rend->assets->GetSurfaceTexture("grayStar"), Emitter::BlendingType::CutOut);
	blizzard = engine->CreateParticalEmitter(2000, engine->rend->assets->GetSurfaceTexture("snowFlake"), Emitter::BlendingType::CutOut);


	goldStars->emitterAcceleration = 2.0f;
	goldStars->transform.Rotate(0.0f, -90.0f, 0.0f);
	goldStars->accelerationDir = goldStars->transform.foward;
	goldStars->emissionRate = 0.7f;
	goldStars->lifeTime = 5.0f;
	goldStars->endRadius = 1.5f;
	goldStars->endSize = 2.0f;

	grayStars->emitterAcceleration = 2.0f;
	grayStars->transform.Rotate(0.0f, -90.0f, 0.0f);
	grayStars->accelerationDir = grayStars->transform.foward;
	grayStars->emissionRate = 0.4f;
	grayStars->lifeTime = 6.0f;
	grayStars->endRadius = 1.5f;

	blizzard->emitterAcceleration = 5.0f;
	blizzard->transform.Rotate(0.0f, 90.0f, 0.0f);
	blizzard->accelerationDir = blizzard->transform.foward;
	blizzard->emissionRate = 0.005f;
	blizzard->lifeTime = 10.0f;
	blizzard->endRadius = 50.0f;
	//blizzard->localSpace = true;
	blizzard->startSize = 1.0f;
	blizzard->endSize = 1.0f;

}
void Scene2::Update(float deltaTime, float totalTime)
{
	blizzard->transform.position = { cam->transform.position.x, cam->transform.position.y + 30.0f, cam->transform.position.z};
	//blizzard->transform.position = {cam->transform.position.x + (cam->transform.foward.x * 2.0f), cam->transform.position.y + 30.0f, cam->transform.position.z + (cam->transform.foward.z * 2.0f)};
	//blizzard->transform.position = { gameObjects[1]->transform.position.x, gameObjects[1]->transform.position.y + 30.0f, gameObjects[1]->transform.position.z};

	//printf("%f, %f, %f\n", cam->transform.foward.x, cam->transform.foward.y, cam->transform.foward.z);
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
	if (inputManager->IsKeyDown(VK_LEFT))
	{
		//triangleObj->transform.Translate(-1.0f * deltaTime,0.0f,0.0f);
		//gameObjects[0]->rigidBody.ApplyForce(-1.0f, 0.0f, 0.0f);

		pointLight->ligComponent->lightPos = DirectX::XMFLOAT3(pointLight->ligComponent->lightPos.x - (1.0f*deltaTime), pointLight->ligComponent->lightPos.y, pointLight->ligComponent->lightPos.z);
	}
	if (inputManager->IsKeyDown(VK_RIGHT))
	{
		//triangleObj->transform.Translate(1.0f * deltaTime, 0.0f, 0.0f);
		//gameObjects[0]->rigidBody.ApplyForce(1.0f, 0.0f, 0.0f);
		pointLight->ligComponent->lightPos = DirectX::XMFLOAT3(pointLight->ligComponent->lightPos.x + (1.0f*deltaTime), pointLight->ligComponent->lightPos.y, pointLight->ligComponent->lightPos.z);
	}
	if (inputManager->IsKeyDown(VK_UP))
	{
		//triangleObj->transform.Translate(0.0f, 1.0f * deltaTime, 0.0f);
		//gameObjects[0]->rigidBody.ApplyForce(0.0f, 1.0f, 0.0f);
		pointLight->ligComponent->lightPos = DirectX::XMFLOAT3(pointLight->ligComponent->lightPos.x, pointLight->ligComponent->lightPos.y + (1.0f*deltaTime), pointLight->ligComponent->lightPos.z);
	}
	if (inputManager->IsKeyDown(VK_DOWN))
	{
		//triangleObj->transform.Translate(0.0f, -1.0f * deltaTime, 0.0f);
		//gameObjects[0]->rigidBody.ApplyForce(0.0f, -1.0f, 0.0f);
		pointLight->ligComponent->lightPos = DirectX::XMFLOAT3(pointLight->ligComponent->lightPos.x, pointLight->ligComponent->lightPos.y, pointLight->ligComponent->lightPos.z - (1.0f*deltaTime));
		//pointLight->ligComponent->lightPos = DirectX::XMFLOAT3(pointLight->ligComponent->lightPos.x, pointLight->ligComponent->lightPos.y - (1.0f*deltaTime), 0);
	}
	for (unsigned int i = 0; i < gameObjects.size(); i++) {
		//gameObjects[i]->Update(deltaTime);
	}

	if (inputManager->IsKeyPressed(VK_RETURN) || inputManager->IsKeyPressed(69) || inputManager->IsButtonPressed(CosmicInput::BUTTON_R2))
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
	if (inputManager->IsKeyDown(49) || inputManager->IsButtonDown(CosmicInput::BUTTON_L1))
	{
		engine->dayTime += 1.0f * deltaTime;
	}
	if (inputManager->IsKeyDown(50) || inputManager->IsButtonDown(CosmicInput::BUTTON_R1))
	{
		engine->dayTime -= 1.0f * deltaTime;
	}
	if (inputManager->IsKeyDown(96))
	{
		DefaultScene* defaultLevel = new DefaultScene(engine);
		engine->rend->LoadSkyBox(1);
		engine->LoadScene(defaultLevel);
	}
	else if (inputManager->IsKeyDown(97))
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
	printf("\nNum of '%ss': %d", &meshName[0], engine->rend->assets->meshStorage[meshName]->instances);
	//printf("\nColor - %f, %f, %f", obj->renderingComponent.mat.surfaceColor.x, obj->renderingComponent.mat.surfaceColor.y, obj->renderingComponent.mat.surfaceColor.z);

}