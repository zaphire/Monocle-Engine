#pragma once

#include "Platform.h"
#include "Vector2.h"

namespace Monocle
{
	class Input
	{
	public:
		void Init();

		bool IsMouseButtonDown();
		bool IsMouseButtonUp();
		bool IsMouseButtonPressed();

		Vector2 GetMousePosition();

		bool IsKeyDown(KeyCode keyCode);
		bool IsKeyUp(KeyCode keyCode);
		bool IsKeyPressed(KeyCode keyCode);

		void Update();
	};
}