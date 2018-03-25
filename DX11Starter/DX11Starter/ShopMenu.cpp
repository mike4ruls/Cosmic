#include "ShopMenu.h"



ShopMenu::ShopMenu()
{

}

ShopMenu::ShopMenu(AssetManager* a, Player* player)
{
	assetManager = a;
	inputManager = InputManager::GetInstance();
	p1 = player;
	shopUIcount = 0;
	subUpgradecount = 0;

	Init();
	CreateUpgradeGraph();
}


ShopMenu::~ShopMenu()
{
	for (unsigned int i = 0; i < upgradeGraph.size(); i++)
	{
		if (upgradeGraph[i] != nullptr) { delete upgradeGraph[i]; upgradeGraph[i] = nullptr; }
	}
}

void ShopMenu::Init()
{
	UIMoveDownHieght = -2.510f;

	yellowTriangle = new Image();
	yellowTriangle->SetAlignment(UI::Bottom);
	yellowTriangle->SetUIColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	yellowTriangle->SetWidth(0.12f);
	yellowTriangle->SetHeight(0.04f);
	//metalTab->posY = -1.149f;
	yellowTriangle->posY = -0.213f;

	shopUI.push_back(yellowTriangle);

	metalTab = new Button();
	metalTab->SetAlignment(UI::Bottom);
	metalTab->LoadTexture(assetManager->GetSurfaceTexture("metalTab"));
	metalTab->SetWidth(0.18f);
	metalTab->SetHeight(0.08f);
	//metalTab->posY = -1.149f;
	metalTab->posY = -0.259f;

	shopUI.push_back(metalTab);

	/*metalTopBar = new Image();
	metalTopBar->LoadTexture(assetManager->GetSurfaceTexture("metalBack"));
	metalTopBar->SetAlignment(UI::Bottom);
	metalTopBar->obj->renderingComponent->mat.uvXOffSet = 0.1f;
	metalTopBar->obj->renderingComponent->mat.uvYOffSet = 1.0f;
	metalTopBar->SetWidth(0.04f);
	metalTopBar->SetHeight(0.755f);
	metalTopBar->posX = 0.501f;
	metalTopBar->posY = 0.1352f;*/

	//shopUI.push_back(metalTopBar);

	float buttonHieght = -0.584f;

	statsButton = new Button();
	statsButton->LoadTexture(assetManager->GetSurfaceTexture("statsText"));
	statsButton->SetAlignment(UI::Bottom);
	statsButton->SetWidth(0.525f);
	statsButton->SetHeight(0.1f);
	statsButton->posX = -1.569f;
	statsButton->posY = buttonHieght;

	shopUI.push_back(statsButton);

	shopButton = new Button();
	shopButton->LoadTexture(assetManager->GetSurfaceTexture("shopText"));
	shopButton->SetAlignment(UI::Bottom);
	shopButton->SetWidth(0.525f);
	shopButton->SetHeight(0.1f);
	shopButton->posX = -0.516f;
	shopButton->posY = buttonHieght;

	shopUI.push_back(shopButton);

	upgradesButton = new Button();
	upgradesButton->LoadTexture(assetManager->GetSurfaceTexture("upgradesText"));
	upgradesButton->SetAlignment(UI::Bottom);
	upgradesButton->SetWidth(0.525f);
	upgradesButton->SetHeight(0.1f);
	upgradesButton->posX = 0.536f;
	upgradesButton->posY = buttonHieght;

	shopUI.push_back(upgradesButton);

	abilitiesButton = new Button();
	abilitiesButton->LoadTexture(assetManager->GetSurfaceTexture("abilitiesText"));
	abilitiesButton->SetAlignment(UI::Bottom);
	abilitiesButton->SetWidth(0.525f);
	abilitiesButton->SetHeight(0.1f);
	abilitiesButton->posX = 1.57f;
	abilitiesButton->posY = buttonHieght;

	shopUI.push_back(abilitiesButton);

	healthUpButton = new Button();
	healthUpButton->LoadTexture(assetManager->GetSurfaceTexture("healthUpIcon"));
	healthUpButton->SetUIColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	healthUpButton->SetAlignment(UI::Bottom);
	healthUpButton->SetWidth(0.2f);
	healthUpButton->SetHeight(0.2f);
	healthUpButton->posX = -1.588f;
	healthUpButton->posY = -2.3f + 1.094f;

	shopUI.push_back(healthUpButton);

	defenseUpButton = new Button();
	defenseUpButton->LoadTexture(assetManager->GetSurfaceTexture("defenseUpIcon"));
	defenseUpButton->SetUIColor({ 0.0f, 1.0f, 1.0f, 1.0f });
	defenseUpButton->SetAlignment(UI::Bottom);
	defenseUpButton->SetWidth(0.2f);
	defenseUpButton->SetHeight(0.2f);
	defenseUpButton->posX = -0.139f;
	defenseUpButton->posY = -2.3f + 1.094f;

	shopUI.push_back(defenseUpButton);

	displayDividerImage = new Image();
	displayDividerImage->SetUIColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	//displayDividerImage->SetUIColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	displayDividerImage->SetAlignment(UI::Bottom);
	displayDividerImage->SetWidth(0.777f);
	displayDividerImage->SetHeight(0.015f);
	displayDividerImage->posX = 1.317f;
	//displayDividerImage->posY = 1.309f;
	displayDividerImage->posY = -2.3f + 0.336f;

	shopUI.push_back(displayDividerImage);

	displayDividerImage2 = new Image();
	displayDividerImage2->SetUIColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	//displayDividerImage->SetUIColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	displayDividerImage2->SetAlignment(UI::Bottom);
	displayDividerImage2->SetWidth(0.777f);
	displayDividerImage2->SetHeight(0.015f);
	displayDividerImage2->posX = 1.317f;
	//displayDividerImage->posY = 1.309f;
	displayDividerImage2->posY = -2.3f + 1.291f;

	shopUI.push_back(displayDividerImage2);

	iconDisplayImage = new Image();
	iconDisplayImage->LoadTexture(assetManager->GetSurfaceTexture("sheildComponentIcon"));
	//iconDisplayImage->SetUIColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	iconDisplayImage->SetAlignment(UI::Bottom);
	iconDisplayImage->SetWidth(0.15f);
	iconDisplayImage->SetHeight(0.15f);
	iconDisplayImage->posX = 0.692f;
	iconDisplayImage->posY = -2.3f + 1.32f;

	shopUI.push_back(iconDisplayImage);

	leftSubButton = new Button();
	leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("sheildPowerIcon"));
	//leftSubButton->SetUIColor({ 0.0f, 1.0f, 1.0f, 1.0f });
	leftSubButton->SetAlignment(UI::Bottom);
	leftSubButton->SetWidth(0.08f);
	leftSubButton->SetHeight(0.08f);
	leftSubButton->posX = iconDisplayImage->posX;
	leftSubButton->posY = iconDisplayImage->posY - 0.288f;

	shopUI.push_back(leftSubButton);

	rightSubButton = new Button();
	rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("sheildRechargeIcon"));
	//rightSubButton->SetUIColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	rightSubButton->SetAlignment(UI::Bottom);
	rightSubButton->SetWidth(0.08f);
	rightSubButton->SetHeight(0.08f);
	rightSubButton->posX = iconDisplayImage->posX;
	rightSubButton->posY = iconDisplayImage->posY - 0.533f;

	shopUI.push_back(rightSubButton);

	displayLineDownImage = new Image();
	displayLineDownImage->SetUIColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	displayLineDownImage->SetAlignment(UI::Bottom);
	displayLineDownImage->SetWidth(0.015f);
	displayLineDownImage->SetHeight(0.177f);
	displayLineDownImage->posX = iconDisplayImage->posX;
	displayLineDownImage->posY = iconDisplayImage->posY - 0.39f;

	shopUI.push_back(displayLineDownImage);

	displayLineDownImage2 = new Image();
	displayLineDownImage2->SetUIColor({ 0.3f, 0.3f, 0.3f, 1.0f });
	displayLineDownImage2->SetAlignment(UI::Bottom);
	displayLineDownImage2->SetWidth(0.015f);
	displayLineDownImage2->SetHeight(0.465f);
	displayLineDownImage2->posX = 0.856f;
	displayLineDownImage2->posY = -2.3f + 0.36f;

	shopUI.push_back(displayLineDownImage2);

	buyButton = new Button();
	buyButton->LoadTexture(assetManager->GetSurfaceTexture("buyText"));
	buyButton->SetAlignment(UI::Bottom);
	buyButton->SetWidth(0.304f);
	buyButton->SetHeight(0.1f);
	buyButton->posX = 1.791f;
	buyButton->posY = UIMoveDownHieght + 0.336f;

	shopUI.push_back(buyButton);

	// Guided Missle
	float shipPos = -2.3f + 0.420f;

	guidedMissleButton = new Button();
	guidedMissleButton->LoadTexture(assetManager->GetSurfaceTexture("guidedMissleIcon"));
	guidedMissleButton->SetUIColor({ 0.0f, 1.0f, 0.4f, 1.0f });
	guidedMissleButton->SetAlignment(UI::Bottom);
	guidedMissleButton->SetWidth(0.15f);
	guidedMissleButton->SetHeight(0.15f);
	guidedMissleButton->posX = -1.788f;
	guidedMissleButton->posY = shipPos + 0.824f;

	shopUI.push_back(guidedMissleButton);

	// Sheild Component

	sheildCompButton = new Button();
	sheildCompButton->LoadTexture(assetManager->GetSurfaceTexture("sheildComponentIcon"));
	sheildCompButton->SetUIColor({ 0.0f, 1.0f, 1.0f, 1.0f });
	sheildCompButton->SetAlignment(UI::Bottom);
	sheildCompButton->SetWidth(0.15f);
	sheildCompButton->SetHeight(0.15f);
	sheildCompButton->posX = 0.16f;
	sheildCompButton->posY = guidedMissleButton->posY;

	shopUI.push_back(sheildCompButton);

	// Front Blaster
	frontBlasterButton = new Button();
	frontBlasterButton->LoadTexture(assetManager->GetSurfaceTexture("cannonIcon"));
	frontBlasterButton->SetUIColor({ 1.0f, 0.0f, 0.3f, 1.0f });
	frontBlasterButton->obj->transform.Rotate(0.0f, 0.0f, 135.0f);
	frontBlasterButton->SetAlignment(UI::Bottom);
	frontBlasterButton->SetWidth(0.11f);
	frontBlasterButton->SetHeight(0.11f);
	frontBlasterButton->posX = -0.844f;
	frontBlasterButton->posY = shipPos + 0.804f;

	shopUI.push_back(frontBlasterButton);

	// Left Blaster
	leftBlasterButton = new Button();
	leftBlasterButton->LoadTexture(assetManager->GetSurfaceTexture("cannonIcon"));
	leftBlasterButton->SetUIColor({ 1.0f, 0.0f, 0.3f, 1.0f });
	leftBlasterButton->obj->transform.Rotate(0.0f, 0.0f, 135.0f);
	leftBlasterButton->SetAlignment(UI::Bottom);
	leftBlasterButton->SetWidth(0.11f);
	leftBlasterButton->SetHeight(0.11f);
	leftBlasterButton->posX = -1.307f;
	leftBlasterButton->posY = shipPos - 0.104f;

	shopUI.push_back(leftBlasterButton);

	// Right Blaster
	rightBlasterButton = new Button();
	rightBlasterButton->LoadTexture(assetManager->GetSurfaceTexture("cannonIcon"));
	rightBlasterButton->SetUIColor({ 1.0f, 0.0f, 0.3f, 1.0f });
	rightBlasterButton->obj->transform.Rotate(0.0f, 0.0f, 135.0f);
	rightBlasterButton->SetAlignment(UI::Bottom);
	rightBlasterButton->SetWidth(0.11f);
	rightBlasterButton->SetHeight(0.11f);
	rightBlasterButton->posX = -0.4f;
	rightBlasterButton->posY = leftBlasterButton->posY;

	shopUI.push_back(rightBlasterButton);

	myShipImage = new Image();
	myShipImage->LoadTexture(assetManager->GetSurfaceTexture("jetFighterIcon"));
	myShipImage->SetUIColor({ 1.0f, 0.3f, 0.0f, 1.0f });
	myShipImage->SetAlignment(UI::Bottom);
	myShipImage->obj->transform.Rotate(0.0f, 0.0f, 45.0f);
	//myShipImage->SetUIColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	myShipImage->SetWidth(0.3f);
	myShipImage->SetHeight(0.3f);
	myShipImage->posX = -0.844f;
	myShipImage->posY = shipPos;

	shopUI.push_back(myShipImage);

	metalForeGround = new Image();
	//metalForeGround->SetUIColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	metalForeGround->SetUIColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	metalForeGround->SetWidth(1.277f);
	metalForeGround->SetHeight(0.755f);
	metalForeGround->posY = UIMoveDownHieght + 0.088f;
	metalForeGround->posX = -0.813f;

	shopUI.push_back(metalForeGround);

	metalForeGround2 = new Image();
	//metalForeGround2->SetUIColor({ 0.05f, 0.05f, 0.05f, 1.0f });
	metalForeGround2->SetUIColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	metalForeGround2->SetWidth(0.777f);
	metalForeGround2->SetHeight(0.755f);
	metalForeGround2->posY = UIMoveDownHieght + 0.088f;
	metalForeGround2->posX = 1.318f;

	shopUI.push_back(metalForeGround2);

	metalBackGround = new Image();
	metalBackGround->LoadTexture(assetManager->GetSurfaceTexture("metalBack"));
	metalBackGround->SetWidth(2.22f);
	metalBackGround->SetHeight(1.25f);
	metalBackGround->obj->renderingComponent->mat.uvXOffSet = 3.5f;
	metalBackGround->obj->renderingComponent->mat.uvYOffSet = 3.0f;
	metalBackGround->posY = UIMoveDownHieght;

	shopUI.push_back(metalBackGround);


	UIMoveSpeed = 5.0f;
	UITurnedOn = false;
}

