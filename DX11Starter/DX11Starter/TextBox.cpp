#include "TextBox.h"



TextBox::TextBox(GameEntity* o) :UI(o, UIType::TextBox)
{
}

TextBox::TextBox() : UI(UIType::TextBox)
{
}


TextBox::~TextBox()
{
}

void TextBox::Update(float dt)
{

	UpdateVars(dt);
}
