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

		static bool IsMouseButtonHeld(int buttonID);
		static bool IsMouseButtonReleased(int buttonID);
		static bool IsMouseButtonPressed(int buttonID);

		static Vector2 GetMousePosition();

		static bool IsKeyHeld(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);
		static bool IsKeyPressed(KeyCode keyCode);

		void Update();
		
	private:
		static Input *instance;

		bool previousKeys [KEY_MAX];
		bool currentKeys [KEY_MAX];
	};
}