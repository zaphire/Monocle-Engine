#pragma once

// deals with managing
// - windows
// - basic mouse, keyboard input wrappers
// - basic touch input wrappers

namespace Monocle
{
	enum KeyCode
	{
		KCLeftArrow,
		KCRightArrow,
		KCUpArrow,
		KCDownArrow,
		KCMax
	};

	class Platform
	{
	public:
		Platform();

		void Init();
		bool IsKeyPressed(KeyCode keyCode);
		void Update();

		static long GetMilliseconds();

		int keys[KCMax];
	};
}
