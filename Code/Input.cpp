#include "Input.h"
#include <stdio.h> // for null :P
#include "Debug.h"

namespace Monocle
{
	Input *Input::instance = NULL;

	Input::Input()
	{
		instance = this;
	}

	void Input::Init()
	{
		for (int i = 0; i < (int)KEY_MAX; i++)
			previousKeys[i] = currentKeys[i] = false;
	}

	void Input::Update()
	{
		for (int i = 0; i < (int)KEY_MAX; i++)
		{
			previousKeys[i] = currentKeys[i];
			currentKeys[i] = Platform::instance->keys[i];
		}
	}

	bool Input::IsKeyHeld(KeyCode keyCode)
	{
		return instance->currentKeys[(int)keyCode];
	}

	bool Input::IsKeyPressed(KeyCode keyCode)
	{
		return instance->currentKeys[(int)keyCode] && !instance->previousKeys[(int)keyCode];
	}

	bool Input::IsKeyReleased(KeyCode keyCode)
	{
		return !instance->currentKeys[(int)keyCode] && instance->previousKeys[(int)keyCode];
	}

	// TODO:

	bool IsMouseButtonHeld(int buttonID)
	{
		return false;
	}

	bool IsMouseButtonReleased(int buttonID)
	{
		return false;
	}

	bool IsMouseButtonPressed(int buttonID)
	{
		return false;
	}
}