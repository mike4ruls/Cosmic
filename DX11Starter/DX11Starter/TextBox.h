#pragma once
#include "UI.h"
class TextBox :
	public UI
{
public:
	TextBox(GameEntity* o);
	TextBox();
	~TextBox();

	void Update(float dt);
};