void ShopMenu::Update(float dt)
{
	if (metalTab->IsClicked()) { UITurnedOn = UITurnedOn ? false : true; }

	if (UITurnedOn)
	{
		if (statsButton->IsClicked()) { shopUIcount = 0; }
		if (shopButton->IsClicked()) { shopUIcount = 1; }
		if (upgradesButton->IsClicked()) { shopUIcount = 2; }
		if (abilitiesButton->IsClicked()) { shopUIcount = 3; }

		switch (shopUIcount)
		{
		case 0:
			if (!statsButton->constantHighlight)
			{
				TurnOnStatsUI();
				TurnOffShopUI();
				TurnOffUpgradesUI();
				TurnOffAbilitiesUI();
			}
			if (healthUpButton->IsClicked())
			{
				healthUpButton->constantHighlight = true;
				defenseUpButton->constantHighlight = false;

				iconDisplayImage->LoadTexture(healthUpButton->obj->renderingComponent->mat.GetSurfaceTexture());
				iconDisplayImage->SetUIColor(healthUpButton->uiSurColor);
				iconDisplayImage->SetVisibility(true);
			}
			if (defenseUpButton->IsClicked())
			{
				defenseUpButton->constantHighlight = true;
				healthUpButton->constantHighlight = false;

				iconDisplayImage->LoadTexture(defenseUpButton->obj->renderingComponent->mat.GetSurfaceTexture());
				iconDisplayImage->SetUIColor(defenseUpButton->uiSurColor);
				iconDisplayImage->SetVisibility(true);
			}
			break;
		case 1:
			if (!shopButton->constantHighlight)
			{
				TurnOffStatsUI();
				TurnOnShopUI();
				TurnOffUpgradesUI();
				TurnOffAbilitiesUI();
			}
			break;
		case 2:
			if (!upgradesButton->constantHighlight)
			{
				TurnOffStatsUI();
				TurnOffShopUI();
				TurnOnUpgradesUI();
				TurnOffAbilitiesUI();
			}

			SetUpUpgradeUI();
			break;
		case 3:
			if (!abilitiesButton->constantHighlight)
			{
				TurnOffStatsUI();
				TurnOffShopUI();
				TurnOffUpgradesUI();
				TurnOnAbilitiesUI();
			}
			break;

		}
		if (metalBackGround->posY < -0.2f)
		{
			for (unsigned int i = 0; i < shopUI.size(); i++)
			{
				shopUI[i]->posY += UIMoveSpeed * dt;
			}
		}

		if (buyButton->IsClicked())
		{
			Buy();
		}

		UpdateInputs(dt);
	}
	else
	{
		if (metalBackGround->posY > UIMoveDownHieght)
		{
			for (unsigned int i = 0; i < shopUI.size(); i++)
			{
				shopUI[i]->posY -= UIMoveSpeed * dt;
			}
		}
	}
}

