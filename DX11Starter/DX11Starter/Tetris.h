#pragma once
#include "GameEntity.h"
#include "TetrisBlock.h"
#include "InputManager.h"
#include <stdlib.h>
#include <time.h>

class Tetris
{
public:
	enum Actions {
		ButtonUp,
		ButtonDown,
		ButtonLeft,
		ButtonRight,
		Rotate,
		Place,
		Start
	};
	Tetris(Mesh &shape, int h, int w);
	~Tetris();

	void StartGame(int h, int w);
	void DrawBoard(int height, int width);
	void UpdateGame(float deltaTime, float totalTime);
	void ResetGame();
	void EndGame();
	std::vector<GameEntity*>* GetBoard();
	std::vector<GameEntity*> GetTBlocks();
	std::vector<GameEntity*> GetPBlocks();
	TetrisBlock* GenerateBlock();
	void SetCurrentBlock(TetrisBlock *block);
	void SetFutureBlock();
	void MoveBlock(float delta);
	void CheckWallCollide();
	void CheckBlockCollide();
	bool CheckWallBlockCollide();
	void CheckFloorCollide();
	void CheckGameOver();
	void PlaceBlock();
	void CheckForLine(float delta);
	void Reposition(int yHieght);
	void AddToScore();
	void ClearBoard();
	void DeleteBoard();

	InputManager* inputManager;
	
	Mesh* shapeBlock;
	DirectX::XMFLOAT4 red;
	DirectX::XMFLOAT4 blue;
	DirectX::XMFLOAT4 green;
	DirectX::XMFLOAT4 purple;
	DirectX::XMFLOAT4 lightBlue;
	DirectX::XMFLOAT4 yellow;
	DirectX::XMFLOAT4 grey;

	TetrisBlock* futureBlock = nullptr;
	TetrisBlock* currentBlock = nullptr;

	int score;
	bool gameStart;
	bool gameOver;
	int height;
	int width;
	float tTime;
	float timeOfDescent;
	float leftRightTime;
	int scoreCol;
	int scoreRow;

	bool pChange;
	bool tChange;

	bool currentState;
	bool previousState;
	
	std::vector<GameEntity*> board;
	std::vector<GameEntity*> tBlocks;
	std::vector<GameEntity*> scoreBlocks;
	std::vector<GameEntity*> otherBlocks;

private:

};

