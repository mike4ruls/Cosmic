#pragma once
#include "Game.h"
#include "Tetris.h"
#include "ConsoleBuddy.h"
class TetrisScene :
	public Game
{
public:
	TetrisScene();
	~TetrisScene();

	void Init();
	void Update(float deltaTime, float totalTime);
	void CheckInputs(float deltaTime);

	ConsoleBuddy* cB;
	Tetris* myTetris;
};

