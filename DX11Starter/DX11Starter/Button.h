#pragma once
#include "UI.h"
class Button :
	public UI
{
public:
	struct MouseScreen
	{
		float x;
		float y;
	};

	Button(GameEntity* o, UIType t, void* eng);
	~Button();

	MouseScreen prevMousePos;

	bool isHovering;

	void Update(float dt);
	bool IsClicked();
	void LoadTexture(ID3D11ShaderResourceView* svr);
	void FlushTexture();

private:
	float maxX;
	float minX;
	float maxY;
	float minY;

	void CheckHovering();

	void* engine;
};

