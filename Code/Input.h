#pragma once

#include "Platform.h"
#include "Vector2.h"

namespace Monocle
{
	class Input
	{
	public:
		Input();
		void Init();

		static bool IsMouseButtonHeld(MouseButton mouseButton);
		static bool IsMouseButtonReleased(MouseButton mouseButton);
		static bool IsMouseButtonPressed(MouseButton mouseButton);

		static Vector2 GetMousePosition();

		static bool IsKeyHeld(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);
		static bool IsKeyPressed(KeyCode keyCode);

		void Update();
		
	private:
		static Input *instance;

		bool previousKeys[KEY_MAX];
		bool currentKeys[KEY_MAX];
	
		bool previousMouseButtons[MOUSE_BUTTON_MAX];
		bool currentMouseButtons[MOUSE_BUTTON_MAX];
	};
}