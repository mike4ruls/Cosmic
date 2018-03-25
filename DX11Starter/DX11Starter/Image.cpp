#include "Image.h"



Image::Image(GameEntity* o) :UI(o, UIType::Image)
{
}


Image::Image() : UI(UIType::Image)
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
	obj->renderingComponent->mat.LoadSurfaceTexture(svr);
}

void Image::FlushTexture()
{
	obj->renderingComponent->mat.FlushSurfaceTexture();
}
