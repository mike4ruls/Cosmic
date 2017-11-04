#include "Button.h"
#include "CosmicEngine.h"


Button::Button(GameEntity* o, UIType t, void* eng):UI(o, t)
{
	engine = eng;
	prevMousePos = MouseScreen();
	prevMousePos.x = 0.0f;
	prevMousePos.y = 0.0f;
}


Button::~Button()
{
}

void Button::Update(float dt)
{
	if (obj->isActive)
	{
		CosmicEngine* en = (CosmicEngine*)engine;
		UpdateVars(dt);

		if (prevMousePos.x != en->currentMousePos.x || prevMousePos.y != en->currentMousePos.y)
		{
			CheckHovering();
		}

		if (isHovering)
		{
			obj->renderingComponent.mat.surfaceColor = uiSurColor;
		}
		else
		{
			obj->renderingComponent.mat.surfaceColor = { uiSurColor.x * 0.3f, uiSurColor.y * 0.3f, uiSurColor.z * 0.3f, 1.0f };
		}

		prevMousePos.x = en->currentMousePos.x;
		prevMousePos.y = en->currentMousePos.y;
	}
}

void Button::CheckHovering()
{
	maxX = (obj->transform.position.x * 0.5f) + (width / 2.0f);
	minX = (obj->transform.position.x * 0.5f) - (width / 2.0f);
	maxY = (obj->transform.position.y * 0.5f) + (height / 2.0f);
	minY = (obj->transform.position.y * 0.5f) - (height / 2.0f);

	CosmicEngine* en= (CosmicEngine*)engine;
	if(en->currentMousePos.x < minX || en->currentMousePos.x > maxX)
	{
		isHovering = false;
		return;
	}
	if (en->currentMousePos.y < minY || en->currentMousePos.y > maxY)
	{
		isHovering = false;
		return;
	}

	isHovering = true;
}

bool Button::IsClicked()
{
	CosmicEngine* en = (CosmicEngine*)engine;
	if(isHovering && en->click)
	{
		return true;
	}

	return false;
}

void Button::LoadTexture(ID3D11ShaderResourceView * svr)
{
	obj->renderingComponent.mat.LoadSurfaceTexture(svr);
}

void Button::FlushTexture()
{
	obj->renderingComponent.mat.FlushSurfaceTexture();
}