void ShopMenu::UpdateInputs(float dt)
{
	if (!buyButton->constantHighlight && (subUpgradecount == 0)&& inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_CIRCLE))
	{
		UITurnedOn = false;
	}
	if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_L1))
	{
		if (shopUIcount > 0)
		{
			shopUIcount--;
		}
	}
	else if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_R1))
	{
		if (shopUIcount < 3)
		{
			shopUIcount++;
		}
	}

	if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_CROSS) && (shopUIcount != 0))
	{
		upgradeGraph[1]->bought = p1->guidedMissleBought;
		upgradeGraph[2]->bought = true;
		upgradeGraph[3]->bought = p1->sheildComponentBought;
		upgradeGraph[4]->bought = p1->leftBlasterBought;
		upgradeGraph[5]->bought = p1->rightBlasterBought;

		if(buyButton->constantHighlight == true)
		{
			Buy();
			buyButton->constantHighlight = false;
		}
		else
		{
			switch (shopUIcount)
			{
			case 1:
				break;
			case 2:
				if ((currentGraph->currentNum != 0))
				{
					if (currentGraph->bought)
					{
						if (subUpgradecount != 0)
						{
							buyButton->constantHighlight = true;
						}
						else
						{
							subUpgradecount = 1;
						}
					}
					else
					{
						buyButton->constantHighlight = true;
					}
				}
				break;
			case 3:
				break;
			}
		}	
	}

	if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::DPAD_UP))
	{
		switch(shopUIcount)
		{
		case 1:
			break;
		case 2:
			if (subUpgradecount != 0) 
			{
				if(subUpgradecount > 1)
				{
					subUpgradecount--;
				}
				break;
			}
			UpgradeGraphUp();
			break;
		case 3:
			break;
		}
	}
	else if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::DPAD_DOWN))
	{
		switch (shopUIcount)
		{
		case 1:
			break;
		case 2:
			if (subUpgradecount != 0)
			{
				if (subUpgradecount < 2)
				{
					subUpgradecount++;
				}
				break;
			}
			UpgradeGraphDown();
			break;
		case 3:
			break;
		}
	}
	else if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::DPAD_LEFT) 
			|| (inputManager->IsButtonPressed(CosmicInput::ControllerButton::BUTTON_CIRCLE) && subUpgradecount != 0))
	{
		switch (shopUIcount)
		{
		case 1:
			break;
		case 2:
			if (subUpgradecount != 0)
			{
				subUpgradecount = 0;
				leftSubButton->constantHighlight = false;
				rightSubButton->constantHighlight = false;
				buyButton->constantHighlight = false;
				break;
			}
			UpgradeGraphLeft();
			break;
		case 3:
			break;
		}
	}
	else if (inputManager->IsButtonPressed(CosmicInput::ControllerButton::DPAD_RIGHT))
	{
		switch (shopUIcount)
		{
		case 1:
			break;
		case 2:
			if (subUpgradecount != 0)
			{
				break;
			}
			UpgradeGraphRight();
			break;
		case 3:
			break;
		}
	}
}

