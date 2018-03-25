#pragma once
#include "DefaultScene.h"
#include "FinishLine.h"
#include "TyrianGameManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "BackGroundTiles.h"
#include "Game.h"

class Tyrian2000 :
	public Game
{
public:
	enum Actions {
		ButtonUp,
		ButtonDown,
		ButtonLeft,
		ButtonRight,
		Fire,
		Strafe,
		Start
	};
	enum GameState {
		StartMenu,
		Game,
		Paused,
		EndLevel
	}currentState;

	Tyrian2000();
	Tyrian2000(char* tile);
	~Tyrian2000();
	TyrianGameManager* gameManager = 0;

	void Init();
	void SetUpLevel();
	void Update(float deltaTime, float totalTime);
	void UpdateParticlesPos();
	void CheckInputs(float dt);
	void CheckControllerInputs(float dt);
	void CheckOutOfBounds();
	void InitUI();
	void CreatePlayer();
	void SetUpParticles();
	void CreateFinishLine();
	void SpawnWaveEnemies();
	void SpawnWaveBlockers();
	void LoadBulletPool();
	void LoadBackgroundTilePool(std::string textureName);
	void CalculateCamPos();
	void KillEnemy(int pos);
	void SetUpActions();
	void ChooseEndPanelText();
	void TogglePauseMenu();
	void ResetLevel();
	void Quit();

	Player* p1 = nullptr;
	FinishLine* fLine = nullptr;

	Image* Tyrian2000Logo = nullptr;
	Image* pausedLogo = nullptr;
	Image* endGamePanel = nullptr;

	Button* startButton = nullptr;

	Button* resumeButton = nullptr;
	Button* retryButton = nullptr;
	Button* quitButton = nullptr;

	Button* endRetryButton = nullptr;
	Button* endContinueButton = nullptr;

	Emitter* shipExhaust;
	Emitter* leftWing;
	Emitter* rightWing;

	DirectX::XMFLOAT3 exhaustPos;
	DirectX::XMFLOAT3 leftWingPos;
	DirectX::XMFLOAT3 rightWingPos;

	std::vector<Enemy*> enemyPool;
	std::vector<BackGroundTiles*> backgroundTilePool;
	std::vector<Emitter*> explosionPool;
	std::vector<Emitter*> explosionStarPool;

	char* tileName = nullptr;

	// Game Values
	float xConstraint;
	float posZConstraint;
	float negZConstraint;

	float tileSize;
	float tileDistOffScreen;

	float moveDownHeight;
	float waveSpawnCD;
	float waveSpawnTimer;
	int waveCount;
	int currentWave;
	bool endGame;

	int pauseOption;
};

