#pragma once
#include "UI.h"
class Image :
	public UI
{
public:
	Image(GameEntity* o, UIType t);
	~Image();

	void Update(float dt);

	void LoadTexture(ID3D11ShaderResourceView* svr);
	void FlushTexture();
};

