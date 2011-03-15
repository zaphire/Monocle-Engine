#pragma once

// deals with managing
// - windows
// - basic mouse, keyboard input wrappers
// - basic touch input wrappers

namespace Monocle
{
	enum KeyCode
	{
		LeftArrow,
		RightArrow,
		UpArrow,
		DownArrow,
		Max
	};

	class Platform
	{
	public:
		Platform();

		void Init();
		bool IsKeyPressed(KeyCode keyCode);
		void Update();

		static long GetMilliseconds();

		int keys[KeyCode::Max];
	};
}