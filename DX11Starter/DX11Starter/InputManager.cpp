#include "InputManager.h"
#include "KeyCode.h"
#include <vector>

InputManager* InputManager::instance = 0;
static std::unordered_map<int, Binding> Actions;


InputManager * InputManager::GetInstance()
{
	if (instance == 0)
	{
		instance = new InputManager();
	}
	return instance;
}

void InputManager::Release()
{
	if (instance != 0) { delete instance; instance = 0; };
}
void InputManager::Update()
{
	CosmicInput::Update();
}
bool InputManager::IsControllerConnected()
{
	return CosmicInput::IsControllerConnected();
}
float InputManager::GetLeftStickX()
{
	return CosmicInput::GetLStickX();
}
float InputManager::GetLeftStickY()
{
	return CosmicInput::GetLStickY();
}
float InputManager::GetRightStickX()
{
	return CosmicInput::GetRStickX();
}
float InputManager::GetRightStickY()
{
	return CosmicInput::GetRStickY();
}
bool InputManager::IsKeyUp(int key)
{
	bool keyboardUp = CosmicInput::KeyUp(key);

	if (keyboardUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyDown(int key)
{
	bool keyboardDown = CosmicInput::KeyDown(key);


	if (keyboardDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyPressed(int key)
{
	bool keyboardPressed= CosmicInput::KeyPressed(key);

	if (keyboardPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsKeyReleased(int key)
{
	bool keyboardReleased = CosmicInput::KeyReleased(key);

	if (keyboardReleased)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonUp(int key)
{
	bool controllerUp = CosmicInput::ButtonUp(key);

	if (controllerUp)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonDown(int key)
{
	bool controllerDown = CosmicInput::ButtonDown(key);

	if (controllerDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonPressed(int key)
{
	bool controllerPressed = CosmicInput::ButtonPressed(key);

	if (controllerPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsButtonReleased(int key)
{
	bool controllerReleased = CosmicInput::ButtonReleased(key);

	if (controllerReleased)
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}
bool InputManager::IsActionUp(int act)
{
	Binding newBind = Actions[act];

	bool keyboardUp = CosmicInput::KeyUp(newBind.keyboard);
	bool controllerUp = CosmicInput::ButtonUp(newBind.controller);

	if (keyboardUp || controllerUp) 
	{
		return true;
	}
	else
	{
		return false;
	}

}
bool InputManager::IsActionDown(int act)
{
	Binding newBind = Actions[act];

	bool keyboardDown = CosmicInput::KeyDown(newBind.keyboard);
	bool controllerDown = CosmicInput::ButtonDown(newBind.controller);

	if (keyboardDown || controllerDown)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsActionPressed(int act)
{
	Binding newBind = Actions[act];

	bool keyboardPressed = CosmicInput::KeyPressed(newBind.keyboard);
	bool controllerPressed = CosmicInput::ButtonPressed(newBind.controller);

	if (keyboardPressed || controllerPressed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool InputManager::IsActionReleased(int act)
{
	Binding newBind = Actions[act];

	bool keyboardReleased = CosmicInput::KeyReleased(newBind.keyboard);
	bool controllerReleased = CosmicInput::ButtonReleased(newBind.controller);

	if (keyboardReleased || controllerReleased)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void InputManager::AddActionBinding(int act, Binding bind)
{
	Actions[act] = bind;
}
void InputManager::AddActionBinding(int act, int keyboardBind, int controllerBind)
{
	Actions[act] = { keyboardBind, controllerBind };
}
void InputManager::ChangeActionBinding(int act, Binding bind)
{
	Actions[act] = bind;
}
void InputManager::ChangeActionBinding(int act, int keyboardBind, int controllerBind)
{
	Actions[act] = { keyboardBind, controllerBind };
}
void InputManager::ChangeActionKeyBoardBinding(int act, int keyboardBind)
{
	Actions[act].keyboard = keyboardBind;
}
void InputManager::ChangeActionControllerBinding(int act, int controllerBind)
{
	Actions[act].controller = controllerBind;
}
InputManager::InputManager() {
	CosmicInput::Initialize();
}
