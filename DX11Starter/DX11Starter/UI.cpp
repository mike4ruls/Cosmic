#include "UI.h"



UI::UI()
{
}

UI::UI(GameEntity* o)
{
	obj = o;
	posX = 0.0f;
	posY = 0.0f;
	posXOffset = posX;
	posYOffset = posY;
	width = 1.0f;
	height = 1.0f;
	align = Alignment::Center;
	obj->transform.Translate(0.0f, 0.0f, 3.0f);
}


UI::~UI()
{
	if (obj != nullptr) { delete obj; obj = nullptr; };
}

void UI::Update(float dt)
{
	obj->transform.position = { posX + posXOffset, posY + posYOffset, obj->transform.position.z };
	obj->Update(dt);
}

float UI::GetWidth()
{
	return width;
}

float UI::GetHeight()
{
	return height;
}

UI::Alignment UI::GetAlignment()
{
	return align;
}

void UI::SetWidth(float wd)
{
	if (width != wd)
	{
		width = wd;
		obj->transform.Scale(width, obj->transform.scale.y, obj->transform.scale.z);
		CalculateXOffsets();
	}
}

void UI::SetHeight(float he)
{
	if (height != he)
	{
		height = he;
		obj->transform.Scale(obj->transform.scale.x, height, obj->transform.scale.z);
		CalculateYOffsets();
	}
}

void UI::SetAlignment(Alignment al)
{
	align = al;
	CalculateXOffsets();
	CalculateYOffsets();
}

void UI::CalculateXOffsets()
{
	if (align == Alignment::Left)
	{
		posXOffset = (width - 1.0f) / 2.0f;
	}
	else if (align == Alignment::Right)
	{
		posXOffset = (width - 1.0f) / -2.0f;
	}
	else
	{
		posXOffset = 0.0f;
	}
}

void UI::CalculateYOffsets()
{
	if (align == Alignment::Bottom)
	{
		posYOffset = (height - 1.0f) / 2.0f;
	}
	else if (align == Alignment::Top)
	{
		posYOffset = (height - 1.0f) / -2.0f;
	}
	else
	{
		posYOffset = 0.0f;
	}
}
