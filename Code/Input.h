#pragma once

#include "Vector2.h"

namespace Monocle
{
	enum KeyCode
	{
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow
	};

	class Input
	{
	public:
		void Init();

		bool GetMouseButtonDown();
		bool GetMouseButtonUp();
		bool GetMouseButton();

		Vector2 GetMousePosition();

		bool GetKeyDown(KeyCode keyCode);
		bool GetKeyUp(KeyCode keyCode);
		bool GetKey(KeyCode keyCode);

		void Update();
	};
}