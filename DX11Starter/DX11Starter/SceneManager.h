#pragma once

class SceneManager
{
public :
	enum Scenes {
		_DefaultScene,
		_TetrisScene,
		_Scene2,
		_Scene,
		_Tyrian2000,
		_FrameRateTestScene,

		_GrassHubWorld,
		_WaterHubWorld,
		_SandHubWorld,
		_DryHubWorld,
		_BrickHubWorld,
		_StarHubWorld,
		_LavaHubWorld,
		_LavaGroundHubWorld,
		_CheckerHubWorld,
		_RainbowHubWorld,
		_MainHUBWorld
	};
	static void LoadScene(Scenes newScene);
};

