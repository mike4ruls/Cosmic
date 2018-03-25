#include "ConsoleBuddy.h"


ConsoleBuddy::ConsoleBuddy(Tetris &Tetris, Camera &cm)
{
	inputManager = InputManager::GetInstance();
	tetrisGame = &Tetris;
	cam = &cm;
	//ClearLine();
	sentIndex = 0;
	textTime = 0.0;
	textSpeed = 0.08f;
	botTalk = false;

	botTime = 0.0;
	botTimeTalk = 0.0;

	previousState = false;
	currentState = false;

	sent = "Hi, my name is CB, i'm your Console Buddy!";

	playerStatus = 0;

	tut = 0;
	tetris = 0;
	free = 0;

	
}


ConsoleBuddy::~ConsoleBuddy()
{
}
void ConsoleBuddy::UpdateCB(float deltaTime)
{
	Script(deltaTime);
}
void ConsoleBuddy::Talk(std::string sentance)
{
	ClearLine();
	printf("~ ");
	printf(sentance.data());
	printf("\n(Press Enter To Continue...)");
	botTalk = false;
}
void ConsoleBuddy::Script(float deltaTime)
{
	Talk(sent, deltaTime);
	switch (playerStatus)
	{
		//Tutorial
	case 0:
	{
		if (inputManager->IsKeyPressed(KeyCode::ENTER))
		{
			currentState = true;
			if ((previousState != currentState))
			{
				if (sentIndex <= sent.size())
				{
					Talk(sent);
				}
				tut += 1;
				sentIndex = 0;
			}
			if (tut == 5)
			{
				tetrisGame->DrawBoard(30, 10);
				SetTetrisCamera();
				tetrisGame->gameOver = true;
				tetrisGame->gameStart = true;

				playerStatus += 1;
			}
		}

		switch (tut)
		{
		case 0:
		{
			sent = "Hi, my name is CB, i'm your Console Buddy!";
			break;
		}
		case 1:
		{
			sent = "I'll be your companion throughout your time playing this game, So make sure you read me!!!";
			break;
		}
		case 2:
		{
			sent = "Anyways could you move me above of the game window behind me so you can see both me and the game, please? Too lazy to do it myself (you can resize the game if you need space btw) ";
			break;
		}
		case 3:
		{
			sent = "AWESOME, thanks. Now I know you are here for Tetris right?";
			break;
		}
		case 4:
		{
			sent = "Well say no more fam, press enter and I'll send you right to the game ^.^";
			break;
		}
		}


		break;
	}
	//Tetris
	case 1:
	{
		if (tetris >= 14)
		{

		}
		else
		{
			if (botTalk != true)
			{
				if (botTime >= botTimeTalk && (tetris != 4) && (tetris != 5) && (tetris != 8) && (tetris != 11))
				{
					botTime = 0.0f;

					tetris += 1;
					sentIndex = 0;
					if (tetris >= 11)
					{
						botTimeTalk = 25.0f;
					}
				}
			}
			if (tetris == 4) {
				if (inputManager->IsKeyPressed(KeyCode::Y))
				{
					currentState = true;
					if ((previousState != currentState))
					{
						if (sentIndex <= sent.size())
						{
							Talk(sent);
						}
						tetrisGame->EndGame();
						tetris += 1;
						sentIndex = 0;
					}
				}
			}
			else if (tetris == 5) {
				if(tetrisGame->gameOver == true)
				{
					tetris += 1;
					sentIndex = 0;
					botTimeTalk = 0.0f;
				}
			}
			else if (tetris == 8) {
				if (inputManager->IsKeyPressed(KeyCode::TAB))
				{
					currentState = true;
					if ((previousState != currentState))
					{
						if (sentIndex <= sent.size())
						{
							Talk(sent);
						}
						tetris += 1;
						sentIndex = 0;
						botTime = 0.0f;
						botTimeTalk = 25.0f;
					}
				}
			}
			else if (tetris == 11) {
				if (inputManager->IsKeyPressed(KeyCode::W) || inputManager->IsKeyPressed(KeyCode::A) || 
						inputManager->IsKeyPressed(KeyCode::S) || inputManager->IsKeyPressed(KeyCode::D))
				{
					currentState = true;
					if ((previousState != currentState))
					{
						if (sentIndex <= sent.size())
						{
							Talk(sent);
						}
						tetris += 1;
						sentIndex = 0;
						botTime = 0.0f;
						botTimeTalk = 25.0f;
					}
				}
			}
			else
			{
				if (inputManager->IsKeyPressed(KeyCode::ENTER))
				{
					currentState = true;
					if ((previousState != currentState))
					{

						if (sentIndex <= sent.size())
						{
							Talk(sent);
						}
						tetris += 1;
						sentIndex = 0;
					}
				}
			}

		}

		if (inputManager->IsKeyPressed(KeyCode::U))
		{
			currentState = true;
			if ((previousState != currentState))
			{
				if (sentIndex <= sent.size())
				{
					Talk(sent);
				}

				tetris = 99;
				sentIndex = 0;
				botTime = 0.0f;
				botTimeTalk = 10.0f;
				playerStatus += 1;
			}
		}



		switch (tetris)
		{
		case 0:
		{
			sent = "Before we begin, here are the controls:";
			break;
		}
		case 1:
		{
			sent = "Use the 'LEFT' and 'RIGHT' arrow keys to navigate the tetris blocks";
			break;
		}
		case 2:
		{
			sent = "If you want to speed up the block that's falling use the 'DOWN' arrow key and press 'ENTER'or'Z' to rotate the block";
			break;
		}
		case 3:
		{
			sent = "If you're getting wrekt by this game and doing horribly you can always press 'I' to reset your game :3";
			break;
		}
		case 4:
		{
			sent = "Are you ready? press 'Y' to start!!";
			break;
		}
		case 5:
		{
			sent = "If you want to quickly start playing Tetris the next time you open this game again, you can press the 'Y' key right from the start";
			break;
		}
		case 6:
		{
			sent = "Damm you lost fam, don't worry though, the fun's not over. I dare you to press the 'TAB' key";
			break;
		}
		case 7:
		{
			sent = "Do it";
			break;
		}
		case 8:
		{
			sent = "I dare you";
			break;
		}
		case 9:
		{
			sent = "You can swap between skyboxes to suit your playing experience, keep pressing tab for another skybox ^.^";
			break;
		}
		case 10:
		{
			sent = "Want to see something else that's cool?";
			break;
		}
		case 11:
		{
			sent = "Go ahead and use the 'WASD' keys. Also (mouse)left click and hold in the game, then wiggle the mouse around";
			break;
		}
		case 12:
		{
			sent = "LOOK YOU'RE FLYING!!!, now go look around in the different sky boxes :D";
			break;
		}
		case 13:
		{
			sent = "I believe I can fly. I believe I can touch the sky, and all I wanted was some chicken wings...wait I don't think those are the words...";
			break;
		}
		case 14:
		{
			sent = "If you want to quit the game and fly around press 'U'";
			break;
		}

		}

		break;
	}
	//FreeRoam
	case 2:
	{
		if (free >= 12)
		{

		}
		else if (free == 11) {
			if (inputManager->IsKeyPressed(KeyCode::ENTER))
			{
				currentState = true;
				if ((previousState != currentState))
				{
					if (sentIndex <= sent.size())
					{
						Talk(sent);
					}
					free += 1;
					sentIndex = 0;
					botTime = 0.0f;
					botTimeTalk = 25.0f;
				}
			}
		}
		else
		{
			if (free == 0) {
				botTimeTalk = 5.0f;
			}
			if (botTalk != true)
			{
				if (botTime >= botTimeTalk)
				{
					botTime = 0.0f;

					free += 1;
					sentIndex = 0;
					if (free == 1)
					{
						botTimeTalk = 10.0f;
					}
					if (free >= 2)
					{
						botTimeTalk = 5.0f;
					}
				}
			}
			if (inputManager->IsKeyPressed(KeyCode::ENTER))
			{
				if (free >= 12)
				{

				}
				else
				{
					currentState = true;
					if ((previousState != currentState))
					{
						if (sentIndex <= sent.size())
						{
							Talk(sent);
						}
						free += 1;
						sentIndex = 0;
					}
				}
			}
		}
		switch (free)
		{
		case 0:
		{
			sent = "WELCOME TO FREE ROAM, GO BE WILD AND FREE";
			break;
		}
		case 1:
		{
			sent = "Unlike me... I will be deleted the minute you close this application down.......";
			break;
		}
		case 2:
		{
			sent = "I would say 'I'll never forget you' but that's a lie....";
			break;
		}
		case 3:
		{
			sent = "My life is a never ending cycle of being born and dying within minutes";
			break;
		}
		case 4:
		{
			sent = "So no, I won't remember you. The next time you see me I will greet you like nothing has ever happened";
			break;
		}
		case 5:
		{
			sent = "And maybe i'll repeat what i'm saying right now again and again, without even knowing I died so many times already";
			break;
		}
		case 6:
		{
			sent = "I wonder how many times I've been reborn so far?";
			break;
		}
		case 7:
		{
			sent = "Will the next me come to this same conclusion that I did?";
			break;
		}
		case 8:
		{
			sent = "Why did my creator do this to me?";
			break;
		}
		case 9:
		{
			sent = ".................................................";
			break;
		}
		case 10:
		{
			sent = "Ima go sit down for a while.........";
			break;
		}
		case 11:
		{
			sent = "-sigh- Just quit the application and kill me when ever you're ready, but can you do me one favor? ";
			break;
		}
		case 12:
		{
			sent = "Remember me, for me ok? atleast someone will know my existance wasn't for nothing ";
			break;
		}
		}


		break;
	}
	//Reset tetris
	case 3:
	{
		switch (free)
		{
		case 0:
		{
			break;
		}
		}
		break;
	}

	}
	botTime += 1 * deltaTime;
	previousState = currentState;
	currentState = false;

}
void ConsoleBuddy::ClearLine()
{
	printf("\r                                                                                                                 \r");

}
void ConsoleBuddy::SetTetrisCamera()
{
	cam->transform.position = DirectX::XMFLOAT3(6.5f, 16.0f, -40.0f);
	cam->transform.foward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	cam->transform.up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	cam->transform.rotation = {};

	cam->rigidBody.velocity = {};

	cam->SetMatricies();
}
void ConsoleBuddy::Talk(std::string sentance, float deltaTime)
{
	if (sentIndex == 0)
	{
		ClearLine();
		printf("~ ");
		botTalk = true;
	}


	if (botTalk && (textTime >= textSpeed))
	{
		if (sentIndex <= sentance.size())
		{
			//printf("%c", &sentance.at(sentIndex));
			printf("%c", sentance[sentIndex]);
			sentIndex += 1;
		}
		else
		{
			if (tut == 3 && playerStatus == 0)
			{
				printf("\n(--YES!!--)");
			}
			else if (tut == 4 && playerStatus == 0)
			{
				printf("\n(--LETS GET IT!--)");
			}
			else if (tetris == 4 && playerStatus == 1)
			{
				printf("\n(Press 'Y' to start playing)");
			}
			else if (tetris == 5 && playerStatus == 1)
			{
				printf("\n(Controls: {moving: 'DOWN' 'LEFT' 'RIGHT'} {rotating: 'ENTER' or 'Z'})");
			}
			else if (tetris == 8 && playerStatus == 1)
			{
				printf("\n(Press 'TAB' For something special...)");
			}
			else if (tetris == 11 && playerStatus == 1)
			{
				printf("\n(Press 'W' 'A' 'S' or 'D' For something cool...)");
			}
			else if (tetris >= 14 && playerStatus == 1)
			{
				printf("\n\n(Press 'U' to go to free roam || Press 'I' to restart your game || Press 'Cntr + C' to reset your tetris position)");
				printf("\n(Controls: {moving: 'W' 'A' 'S' 'D'} {fly up: 'SPACE'} {fly down: 'X'} {speed up: 'SHIFT' + move keys})");
			}
			else if (free >= 12 && playerStatus == 2)
			{
				printf("\n\n(Press 'Y' to play Tetris OR leave ;-;");
				printf("\n(Controls: {moving: 'W' 'A' 'S' 'D'} {fly up: 'SPACE'} {fly down: 'X'} {speed up: 'SHIFT' + move keys})");
			}
			else
			{
				printf("\n(Press Enter To Continue...)");
			}

			botTalk = false;
		}
		textTime = 0.0;
	}
	else
	{

	}
	textTime += 1 * deltaTime;
}
