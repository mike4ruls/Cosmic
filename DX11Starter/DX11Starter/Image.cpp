#include "Image.h"



Image::Image(GameEntity* o, UIType t) :UI(o, t)
{
}


Image::~Image()
{
}

void Image::Update(float dt)
{

	UpdateVars(dt);
}

void Image::LoadTexture(ID3D11ShaderResourceView * svr)
{
	obj->renderingComponent.mat.LoadSurfaceTexture(svr);
}

void Image::FlushTexture()
{
	obj->renderingComponent.mat.FlushSurfaceTexture();
}
