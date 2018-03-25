#include "TyrianGameManager.h"
#include "Tyrian2000.h"
#include "SceneManager.h"
#include "EngineManager.h"

TyrianGameManager* instance = 0;
Player p1Store;

std::string tileName;
Worlds curWorld;
float numCheck;
int curLevel;

DirectX::XMFLOAT3 playerWorldPos = {0.0f, 4.0f - 30.0f, -1.0f};
DirectX::XMFLOAT3 playerLevelPos = { 0.0f,  4.0f - 30.0f, -1.0f };

int levelCount[10] = {4,4,4,4,4,4,4,4,4,4};

std::vector<int*> worldsLevelStorage;

int WorldLevels[10] = {};

bool closingProgram = false;

TyrianGameManager::TyrianGameManager()
{
	worldsLevelStorage.resize(10);
	for (int i = 0; i < 10; i++)
	{
		worldsLevelStorage[i] = new int[levelCount[i]]{};
		worldsLevelStorage[i][0] = 1;
	}
	p1Store = Player();
	numCheck = p1Store.frontAtkDamage;
	curWorld = Worlds::Grass;
}


TyrianGameManager::~TyrianGameManager()
{
	
}

TyrianGameManager* TyrianGameManager::GetInstance()
{
	if (instance == 0) 
	{
		instance = new TyrianGameManager();
		closingProgram = false;
		//p1Store->isDead = true;
	}
	return instance;
}

void TyrianGameManager::Release()
{
	for (int i = 0; i < 10; i++)
	{
		delete worldsLevelStorage[i];
		WorldLevels[i] = 0;
	}
	closingProgram = true;
	if (instance != 0) { delete instance; instance = 0; };
	playerWorldPos = { 0.0f, 4.0f - 30.0f, -1.0f };
	playerLevelPos = { 0.0f,  4.0f - 30.0f, -1.0f };
	//if (instance != 0) { delete instance; instance = nullptr; };
}

void TyrianGameManager::LoadLevel(int levelind)
{
	EngineManager* manager = EngineManager::GetInstance();

	curLevel = levelind;
	Tyrian2000* tyrian;

	switch (curWorld) {
	case Worlds::Grass:
		tyrian = new Tyrian2000("grass");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Water:
		tyrian = new Tyrian2000("water");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Brick:
		tyrian = new Tyrian2000("brick");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Checker:
		tyrian = new Tyrian2000("checker");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Dry:
		tyrian = new Tyrian2000("dry");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Rainbow:
		tyrian = new Tyrian2000("rainbow");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Lava:
		tyrian = new Tyrian2000("lava");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Sand:
		tyrian = new Tyrian2000("sand");
		manager->LoadScene(tyrian);
		break;
	case Worlds::LavaGround:
		tyrian = new Tyrian2000("lavaGround");
		manager->LoadScene(tyrian);
		break;
	case Worlds::Star:
		tyrian = new Tyrian2000("star");
		manager->LoadScene(tyrian);
		break;
	}
}

void TyrianGameManager::LoadWorld()
{
	if (tileName == "grass") {
		SceneManager::LoadScene(SceneManager::_GrassHubWorld);
		return;
	}
	else if (tileName == "water") {
		SceneManager::LoadScene(SceneManager::_WaterHubWorld);
		return;
	}
	else if (tileName == "brick") {
		SceneManager::LoadScene(SceneManager::_BrickHubWorld);
		return;
	}
	else if (tileName == "checker") {
		SceneManager::LoadScene(SceneManager::_CheckerHubWorld);
		return;
	}
	else if (tileName == "dry") {
		SceneManager::LoadScene(SceneManager::_DryHubWorld);
		return;
	}
	else if (tileName == "rainbow") {
		SceneManager::LoadScene(SceneManager::_RainbowHubWorld);
		return;
	}
	else if (tileName == "lava") {
		SceneManager::LoadScene(SceneManager::_LavaHubWorld);
		return;
	}
	else if (tileName == "sand") {
		SceneManager::LoadScene(SceneManager::_SandHubWorld);
		return;
	}
	else if (tileName == "lavaGround") {
		SceneManager::LoadScene(SceneManager::_LavaGroundHubWorld);
		return;
	}
	else if (tileName == "star") {
		SceneManager::LoadScene(SceneManager::_StarHubWorld);
		return;
	}
}

void TyrianGameManager::LoadHubWorld()
{
	playerLevelPos = {0.0f, playerLevelPos.y, 0.0f};
	SceneManager::LoadScene(SceneManager::_MainHUBWorld);
}

