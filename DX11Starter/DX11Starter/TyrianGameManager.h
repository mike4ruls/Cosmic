#pragma once
#include "Player.h"
enum LevelStatus {
	Locked,
	Unlocked,
	Completed
};

enum Worlds {
	Grass,
	Water,
	Sand,
	Dry,
	Brick,
	Star,
	Lava,
	LavaGround,
	Checker,
	Rainbow
};

class TyrianGameManager
{
public:
	TyrianGameManager();
	~TyrianGameManager();

	static TyrianGameManager* GetInstance();
	static void Release();
	static void LoadLevel(int levelind);
	static void LoadWorld();
	static void LoadHubWorld();

	static void LoadPlayer(Player* p1);
	static void SavePlayer(Player* p1);

	static void SetTileLoadName(char* name);
	static int* GetActiveLevels(int index);
	static void SetActiveLevels(int index, int val);
	static int GetActiveWorld(int index);
	static void CompleteLevels();
	static void StoreLevelPosistion(DirectX::XMFLOAT3 pos);
	static void StoreWorldPosistion(DirectX::XMFLOAT3 pos);
	static DirectX::XMFLOAT3 GetLevelPosistion();
	static DirectX::XMFLOAT3 GetWorldPosistion();

private:

};

