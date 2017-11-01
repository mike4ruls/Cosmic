#pragma once
#include "GameEntity.h"

class UI
{
public:
	UI();
	UI(GameEntity* o);
	~UI();

	enum Alignment {
		Center,
		Left,
		Right,
		Bottom,
		Top
	};
	GameEntity* obj;
	float posX;
	float posY;
	//int zOrder; ?
	unsigned int Id;

	void Update(float dt);
	float GetWidth();
	float GetHeight();
	UI::Alignment GetAlignment();
	void SetWidth(float wd);
	void SetHeight(float he);
	void SetAlignment(Alignment al);


private:
	Alignment align;

	float posXOffset;
	float posYOffset;

	float width;
	float height;

	void CalculateXOffsets();
	void CalculateYOffsets();
};

