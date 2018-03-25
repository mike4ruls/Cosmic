#include "EngineManager.h"

EngineManager* myManager = 0;

EngineManager* EngineManager::GetInstance()
{
	if (myManager == NULL)
	{
		myManager = new EngineManager();
	}
	return myManager;
}

void EngineManager::SetUp(CosmicEngine* myEngine)
{
	myManager->curEngine = myEngine;
}
void EngineManager::DestroyInstance()
{
	if (myManager != nullptr) { delete myManager; myManager = nullptr; }
}

void EngineManager::RegistarGameObject(GameEntity& obj)
{
	curEngine->PushGameObject(&obj);
	/*curEngine->AttachRenderingComponent(obj.GetModel(), MatType::Opaque, pos);*/
}

void EngineManager::DeleteGameObject(GameEntity & obj)
{
	curEngine->DestroyGameObject(&obj);
}
void EngineManager::RegistarUI(UI& obj)
{
	curEngine->PushUI(&obj);
}

void EngineManager::DeleteUI(UI & obj)
{
	curEngine->DestroyUIObject(&obj);
}
void EngineManager::RegistarEmitter(Emitter& obj)
{
	curEngine->PushEmitter(&obj);
}

void EngineManager::DeleteEmitter(Emitter & obj)
{
	curEngine->DestroyEmitter(&obj);
}

ID3D11Device * EngineManager::GetDevice()
{
	return curEngine->GetDevice();
}

void EngineManager::LoadScene(Game * scene)
{
	curEngine->LoadScene(scene);
}

Renderer * EngineManager::GetRender()
{
	return curEngine->rend;
}

Mesh * EngineManager::GetMesh(char * name)
{
	Mesh* newMesh = curEngine->rend->assets->GetMesh(name);
	return newMesh;
}
