#include "TextBox.h"



TextBox::TextBox(GameEntity* o, UIType t) :UI(o, t)
{
}


TextBox::~TextBox()
{
}

void TextBox::Update(float dt)
{

	UpdateVars(dt);
}