void ShopMenu::CreateUpgradeGraph()
{
	for(unsigned int i = 0; i < 6; i++)
	{
		upgradeGraph.push_back(new MyGraph(i));
	}

	upgradeGraph[0]->Down = upgradeGraph[2];
	upgradeGraph[0]->Left = upgradeGraph[1];
	upgradeGraph[0]->Right = upgradeGraph[3];

	upgradeGraph[1]->Right = upgradeGraph[2];
	upgradeGraph[1]->Down = upgradeGraph[4];

	upgradeGraph[2]->Left = upgradeGraph[1];
	upgradeGraph[2]->Right = upgradeGraph[3];
	upgradeGraph[2]->Down = upgradeGraph[4];

	upgradeGraph[3]->Left = upgradeGraph[2];
	upgradeGraph[3]->Down = upgradeGraph[5];

	upgradeGraph[4]->Up = upgradeGraph[2];
	upgradeGraph[4]->Left = upgradeGraph[1];
	upgradeGraph[4]->Right = upgradeGraph[5];

	upgradeGraph[5]->Up = upgradeGraph[2];
	upgradeGraph[5]->Left = upgradeGraph[4];
	upgradeGraph[5]->Right = upgradeGraph[3];

	upgradeGraph[1]->bought = p1->guidedMissleBought;
	upgradeGraph[2]->bought = true;
	upgradeGraph[3]->bought = p1->sheildComponentBought;
	upgradeGraph[4]->bought = p1->leftBlasterBought;
	upgradeGraph[5]->bought = p1->rightBlasterBought;

	currentGraph = upgradeGraph[0];
}

