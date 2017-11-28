#pragma once
#include "CosmicEngine.h"
#include "DefaultScene.h"
#include "Game.h"
#include "Bullet.h"
#include "Player.h"
class MainHUBWorld :
	public Game
{
public:
	enum Actions {
		ButtonUp,
		ButtonDown,
		ButtonLeft,
		ButtonRight,
		LB,
		RB,
		Fire,
		Strafe,
		Start,
		Select
	};
	enum GameState {
		StartMenu,
		Game,
		Paused,
		EndLevel
	}currentState;

	MainHUBWorld(CosmicEngine* eng);
	~MainHUBWorld();
	CosmicEngine* engine;

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
	void LoadBulletPool();
	void Shoot();
	void CalculateCamPos(float dt);
	void SetUpActions();

	void TurnOnStatsUI();
	void TurnOnShopUI();
	void TurnOnUpgradesUI();
	void TurnOnAbilitiesUI();

	void TurnOffStatsUI();
	void TurnOffShopUI();
	void TurnOffUpgradesUI();
	void TurnOffAbilitiesUI();

	Player* p1 = nullptr;

	Image* healthBar = nullptr;
	Image* healthBarFade = nullptr;
	Image* healthBarBack = nullptr;
	Image* healthBarBorder = nullptr;

	Image* yellowTriangle;
	Button* metalTab;
	Image* metalTopBar;
	Image* metalForeGround;
	Image* metalBackGround;

	Button* statsButton;
	Button* shopButton;
	Button* upgradesButton;
	Button* abilitiesButton;

	Emitter* shipExhaust;
	Emitter* leftWing;
	Emitter* rightWing;

	DirectX::XMFLOAT3 exhaustPos;
	DirectX::XMFLOAT3 leftWingPos;
	DirectX::XMFLOAT3 rightWingPos;

	DirectX::XMFLOAT3 curCamPos;

	std::vector<GameEntity*> worlds;
	std::vector<Bullet*> bulletPool;
	std::vector<Emitter*> goldStarPool;
	std::vector<Emitter*> grayStarPool;

	std::vector<UI*> shopUI;

	float posXConstraint;
	float negXConstraint;
	float posZConstraint;
	float negZConstraint;

	float xCamConstraint;
	float posZCamConstraint;
	float negZCamConstraint;

	float moveDownHeight;
	float camSpeed;
	float worldRotSpeed;

	float UIMoveDownHieght;
	float UIMoveSpeed;

	int shopUIcount;

	bool UITurnedOn;
};

