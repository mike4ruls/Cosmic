#include "CosmicInput.h"
#include <Windows.h>

using namespace CosmicInput;
bool allKeys[256];

int CosmicInput::Initialize()
{
	return 0;
}

void CosmicInput::InitButtonMap()
{
}

void CosmicInput::SetButton(int key, int value)
{
}

void CosmicInput::Update()
{
}
bool CosmicInput::KeyDown(int key)
{
	if (GetKeyState(key) & 0x8000) {
		allKeys[key] = true;
		return true;
	}
	else {
		allKeys[key] = false;
		return false;
	}
}
bool CosmicInput::KeyUp(int key)
{
	if (KeyDown(key)) {
		return false;
	}
	else {
		return true;
	}
}
bool CosmicInput::KeyPressed(int key)
{
	if (!allKeys[key]) {
		if (KeyDown(key)) {
			return true;
		}
		return false;
	}
	else {
		KeyDown(key);
		return false;
	}
}
bool CosmicInput::KeyReleased(int key)
{
	if (allKeys[key]) {
		if (!KeyDown(key)) {
			return true;
		}
		return false;
	}
	else {
		KeyDown(key);
		return false;
	}
}
bool CosmicInput::ButtonDown(int key)
{
	return false;
}

bool CosmicInput::ButtonUp(int key)
{
	return false;
}

bool CosmicInput::ButtonPressed(int key)
{
	return false;
}

bool CosmicInput::ButtonReleased(int key)
{
	return false;
}

InputType CosmicInput::GetInputType()
{
	return InputType();
}