void ShopMenu::SetUpUpgradeUI()
{
	//Setting up UI clicks on buttons
	if ((guidedMissleButton->IsClicked() || currentGraph->currentNum == 1) && !guidedMissleButton->constantHighlight)
	{
		guidedMissleButton->constantHighlight = true;
		sheildCompButton->constantHighlight = false;
		frontBlasterButton->constantHighlight = false;
		leftBlasterButton->constantHighlight = false;
		rightBlasterButton->constantHighlight = false;

		leftSubButton->constantHighlight = false;
		rightSubButton->constantHighlight = false;

		iconDisplayImage->LoadTexture(guidedMissleButton->obj->renderingComponent->mat.GetSurfaceTexture());
		iconDisplayImage->SetUIColor(guidedMissleButton->uiSurColor);
		iconDisplayImage->SetVisibility(true);

		if (p1->guidedMissleBought)
		{
			leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("powerIcon"));
			leftSubButton->SetUIColor(guidedMissleButton->uiSurColor);
			leftSubButton->SetVisibility(true);
			rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("sheildRechargeIcon"));
			rightSubButton->SetUIColor(guidedMissleButton->uiSurColor);
			rightSubButton->SetVisibility(true);
			displayLineDownImage->SetVisibility(true);
		}
		else
		{
			leftSubButton->SetVisibility(false);
			rightSubButton->SetVisibility(false);
			displayLineDownImage->SetVisibility(false);
		}

		subUpgradecount = 0;
		currentGraph = upgradeGraph[1];
	}
	if ((sheildCompButton->IsClicked() || currentGraph->currentNum == 3) && !sheildCompButton->constantHighlight)
	{
		guidedMissleButton->constantHighlight = false;
		sheildCompButton->constantHighlight = true;
		frontBlasterButton->constantHighlight = false;
		leftBlasterButton->constantHighlight = false;
		rightBlasterButton->constantHighlight = false;

		leftSubButton->constantHighlight = false;
		rightSubButton->constantHighlight = false;

		iconDisplayImage->LoadTexture(sheildCompButton->obj->renderingComponent->mat.GetSurfaceTexture());
		iconDisplayImage->SetUIColor(sheildCompButton->uiSurColor);
		iconDisplayImage->SetVisibility(true);

		if (p1->sheildComponentBought)
		{
			leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("sheildPowerIcon"));
			leftSubButton->SetUIColor(sheildCompButton->uiSurColor);
			leftSubButton->SetVisibility(true);
			rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("sheildRechargeIcon"));
			rightSubButton->SetUIColor(sheildCompButton->uiSurColor);
			rightSubButton->SetVisibility(true);
			displayLineDownImage->SetVisibility(true);
		}
		else
		{
			leftSubButton->SetVisibility(false);
			rightSubButton->SetVisibility(false);
			displayLineDownImage->SetVisibility(false);
		}

		subUpgradecount = 0;
		currentGraph = upgradeGraph[3];
	}
	if ((frontBlasterButton->IsClicked() || currentGraph->currentNum == 2) && !frontBlasterButton->constantHighlight)
	{
		guidedMissleButton->constantHighlight = false;
		sheildCompButton->constantHighlight = false;
		frontBlasterButton->constantHighlight = true;
		leftBlasterButton->constantHighlight = false;
		rightBlasterButton->constantHighlight = false;

		leftSubButton->constantHighlight = false;
		rightSubButton->constantHighlight = false;

		iconDisplayImage->LoadTexture(frontBlasterButton->obj->renderingComponent->mat.GetSurfaceTexture());
		iconDisplayImage->SetUIColor(frontBlasterButton->uiSurColor);
		iconDisplayImage->SetVisibility(true);

		leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("powerIcon"));
		leftSubButton->SetUIColor(frontBlasterButton->uiSurColor);
		leftSubButton->SetVisibility(true);
		rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("attackSpeedIcon"));
		rightSubButton->SetUIColor(frontBlasterButton->uiSurColor);
		rightSubButton->SetVisibility(true);
		displayLineDownImage->SetVisibility(true);

		subUpgradecount = 0;
		currentGraph = upgradeGraph[2];
	}
	if ((leftBlasterButton->IsClicked() || currentGraph->currentNum == 4) && !leftBlasterButton->constantHighlight)
	{
		guidedMissleButton->constantHighlight = false;
		sheildCompButton->constantHighlight = false;
		frontBlasterButton->constantHighlight = false;
		leftBlasterButton->constantHighlight = true;
		rightBlasterButton->constantHighlight = false;

		leftSubButton->constantHighlight = false;
		rightSubButton->constantHighlight = false;

		iconDisplayImage->LoadTexture(leftBlasterButton->obj->renderingComponent->mat.GetSurfaceTexture());
		iconDisplayImage->SetUIColor(leftBlasterButton->uiSurColor);
		iconDisplayImage->SetVisibility(true);

		if (p1->leftBlasterBought)
		{
			leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("powerIcon"));
			leftSubButton->SetUIColor(leftBlasterButton->uiSurColor);
			leftSubButton->SetVisibility(true);
			rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("attackSpeedIcon"));
			rightSubButton->SetUIColor(leftBlasterButton->uiSurColor);
			rightSubButton->SetVisibility(true);
			displayLineDownImage->SetVisibility(true);
		}
		else
		{
			leftSubButton->SetVisibility(false);
			rightSubButton->SetVisibility(false);
			displayLineDownImage->SetVisibility(false);
		}

		subUpgradecount = 0;
		currentGraph = upgradeGraph[4];
	}
	if ((rightBlasterButton->IsClicked() || currentGraph->currentNum == 5) && !rightBlasterButton->constantHighlight)
	{
		guidedMissleButton->constantHighlight = false;
		sheildCompButton->constantHighlight = false;
		frontBlasterButton->constantHighlight = false;
		leftBlasterButton->constantHighlight = false;
		rightBlasterButton->constantHighlight = true;

		leftSubButton->constantHighlight = false;
		rightSubButton->constantHighlight = false;

		iconDisplayImage->LoadTexture(rightBlasterButton->obj->renderingComponent->mat.GetSurfaceTexture());
		iconDisplayImage->SetUIColor(rightBlasterButton->uiSurColor);
		iconDisplayImage->SetVisibility(true);

		if (p1->rightBlasterBought)
		{
			leftSubButton->LoadTexture(assetManager->GetSurfaceTexture("powerIcon"));
			leftSubButton->SetUIColor(rightBlasterButton->uiSurColor);
			leftSubButton->SetVisibility(true);
			rightSubButton->LoadTexture(assetManager->GetSurfaceTexture("attackSpeedIcon"));
			rightSubButton->SetUIColor(rightBlasterButton->uiSurColor);
			rightSubButton->SetVisibility(true);
			displayLineDownImage->SetVisibility(true);
		}
		else
		{
			leftSubButton->SetVisibility(false);
			rightSubButton->SetVisibility(false);
			displayLineDownImage->SetVisibility(false);
		}

		subUpgradecount = 0;
		currentGraph = upgradeGraph[5];
	}

	if ((leftSubButton->IsClicked() || subUpgradecount == 1) && !leftSubButton->constantHighlight)
	{
		leftSubButton->constantHighlight = true;
		rightSubButton->constantHighlight = false;

		subUpgradecount = 1;
	}

	if ((rightSubButton->IsClicked() || subUpgradecount == 2) && !rightSubButton->constantHighlight)
	{
		rightSubButton->constantHighlight = true;
		leftSubButton->constantHighlight = false;

		subUpgradecount = 2;
	}
}

