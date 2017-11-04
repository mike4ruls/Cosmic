#pragma once
#include "UI.h"
class TextBox :
	public UI
{
public:
	TextBox(GameEntity* o, UIType t);
	~TextBox();

	void Update(float dt);
};

