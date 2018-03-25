#include "SceneManager.h"
#include "EngineManager.h"

#include "DefaultScene.h"
#include "TetrisScene.h"
#include "Scene2.h"
#include "Scene.h"
#include "Tyrian2000.h"
#include "FrameRateTestScene.h"

#include "GrassHubWorld.h"
#include "WaterHubWorld.h"
#include "SandHubWorld.h"
#include "DryHubWorld.h"
#include "BrickHubWorld.h"
#include "StarHubWorld.h"
#include "LavaHubWorld.h"
#include "LavaGroundHubWorld.h"
#include "CheckerHubWorld.h"
#include "RainbowHubWorld.h"
#include "MainHUBWorld.h"


void SceneManager::LoadScene(Scenes newSceneEnum)
{
	EngineManager* manager = EngineManager::GetInstance();
	Game* newScene;
	switch (newSceneEnum)
	{
	case _DefaultScene:
		newScene = new DefaultScene();
		manager->LoadScene(newScene);
		break;

	case _TetrisScene:
		newScene = new TetrisScene();
		manager->LoadScene(newScene);
		break;


	case _Scene2:
		newScene = new Scene2();
		manager->LoadScene(newScene);
		break;


	case _Scene:
		newScene = new Scene();
		manager->LoadScene(newScene);
		break;


	case _Tyrian2000:
		newScene = new Tyrian2000();
		manager->LoadScene(newScene);
		break;


	case _FrameRateTestScene:
		newScene = new FrameRateTestScene();
		manager->LoadScene(newScene);
		break;



	case _GrassHubWorld:
		newScene = new GrassHubWorld();
		manager->LoadScene(newScene);
		break;


	case _WaterHubWorld:
		newScene = new WaterHubWorld();
		manager->LoadScene(newScene);
		break;


	case _SandHubWorld:
		newScene = new SandHubWorld();
		manager->LoadScene(newScene);
		break;


	case _DryHubWorld:
		newScene = new DryHubWorld();
		manager->LoadScene(newScene);
		break;


	case _BrickHubWorld:
		newScene = new BrickHubWorld();
		manager->LoadScene(newScene);
		break;


	case _StarHubWorld:
		newScene = new StarHubWorld();
		manager->LoadScene(newScene);
		break;


	case _LavaHubWorld:
		newScene = new LavaHubWorld();
		manager->LoadScene(newScene);
		break;


	case _LavaGroundHubWorld:
		newScene = new LavaGroundHubWorld();
		manager->LoadScene(newScene);
		break;


	case _CheckerHubWorld:
		newScene = new CheckerHubWorld();
		manager->LoadScene(newScene);
		break;


	case _RainbowHubWorld:
		newScene = new RainbowHubWorld();
		manager->LoadScene(newScene);
		break;


	case _MainHUBWorld:
		newScene = new MainHUBWorld();
		manager->LoadScene(newScene);
		break;
	}
	
}