void ShopMenu::Buy()
{
	if (currentGraph->currentNum != 0)
	{
		switch (currentGraph->currentNum)
		{

			//Guided Missles
		case 1:
			if (!p1->guidedMissleBought)
			{
				p1->guidedMissleBought = true;
				guidedMissleButton->constantHighlight = false;
				SetUpUpgradeUI();

				break;
			}

			if (subUpgradecount != 0)
			{
				switch (subUpgradecount)
				{
				case 1:
					break;
				case 2:
					break;
				}
			}
			break;
			//Front Blaster
		case 2:
			if (subUpgradecount != 0)
			{
				switch (subUpgradecount)
				{
				case 1:
					p1->frontAtkDamage += 0.2f;
					break;
				case 2:
					p1->frontAtkSpeed -= 0.01f;
					break;
				}
			}
			break;
			//Sheild Component
		case 3:
			if (!p1->sheildComponentBought)
			{
				p1->sheildComponentBought = true;
				p1->sheildBar->SetVisibility(true);
				p1->sheildBarFade->SetVisibility(true);
				p1->sheildBarBack->SetVisibility(true);
				p1->sheildBarBorder->SetVisibility(true);

				p1->maxSheild = 50.0f;
				p1->sheild = p1->maxSheild;

				sheildCompButton->constantHighlight = false;
				SetUpUpgradeUI();

				break;
			}

			if (subUpgradecount != 0)
			{
				switch (subUpgradecount)
				{
				case 1:
					p1->maxSheild += 5.0f;
					break;
				case 2:
					p1->sheildRechargeRate -= 0.01f;
					break;
				}
			}
			break;
			//Left Blaster
		case 4:
			if (!p1->leftBlasterBought)
			{
				p1->leftBlasterBought = true;
				p1->leftAtkDamage = 1.0f;
				p1->leftAtkSpeed = 0.1f;
				p1->leftTimer = p1->leftAtkSpeed;

				leftBlasterButton->constantHighlight = false;
				SetUpUpgradeUI();

				break;
			}

			if (subUpgradecount != 0)
			{
				switch (subUpgradecount)
				{
				case 1:
					p1->leftAtkDamage += 0.2f;
					break;
				case 2:
					p1->leftAtkSpeed -= 0.01f;
					break;
				}
			}
			break;
			//Right Blaster
		case 5:
			if (!p1->rightBlasterBought)
			{
				p1->rightBlasterBought = true;
				p1->rightAtkDamage = 1.0f;
				p1->rightAtkSpeed = 0.1f;
				p1->rightTimer = p1->rightAtkSpeed;

				rightBlasterButton->constantHighlight = false;
				SetUpUpgradeUI();

				break;
			}

			if (subUpgradecount != 0)
			{
				switch (subUpgradecount)
				{
				case 1:
					p1->rightAtkDamage += 0.2f;
					break;
				case 2:
					p1->rightAtkSpeed -= 0.01f;
					break;
				}
			}
			break;
		}
	}
}


