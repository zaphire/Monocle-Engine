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

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
			previousMouseButtons[i] = currentMouseButtons[i] = false;
	}

	void Input::Update()
	{
		for (int i = 0; i < (int)KEY_MAX; i++)
		{
			previousKeys[i] = currentKeys[i];
			currentKeys[i] = Platform::keys[i];
		}

		for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
		{
			previousMouseButtons[i] = currentMouseButtons[i];
			currentMouseButtons[i] = Platform::mouseButtons[i];
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

	Vector2 Input::GetMousePosition()
	{
		return Platform::mousePosition;
	}

	bool Input::IsMouseButtonHeld(MouseButton mouseButton)
	{
		return Platform::mouseButtons[(int)mouseButton];
	}

	bool Input::IsMouseButtonReleased(MouseButton mouseButton)
	{
		return !instance->currentMouseButtons[(int)mouseButton] && instance->previousMouseButtons[(int)mouseButton];
	}

	bool Input::IsMouseButtonPressed(MouseButton mouseButton)
	{
		return instance->currentMouseButtons[(int)mouseButton] && !instance->previousMouseButtons[(int)mouseButton];
	}
}