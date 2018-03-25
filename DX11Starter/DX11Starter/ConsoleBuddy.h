#pragma once
#include "InputManager.h"
#include "Tetris.h"
#include "Camera.h"

class ConsoleBuddy
{
public:
	ConsoleBuddy(Tetris &TetrisGame, Camera &cm);
	~ConsoleBuddy();

	void UpdateCB(float deltaTime);
	void Script(float deltaTime);
	void SetTetrisCamera();

	int playerStatus;
private:
	std::string sent;
	InputManager* inputManager;

	bool previousState;
	bool currentState;

	void Talk(std::string sentance, float deltaTime);
	void Talk(std::string sentance);
	void ClearLine();

	bool botTalk;
	unsigned int sentIndex;

	int tut;
	int tetris;
	int free;

	float textTime;
	float textSpeed;
	float botTime;
	float botTimeTalk;

	Tetris* tetrisGame;
	Camera* cam;
};

