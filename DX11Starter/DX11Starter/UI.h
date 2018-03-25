#pragma once
#include "GameEntity.h"

class UI
{
public:
	enum UIType {
		TextBox,
		Button,
		Image
	}uiType;

	UI();
	UI(GameEntity* o, UIType t);
	UI(UIType t);
	~UI();

	enum Alignment {
		Center,
		Left,
		Right,
		Bottom,
		Top
	};
	GameEntity* obj;
	DirectX::XMFLOAT4 uiSurColor;
	float posX;
	float posY;
	//int zOrder; ?
	unsigned int Id;

	virtual void Update(float dt) = 0;
	void UpdateVars(float dt);
	float GetWidth();
	float GetHeight();
	float GetXOffSet();
	float GetYOffSet();
	UI::Alignment GetAlignment();
	void SetWidth(float wd);
	void SetHeight(float he);
	void SetAlignment(Alignment al);
	void SetUIColor(DirectX::XMFLOAT4 color);
	void SetVisibility(bool vis);
	void SetActive(bool act);

	void Destroy();

protected:
	Alignment align;

	float posXOffset;
	float posYOffset;

	float width;
	float height;

private:

	void CalculateXOffsets();
	void CalculateYOffsets();
};

