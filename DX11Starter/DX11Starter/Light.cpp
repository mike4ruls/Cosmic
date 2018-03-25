#include "Light.h"
#include "EngineManager.h"

Light::Light()
{
}

Light::~Light()
{
}

Light * Light::CreateDirectionalLight()
{
	EngineManager* manager = EngineManager::GetInstance();
	Light* newLight = manager->GetRender()->CreateDirectionalLight({});

	return newLight;
}

Light * Light::CreatePointLight()
{
	EngineManager* manager = EngineManager::GetInstance();
	Light* newLight = manager->GetRender()->CreatePointLight({});

	return newLight;
}

Light * Light::CreateSpotLight()
{
	EngineManager* manager = EngineManager::GetInstance();
	Light* newLight = manager->GetRender()->CreateSpotLight({});

	return newLight;
}