void ShopMenu::TurnOnStatsUI()
{
	statsButton->constantHighlight = true;

	healthUpButton->SetVisibility(true);
	defenseUpButton->SetVisibility(true);
}

void ShopMenu::TurnOnShopUI()
{
	shopButton->constantHighlight = true;
}

void ShopMenu::TurnOnUpgradesUI()
{
	upgradesButton->constantHighlight = true;

	/*displayLineDownImage;
	leftSubButton;
	rightSubButton;*/
	guidedMissleButton->SetVisibility(true);
	sheildCompButton->SetVisibility(true);
	frontBlasterButton->SetVisibility(true);
	leftBlasterButton->SetVisibility(true);
	rightBlasterButton->SetVisibility(true);
}

void ShopMenu::TurnOnAbilitiesUI()
{
	abilitiesButton->constantHighlight = true;
}

void ShopMenu::TurnOffStatsUI()
{
	statsButton->constantHighlight = false;

	healthUpButton->SetVisibility(false);
	defenseUpButton->SetVisibility(false);

	iconDisplayImage->SetVisibility(false);
}

void ShopMenu::TurnOffShopUI()
{
	shopButton->constantHighlight = false;

	iconDisplayImage->SetVisibility(false);
}

void ShopMenu::TurnOffUpgradesUI()
{
	upgradesButton->constantHighlight = false;

	guidedMissleButton->constantHighlight = false;
	sheildCompButton->constantHighlight = false;
	frontBlasterButton->constantHighlight = false;
	leftBlasterButton->constantHighlight = false;
	rightBlasterButton->constantHighlight = false;

	displayLineDownImage->SetVisibility(false);
	leftSubButton->SetVisibility(false);
	rightSubButton->SetVisibility(false);
	iconDisplayImage->SetVisibility(false);
	guidedMissleButton->SetVisibility(false);
	sheildCompButton->SetVisibility(false);
	frontBlasterButton->SetVisibility(false);
	leftBlasterButton->SetVisibility(false);
	rightBlasterButton->SetVisibility(false);

	subUpgradecount = 0;

	currentGraph = upgradeGraph[0];
}

void ShopMenu::TurnOffAbilitiesUI()
{
	abilitiesButton->constantHighlight = false;

	iconDisplayImage->SetVisibility(false);
}

void ShopMenu::UpgradeGraphUp()
{
	if(currentGraph->Up == nullptr){return; }

	currentGraph = currentGraph->Up;
}

void ShopMenu::UpgradeGraphDown()
{
	if (currentGraph->Down == nullptr) { return; }

	currentGraph = currentGraph->Down;
}

void ShopMenu::UpgradeGraphLeft()
{
	if (currentGraph->Left == nullptr) { return; }

	currentGraph = currentGraph->Left;
}

void ShopMenu::UpgradeGraphRight()
{
	if (currentGraph->Right == nullptr) { return; }

	currentGraph = currentGraph->Right;
}

