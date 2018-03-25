#include "TetrisBlock.h"



TetrisBlock::TetrisBlock(Mesh &bShape, DirectX::XMFLOAT4 matColor, int tp, int r)
{
	type = tp;
	rot = r;
	translation = {0.0, 0.0, 0.0};
	for (int i = 0; i < 4; i++)
	{
		GameEntity* block = new GameEntity(&bShape, false);
		tetrisBlocks.push_back(block);
		tetrisBlocks[i]->renderingComponent->mat.surfaceColor = matColor;
	}

	LoadTetrisBlock();
}

std::vector<GameEntity*> TetrisBlock::GetEntities()
{
	return tetrisBlocks;
}

TetrisBlock::~TetrisBlock()
{
	
}
void TetrisBlock::DeleteBlockGameEntities()
{
	for (unsigned int i = 0; i < tetrisBlocks.size(); i++)
	{
	tetrisBlocks[i]->Destroy();
	}
}
void TetrisBlock::LoadTetrisBlock()
{
	if(rot > 4)
	{
		rot = 1;
	}
	if (type > 6)
	{
		type = 1;
	}

	tetrisBlocks[0]->ResetGameEntity();
	tetrisBlocks[1]->ResetGameEntity();
	tetrisBlocks[2]->ResetGameEntity();
	tetrisBlocks[3]->ResetGameEntity();


	switch(type)
	{
		//STRAIGHT BLOCK
	case 1:
	{
		if(rot == 1)
		{
			tetrisBlocks[0]->transform.Translate(2.0, 0.0, 0.0);
			tetrisBlocks[1]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, 0.0, 0.0);
		}
		if (rot == 2)
		{
			tetrisBlocks[0]->transform.Translate(0.0, -2.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(2.0, 0.0, 0.0);
		}
		if (rot == 4)
		{
			tetrisBlocks[0]->transform.Translate(0.0, 2.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, -1.0, 0.0);
		}
		break;
	}
	// Z BLOCK
	case 2:
	{
		if (rot == 1)
		{
			tetrisBlocks[0]->transform.Translate(1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, -1.0, 0.0);
		}
		if (rot == 2)
		{
			tetrisBlocks[0]->transform.Translate(1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, 0.0, 0.0);
		}
		if (rot == 3)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, 1.0, 0.0);
		}
		if (rot == 4)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(1.0, 0.0, 0.0);
		}
		break;
	}
	// REVERSE Z BLOCK
	case 3:
	{
		if (rot == 1)
		{
			tetrisBlocks[0]->transform.Translate(1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, 1.0, 0.0);
		}
		if (rot == 2)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(1.0, 0.0, 0.0);
		}
		if (rot == 3)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, -1.0, 0.0);
		}
		if (rot == 4)
		{
			tetrisBlocks[0]->transform.Translate(1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, 0.0, 0.0);
		}

		break;
	}
	// L BLOCK
	case 4:
	{
		if (rot == 1)
		{
			tetrisBlocks[0]->transform.Translate(1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, 0.0, 0.0);
		}
		if (rot == 2)
		{
			tetrisBlocks[0]->transform.Translate(1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(1.0, 0.0, 0.0);
		}
		if (rot == 4)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(0.0, -1.0, 0.0);
		}
		break;
	}
	// REVERSE L BLOCK
	case 5:
	{
		if (rot == 1)
		{
			tetrisBlocks[0]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, 1.0, 0.0);
		}
		if (rot == 2)
		{
			tetrisBlocks[0]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[3]->transform.Translate(1.0, 1.0, 0.0);
		}
		if (rot == 3)
		{
			tetrisBlocks[0]->transform.Translate(-1.0, 0.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(1.0, 0.0, 0.0);
			tetrisBlocks[3]->transform.Translate(1.0, -1.0, 0.0);
		}
		if (rot == 4)
		{
			tetrisBlocks[0]->transform.Translate(0.0, 1.0, 0.0);
			tetrisBlocks[1]->transform.Translate(0.0, 0.0, 0.0);
			tetrisBlocks[2]->transform.Translate(0.0, -1.0, 0.0);
			tetrisBlocks[3]->transform.Translate(-1.0, -1.0, 0.0);
		}
		break;
	}
	// SQUARE BLOCK
	case 6:
	{
		tetrisBlocks[0]->transform.Translate(0.0, 0.0, 0.0);
		tetrisBlocks[1]->transform.Translate(0.0, -1.0, 0.0);
		tetrisBlocks[2]->transform.Translate(1.0, -1.0, 0.0);
		tetrisBlocks[3]->transform.Translate(1.0, 0.0, 0.0);
		break;
	}
	}
	for (int i = 0; i < (int)tetrisBlocks.size(); i++)
	{
		tetrisBlocks[i]->transform.Translate(translation.x, translation.y, translation.z);
	}
}
void TetrisBlock::TransTetrisBlock(float x, float y, float z)
{
	for (int i = 0; i < (int)tetrisBlocks.size(); i++)
	{
		tetrisBlocks[i]->transform.Translate(x, y, z);
	}
	translation.x += x;
	translation.y += y;
	translation.z += z;
}