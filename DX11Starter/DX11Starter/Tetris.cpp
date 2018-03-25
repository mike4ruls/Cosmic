#include "Tetris.h"



Tetris::Tetris(Mesh &shape, int h, int w)
{
	inputManager = InputManager::GetInstance();

	shapeBlock = &shape;
	red = {1.0f, 0.0f, 0.0f, 1.0f};
	blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	green = { 0.0f, 1.0f, 0.0f, 1.0f };
	purple = { 1.0f, 0.0f, 1.0f, 1.0f };
	lightBlue = { 0.0f, 1.0f, 1.0f, 1.0f };
	yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	grey = { 0.5f, 0.5f, 0.5f, 1.0f };
	gameStart = false;
	gameOver = false;
	score = 0;
	height = h;
	width = w;

	tChange = false;
	pChange = false;
	
	currentState = false;
	previousState = false;
	tTime = 0.0;
	timeOfDescent = 1.0f;
	leftRightTime = 0.0f;
	scoreCol = 1;
	scoreRow = 0;
	srand((unsigned int)time(NULL));

	inputManager->AddActionBinding(Actions::ButtonUp, { 87, CosmicInput::ControllerButton::DPAD_UP });
	inputManager->AddActionBinding(Actions::ButtonDown, { 83, CosmicInput::ControllerButton::DPAD_DOWN });
	inputManager->AddActionBinding(Actions::ButtonLeft, { 65, CosmicInput::ControllerButton::DPAD_LEFT });
	inputManager->AddActionBinding(Actions::ButtonRight, { 68, CosmicInput::ControllerButton::DPAD_RIGHT });
	inputManager->AddActionBinding(Actions::Rotate, { VK_RETURN,  CosmicInput::ControllerButton::BUTTON_R2 });
	inputManager->AddActionBinding(Actions::Place, { VK_SPACE, CosmicInput::ControllerButton::BUTTON_CROSS });
	inputManager->AddActionBinding(Actions::Start, { 99, CosmicInput::ControllerButton::BUTTON_OPTIONS });

	//DrawBoard(height, width);
}


Tetris::~Tetris()
{
	if (currentBlock != nullptr) { delete currentBlock; currentBlock = nullptr; }
	if (futureBlock != nullptr) { delete futureBlock; futureBlock = nullptr; }

}
void Tetris::UpdateGame(float deltaTime, float totalTime)
{
	if (gameOver != true) 
	{
		MoveBlock(deltaTime);
		CheckWallCollide();
		CheckBlockCollide();
		CheckFloorCollide();
		CheckForLine(deltaTime);
		CheckGameOver();
		
		//Testing AddScore()
		
		
		/*if (inputManager->IsKeyPressed(KeyCode::N))
		{
			currentState = true;
			if (currentState != previousState) {
				AddToScore();
			}
		}*/
		
		tTime += (float)1.0 * deltaTime;
		previousState = currentState;
		currentBlock->LoadTetrisBlock();
		SetFutureBlock();
		currentState = false;
	}
	else
	{

	}
	for (unsigned int i = 0; i < scoreBlocks.size(); i++)
	{
		//float scale = sin(((float)(scoreBlocks[i].GetPosition().x + scoreBlocks[i].GetPosition().y + scoreBlocks[i].GetPosition().z)*2.0f) + totalTime) * (deltaTime*0.2f);
		//scoreBlocks[i]->transform.Rotate(0.0f, ((totalTime * 3.0f) - scoreBlocks[i]->transform.rotation.y), ((totalTime * ((float)i*1.5f)) - scoreBlocks[i]->transform.rotation.z));
		scoreBlocks[i]->transform.Rotate(((totalTime * 150.0f) - scoreBlocks[i]->transform.rotation.x), ((totalTime * ((float)i*75.0f)) - scoreBlocks[i]->transform.rotation.y), 0.0f);
		//scoreBlocks[i].Scale(scale, scale, scale);
	}
}
void Tetris::StartGame(int h, int w)
{
	height = h;
	width = w;

	DrawBoard(height, width);
	currentBlock = GenerateBlock();
	futureBlock = GenerateBlock();
	gameStart = true;
	tChange = true;
	pChange = true;
	gameOver = false;

}

