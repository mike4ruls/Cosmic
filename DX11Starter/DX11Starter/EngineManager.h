#pragma once
#include "CosmicEngine.h"


 class EngineManager{
 private:
	 CosmicEngine* curEngine;

	 EngineManager()
	 {

	 }
	 ~EngineManager()
	 {

	 }

 public:
	 static EngineManager* GetInstance();

	 static void SetUp(CosmicEngine* myEngine);

	 static void DestroyInstance();

	 void RegistarGameObject(GameEntity& obj);
	 void DeleteGameObject(GameEntity& obj);
	 void RegistarUI(UI& obj);
	 void DeleteUI(UI& obj);
	 void RegistarEmitter(Emitter& obj);
	 void DeleteEmitter(Emitter& obj);
	 ID3D11Device* GetDevice();
	 void LoadScene(Game* scene);
	 Renderer* GetRender();

	 Mesh* GetMesh(char* name);
	 
 };
