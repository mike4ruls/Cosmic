#pragma once
#include "Button.h"
#include "Image.h"
#include "Player.h"
#include "AssetManager.h"

class ShopMenu
{
private:
	struct MyGraph {
		int currentNum;
		bool bought;

		MyGraph* Up;
		MyGraph* Down;
		MyGraph* Left;
		MyGraph* Right;
		MyGraph(int num) {
			currentNum = num;
			bought = false;

			Up = nullptr;
			Down = nullptr;
			Left = nullptr;
			Right = nullptr;
		}
	};

public:
	AssetManager* assetManager;
	InputManager* inputManager;
	Player* p1;

	MyGraph* currentGraph;

	std::vector<MyGraph*> upgradeGraph;

	ShopMenu();
	ShopMenu(AssetManager* a, Player* player);
	~ShopMenu();

	void Init();
	void Update(float dt);
	void UpdateInputs(float dt);

	void CreateUpgradeGraph();
	void SetUpUpgradeUI();

	void Buy();

	void TurnOnStatsUI();
	void TurnOnShopUI();
	void TurnOnUpgradesUI();
	void TurnOnAbilitiesUI();

	void TurnOffStatsUI();
	void TurnOffShopUI();
	void TurnOffUpgradesUI();
	void TurnOffAbilitiesUI();

	void UpgradeGraphUp();
	void UpgradeGraphDown();
	void UpgradeGraphLeft();
	void UpgradeGraphRight();

	Image* yellowTriangle;
	Button* metalTab;
	Image* metalTopBar;
	Image* metalForeGround;
	Image* metalForeGround2;
	Image* metalBackGround;

	Button* statsButton;
	Button* shopButton;
	Button* upgradesButton;
	Button* abilitiesButton;

	Image* iconDisplayImage;

	Image* displayDividerImage;
	Image* displayDividerImage2;
	Image* displayLineDownImage;
	Image* displayLineDownImage2;

	Button* leftSubButton;
	Button* rightSubButton;
	Button* buyButton;

	Image* myShipImage;
	Button* healthUpButton;
	Button* defenseUpButton;

	Button* guidedMissleButton;

	Button* sheildCompButton;

	Button* frontBlasterButton;

	Button* leftBlasterButton;

	Button* rightBlasterButton;

	std::vector<UI*> shopUI;

	float UIMoveDownHieght;
	float UIMoveSpeed;

	int shopUIcount;
	int subUpgradecount;

	bool UITurnedOn;
};