void Tetris::DrawBoard(int height, int width)
{
	board.clear();
	for (int i = 0; i < height; i++)
	{
		GameEntity* wall = new GameEntity(shapeBlock, false);
		wall->renderingComponent->mat.surfaceColor = lightBlue;
		wall->name = "wall";
		wall->transform.Translate(0.0, (float)i, 0.0);

		board.push_back(wall);
	}
	for (int i = 0; i < width; i++)
	{
		GameEntity* wall = new GameEntity(shapeBlock, false);
		wall->renderingComponent->mat.surfaceColor = lightBlue;
		wall->name = "wall";
		wall->transform.Translate((float)(i+1), 0, 0);

		board.push_back(wall);
	}
	for (int i = 0; i < height; i++)
	{
		GameEntity* wall = new GameEntity(shapeBlock, false);
		wall->renderingComponent->mat.surfaceColor = lightBlue;
		wall->name = "wall";
		wall->transform.Translate((float)(width + 1), (float)i, 0.0);

		board.push_back(wall);
	}


}

TetrisBlock* Tetris::GenerateBlock()
{
	TetrisBlock* block;
	int ranBlock = rand() % 6 + 1;
	switch(ranBlock)
	{
	case 1:
	{
		block = new TetrisBlock(*shapeBlock, blue, ranBlock, 2);
		break;
	}
	case 2:
	{
		block = new TetrisBlock(*shapeBlock, green, ranBlock, 2);
		break;
	}
	case 3:
	{
		block = new TetrisBlock(*shapeBlock, purple, ranBlock, 2);
		break;
	}
	case 4:
	{
		block = new TetrisBlock(*shapeBlock, yellow, ranBlock, 2);
		break;
	}
	case 5:
	{
		block = new TetrisBlock(*shapeBlock, grey, ranBlock, 2);
		break;
	}
	case 6:
	{
		block = new TetrisBlock(*shapeBlock, red, ranBlock, 2);
		break;
	}
	}
	SetCurrentBlock(block);
	return block;
}
std::vector<GameEntity*>* Tetris::GetBoard()
{
	return &board;
}
std::vector<GameEntity*> Tetris::GetTBlocks()
{
	std::vector<GameEntity*> blockEntities;
	//Gets current block falling
	for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
	{
		blockEntities.push_back(currentBlock->GetEntities()[i]);
	}
	//Gets future block to fall
	for (int i = 0; i < (int)futureBlock->GetEntities().size(); i++)
	{
		blockEntities.push_back(futureBlock->GetEntities()[i]);
	}

	return blockEntities;
}
std::vector<GameEntity*> Tetris::GetPBlocks()
{
	std::vector<GameEntity*> blockEntities;
	//Gets placed blocks
	for (int i = 0; i < (int)tBlocks.size(); i++)
	{
		blockEntities.push_back(tBlocks[i]);
	}

	return blockEntities;
}
void Tetris::SetCurrentBlock(TetrisBlock *block)
{
	int newH = height - 1;
	int newW = (int)(width / 2);
	block->translation = { (float)newW,(float)newH,0.0 };
	block->LoadTetrisBlock();
}
void Tetris::SetFutureBlock()
{
	int newH = (int)(height/2);
	int newW = (int)width + 5;
	futureBlock->translation = { (float)newW,(float)newH,0.0 };
	futureBlock->LoadTetrisBlock();
}
void Tetris::MoveBlock(float delta)
{
	if (score <= 5)
	{
		timeOfDescent = 1.0f;
	}
	else if (score <= 10)
	{
		timeOfDescent = 0.6f;
	}
	else if (score <= 20)
	{
		timeOfDescent = 0.2f;
	}
	else if (score <= 30)
	{
		timeOfDescent = 0.09f;
	}
	if (inputManager->IsActionPressed(Actions::Rotate))
	{
		currentBlock->rot += 1;
		tChange = true;
		//currentState = true;
		//if (currentState != previousState) {
		//	currentBlock->rot += 1;
		//	tChange = true;
		//}
	}
	if (inputManager->IsActionDown(Actions::ButtonLeft))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			if(CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			}
			tChange = true;
			leftRightTime = 0.1f;
		}
		if(leftRightTime <= 0.0f)
		{
			currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			if (CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			}
			tChange = true;
			leftRightTime = 0.1f;
		}

		leftRightTime -= 0.8f * delta;
	}
	if (inputManager->IsActionDown(Actions::ButtonRight))
	{
		currentState = true;
		if (currentState != previousState) {
			currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			if (CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			}
			tChange = true;
			leftRightTime = 0.1f;
		}
		if (leftRightTime <= 0.0f)
		{
			currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			if (CheckWallBlockCollide())
			{
				currentBlock->TransTetrisBlock(-1.0, 0.0, 0.0);
			}
			tChange = true;
			leftRightTime = 0.1f;
		}

		leftRightTime -= 0.8f * delta;
	}
	if (inputManager->IsActionDown(Actions::ButtonDown))
	{
		currentState = true;
		timeOfDescent = 0.05f;
		//currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tChange = true;
	}
	if (tTime >= timeOfDescent)
	{
		currentBlock->TransTetrisBlock(0.0, -1.0, 0.0);
		tChange = true;
		tTime = 0.0;
	}

}
void Tetris::CheckWallCollide()
{
	for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
	{
		if (currentBlock->GetEntities()[i]->transform.position.x <= board[1]->transform.position.x)
		{
			currentBlock->TransTetrisBlock(1.0, 0.0, 0.0);
			tChange = true;

		}
		else if (currentBlock->GetEntities()[i]->transform.position.x >= board[1]->transform.position.x + (width + 1))
		{
			currentBlock->TransTetrisBlock(-1.0,0.0,0.0);
			tChange = true;

		}
	}
}
void Tetris::CheckBlockCollide()
{
	if ((int)tBlocks.size() == 0)
	{
	}
	else {
		for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
		{
			for (int j = 0; j < (int)tBlocks.size(); j++)
			{
				if (currentBlock->GetEntities()[i]->transform.position.x == tBlocks[j]->transform.position.x && currentBlock->GetEntities()[i]->transform.position.y == tBlocks[j]->transform.position.y)
				{
					currentBlock->TransTetrisBlock(0.0, 1.0, 0.0);
					PlaceBlock();

					break;
				}
			}

		}
	}
}
bool Tetris::CheckWallBlockCollide()
{
	if ((int)tBlocks.size() == 0)
	{
	}
	else {
		for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
		{
			for (int j = 0; j < (int)tBlocks.size(); j++)
			{
				if (currentBlock->GetEntities()[i]->transform.position.x == tBlocks[j]->transform.position.x && currentBlock->GetEntities()[i]->transform.position.y == tBlocks[j]->transform.position.y)
				{
					return true;

					break;
				}
			}

		}
	}
	return false;
}
void Tetris::CheckFloorCollide()
{
	for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
	{
		if (currentBlock->GetEntities()[i]->transform.position.y <= board[1]->transform.position.y - 1)
		{
			currentBlock->TransTetrisBlock(0.0, 1.0, 0.0);
			PlaceBlock();
		}
	}
}
void Tetris::PlaceBlock()
{
	for (int i = 0; i < (int)currentBlock->GetEntities().size(); i++)
	{
		tBlocks.push_back(currentBlock->GetEntities()[i]);
	}
	if (currentBlock != nullptr) { delete currentBlock; currentBlock = nullptr; }
	currentBlock = futureBlock;
	futureBlock = GenerateBlock();
	SetCurrentBlock(currentBlock);
	pChange = true;
}
void Tetris::CheckForLine(float delta)
{
	std::vector<int> spot;
	std::vector<int> heig;
	int numOfBlocks = 0;
	int lines = 0;

	for (int i = 1; i < height; i++) 
	{
		for (int j = 0; j < (int)tBlocks.size(); j++)
		{
			if(tBlocks[j]->transform.position.y == i)
			{
				numOfBlocks += 1;
				spot.push_back(j);
			}
		}

		if(numOfBlocks == width)
		{
			lines += 1;
			heig.push_back(i);
			
		}
		else
		{
			if (lines == 0)
			{
				spot.clear();
			}
			else
			{
				for (int i = 0; i < numOfBlocks; i++)
				{
					spot.pop_back();
				}
			}
		}
		
		
		pChange = true;
		numOfBlocks = 0;
		
	}
	if(lines > 0)
	{
		// Swap numbers in order
		for (int j = 0; j < (int)spot.size(); j++)
		{
			for (int k = j; k < (int)spot.size(); k++)
			{
				if (spot[j]<spot[k])
				{
					std::iter_swap(spot.begin() + j, spot.begin() + k);
				}
			}
		}
		// Starts Deleting them
		for (int k = (int)spot.size() - 1; k >= 0; k--)
		{
			if (k == (int)spot.size() - 1)
			{
				if (tBlocks[spot[k]]->transform.position.z >= 300.0)
				{

				}
				else
				{
					tBlocks[spot[k]]->transform.Translate(0.0f, 0.0f, (300.0f * delta));
				}
			}
			else
			{
				if (tBlocks[spot[k]]->transform.position.z >= 300.0)
				{

				}
				else
				{
					if (tBlocks[spot[k + 1]]->transform.position.z >= 20)
					{
						tBlocks[spot[k]]->transform.Translate(0.0f, 0.0f, (300.0f * delta));
					}
				}

			}
		}
		// When all of them has disappeared
		if (tBlocks[spot[0]]->transform.position.z >= 60.0)
		{
			for (unsigned int j = 0; j < heig.size(); j++)
			{
				for (unsigned int k = j; k < heig.size(); k++)
				{
					if (spot[j]>spot[k])
					{
						std::iter_swap(heig.begin() + j, heig.begin() + k);
					}
				}
			}

			for (int k = 0; k < (int)spot.size(); k++)
			{
				tBlocks[spot[k]]->Destroy();
				tBlocks.erase(tBlocks.begin() + spot[k]);
			}
			for (unsigned int i = 0; i < heig.size(); i++)
			{
				AddToScore();
				Reposition(heig[i]);
			}
			spot.clear();
			heig.clear();
			lines = 0;

		}
	}
}
void Tetris::Reposition(int yHieght)
{
	for (int i = 0; i < (int)tBlocks.size();i++)
	{
		if(tBlocks[i]->transform.position.y > yHieght)
		{
			tBlocks[i]->transform.Translate(0.0, -1.0, 0.0);
		}
	}
}
void Tetris::CheckGameOver()
{
	for (int i = 0; i < (int)tBlocks.size(); i++)
	{
		if(tBlocks[i]->transform.position.y == height)
		{
			gameOver = true;
			break;
		}
	}
}
void Tetris::EndGame()
{
	ClearBoard();
	DeleteBoard();

	gameStart = false;
	gameOver = true;

	pChange = true;
	tChange = true;
}
void Tetris::ResetGame()
{
	ClearBoard();

	score = 0;
	tTime = 0.0;
	
	scoreCol = 1;
	scoreRow = 0;

	currentBlock = GenerateBlock();
	futureBlock = GenerateBlock();
	gameStart = true;
	tChange = true;
	pChange = true;
	gameOver = false;
}
void Tetris::AddToScore()
{
	score += 1;
	scoreRow += 1;
	GameEntity* block = new GameEntity(shapeBlock, false);
	block->renderingComponent->mat.surfaceColor = red;

	if(score > (10 * scoreCol))
	{
		scoreRow = 1;
		scoreCol += 1;
	}

	block->transform.Translate(board[0]->transform.position.x - (2.0f * (float)scoreCol),height - (2.0f * (float)scoreRow), 0.0f);

	scoreBlocks.push_back(block);
}

void Tetris::ClearBoard()
{
	for (unsigned int i = 0; i < tBlocks.size(); i++)
	{
		tBlocks[i]->Destroy();
	}
	for (unsigned int i = 0; i < scoreBlocks.size(); i++)
	{
		scoreBlocks[i]->Destroy();
	}
	tBlocks.clear();
	scoreBlocks.clear();

	if (currentBlock != nullptr) { currentBlock->DeleteBlockGameEntities(); delete currentBlock; currentBlock = nullptr; }
	if (futureBlock != nullptr) { futureBlock->DeleteBlockGameEntities(); delete futureBlock; futureBlock = nullptr; }

}

void Tetris::DeleteBoard()
{
	for (int i = 0; i < board.size(); i++) 
	{
		board[i]->Destroy();
	}
}
