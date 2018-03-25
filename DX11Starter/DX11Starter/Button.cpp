#include "Button.h"


Button::Button(GameEntity* o):UI(o, UIType::Button)
{
	inputManager = InputManager::GetInstance();
	prevMousePos = MouseScreen();
	prevMousePos.x = 0.0f;
	prevMousePos.y = 0.0f;
	constantHighlight = false;
	isHovering = false;
}

Button::Button() :UI(UIType::Button)
{
	inputManager = InputManager::GetInstance();
	prevMousePos = MouseScreen();
	prevMousePos.x = 0.0f;
	prevMousePos.y = 0.0f;
	constantHighlight = false;
	isHovering = false;
}


Button::~Button()
{
}

void Button::Update(float dt)
{
	if (obj->isActive)
	{
		UpdateVars(dt);

		if (prevMousePos.x != inputManager->currentMousePos.x || prevMousePos.y != inputManager->currentMousePos.y)
		{
			CheckHovering();
		}

		if (isHovering || constantHighlight)
		{
			obj->renderingComponent->mat.surfaceColor = uiSurColor;
		}
		else
		{
			obj->renderingComponent->mat.surfaceColor = { uiSurColor.x * 0.3f, uiSurColor.y * 0.3f, uiSurColor.z * 0.3f, 1.0f };
		}

		prevMousePos.x = inputManager->currentMousePos.x;
		prevMousePos.y = inputManager->currentMousePos.y;
	}
}

void Button::CheckHovering()
{
	maxX = (obj->transform.position.x * 0.5f) + (width / 2.0f);
	minX = (obj->transform.position.x * 0.5f) - (width / 2.0f);
	maxY = (obj->transform.position.y * 0.5f) + (height / 2.0f);
	minY = (obj->transform.position.y * 0.5f) - (height / 2.0f);

	if(inputManager->currentMousePos.x < minX || inputManager->currentMousePos.x > maxX)
	{
		isHovering = false;
		return;
	}
	if (inputManager->currentMousePos.y < minY || inputManager->currentMousePos.y > maxY)
	{
		isHovering = false;
		return;
	}

	isHovering = true;
}

bool Button::IsClicked()
{
	if(isHovering && inputManager->click && !inputManager->prevClick)
	{
		return true;
	}

	return false;
}

void Button::LoadTexture(ID3D11ShaderResourceView * svr)
{
	obj->renderingComponent->mat.LoadSurfaceTexture(svr);
}

void Button::FlushTexture()
{
	obj->renderingComponent->mat.FlushSurfaceTexture();
}