void TyrianGameManager::LoadPlayer(Player * p1)
{
	if(p1Store.frontAtkDamage != numCheck)
	{
		p1->maxHealth = p1Store.maxHealth;
		p1->health = p1Store.health;
		p1->topDisplayHealth = p1->health;
		p1->botDisplayHealth = p1->health;

		p1->maxSheild = p1Store.maxSheild;
		p1->sheild = p1Store.sheild;
		p1->topDisplaySheild = p1->sheild;
		p1->botDisplaySheild = p1->sheild;

		p1->frontAtkSpeed = p1Store.frontAtkSpeed;
		p1->frontAtkDamage = p1Store.frontAtkDamage;
		p1->frontTimer = p1->frontAtkSpeed;

		p1->leftAtkSpeed = p1Store.leftAtkSpeed;
		p1->leftAtkDamage = p1Store.leftAtkDamage;
		p1->leftTimer = p1->leftAtkSpeed;

		p1->rightAtkSpeed = p1Store.rightAtkSpeed;
		p1->rightAtkDamage = p1Store.rightAtkDamage;
		p1->rightTimer = p1->rightAtkSpeed;

		p1->guidedMissleMax = p1Store.guidedMissleMax;
		p1->guidedMissleRechargeRate = p1Store.guidedMissleRechargeRate;

		p1->leftBlasterBought = p1Store.leftBlasterBought;
		p1->rightBlasterBought = p1Store.rightBlasterBought;
		p1->guidedMissleBought = p1Store.guidedMissleBought;
		p1->sheildComponentBought = p1Store.sheildComponentBought;

		p1->leftBlasterBroken = p1Store.leftBlasterBroken;
		p1->rightBlasterBroken = p1Store.rightBlasterBroken;
		p1->guidedMissleBroken = p1Store.guidedMissleBroken;
		p1->sheildComponentBroken = p1Store.sheildComponentBroken;

		p1->curLevel = p1Store.curLevel;
		p1->currency = p1Store.currency;
	}
}

void TyrianGameManager::SavePlayer(Player * p1)
{
	if(!closingProgram)
	{
		p1Store.maxHealth = p1->maxHealth;
		p1Store.health = p1->health;
		
		p1Store.maxSheild = p1->maxSheild;
		p1Store.sheild = p1->sheild;
		
		p1Store.frontAtkSpeed = p1->frontAtkSpeed;
		p1Store.frontAtkDamage = p1->frontAtkDamage;
		
		p1Store.leftAtkSpeed = p1->leftAtkSpeed;
		p1Store.leftAtkDamage = p1->leftAtkDamage;
		
		p1Store.rightAtkSpeed = p1->rightAtkSpeed;
		p1Store.rightAtkDamage = p1->rightAtkDamage;
		
		p1Store.guidedMissleMax = p1->guidedMissleMax;
		p1Store.guidedMissleRechargeRate = p1->guidedMissleRechargeRate;
		
		p1Store.leftBlasterBought = p1->leftBlasterBought;
		p1Store.rightBlasterBought = p1->rightBlasterBought;
		p1Store.guidedMissleBought = p1->guidedMissleBought;
		p1Store.sheildComponentBought = p1->sheildComponentBought;
		
		p1Store.leftBlasterBroken = p1->leftBlasterBroken;
		p1Store.rightBlasterBroken = p1->rightBlasterBroken;
		p1Store.guidedMissleBroken = p1->guidedMissleBroken;
		p1Store.sheildComponentBroken = p1->sheildComponentBroken;
		
		p1Store.curLevel = p1Store.curLevel;
		p1Store.currency = p1Store.currency;
	}
}

void TyrianGameManager::SetTileLoadName(char * name)
{
	tileName = name;

	if (tileName == "grass") {
		curWorld = Worlds::Grass;
	}
	else if (tileName == "water") {
		curWorld = Worlds::Water;
	}
	else if (tileName == "brick") {
		curWorld = Worlds::Brick;
	}
	else if (tileName == "checker") {
		curWorld = Worlds::Checker;
	}
	else if (tileName == "dry") {
		curWorld = Worlds::Dry;
	}
	else if (tileName == "rainbow") {
		curWorld = Worlds::Rainbow;
	}
	else if (tileName == "lava") {
		curWorld = Worlds::Lava;
	}
	else if (tileName == "sand") {
		curWorld = Worlds::Sand;
	}
	else if (tileName == "lavaGround") {
		curWorld = Worlds::LavaGround;
	}
	else if (tileName == "star") {
		curWorld = Worlds::Star;
	}
}

int * TyrianGameManager::GetActiveLevels(int index)
{
	return worldsLevelStorage[index];
}

void TyrianGameManager::SetActiveLevels(int index, int val)
{
	WorldLevels[index] = val;
}

int TyrianGameManager::GetActiveWorld(int index)
{
	return WorldLevels[index];
}

void TyrianGameManager::CompleteLevels()
{
	GetActiveLevels(curWorld)[curLevel] = 2;
	if(curLevel < levelCount[curWorld] - 1)
	{
		GetActiveLevels(curWorld)[curLevel + 1] = 1;
	}
	else if(WorldLevels[curWorld] == 0)
	{
		WorldLevels[curWorld] += 1;
		LoadHubWorld();
		return;
	}

	LoadWorld();
}

void TyrianGameManager::StoreLevelPosistion(DirectX::XMFLOAT3 pos)
{
	playerLevelPos = pos;
}

void TyrianGameManager::StoreWorldPosistion(DirectX::XMFLOAT3 pos)
{
	playerWorldPos = pos;
}

DirectX::XMFLOAT3 TyrianGameManager::GetLevelPosistion()
{
	return playerLevelPos;
}

DirectX::XMFLOAT3 TyrianGameManager::GetWorldPosistion()
{
	return playerWorldPos;
}
