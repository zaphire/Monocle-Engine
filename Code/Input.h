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

		bool IsMouseButtonDown();
		bool IsMouseButtonUp();
		bool IsMouseButtonPressed();

		Vector2 GetMousePosition();

		static bool IsKeyDown(KeyCode keyCode);
		static bool IsKeyReleased(KeyCode keyCode);
		static bool IsKeyPressed(KeyCode keyCode);

		void Update();
		
		static Input *instance;

	private:
		bool previousKeys [KEY_MAX];
		bool currentKeys [KEY_MAX];
	};
}