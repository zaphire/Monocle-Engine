#pragma once

namespace Monocle
{
	enum KeyCode
	{
		KEY_UNDEFINED=-1,
		KEY_BACKSPACE=0,
		KEY_TAB,
		KEY_CLEAR,
		KEY_RETURN,
		KEY_PAUSE,
		KEY_ESCAPE,
		KEY_SPACE,
		KEY_QUOTE,
		KEY_COMMA,
		KEY_MINUS,
		KEY_PERIOD,
		KEY_SLASH,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_SEMICOLON,
		KEY_EQUALS,
		KEY_LEFTBRACKET,
		KEY_BACKSLASH,
		KEY_LESS,
		KEY_RIGHTBRACKET,
		KEY_BACKQUOTE,
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_DELETE,
		KEY_KP0,
		KEY_KP1,
		KEY_KP2,
		KEY_KP3,
		KEY_KP4,
		KEY_KP5,
		KEY_KP6,
		KEY_KP7,
		KEY_KP8,
		KEY_KP9,
		KEY_KP_PERIOD,
		KEY_KP_DIVIDE,
		KEY_KP_MULTIPLY,
		KEY_KP_MINUS,
		KEY_KP_PLUS,
		KEY_UP,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
		KEY_INSERT,
		KEY_HOME,
		KEY_END,
		KEY_PAGEUP,
		KEY_PAGEDOWN,
		KEY_F1,
		KEY_F2,
		KEY_F3,
		KEY_F4,
		KEY_F5,
		KEY_F6,
		KEY_F7,
		KEY_F8,
		KEY_F9,
		KEY_F10,
		KEY_F11,
		KEY_F12,
		KEY_F13,
		KEY_F14,
		KEY_F15,
		KEY_NUMLOCK,
		KEY_CAPSLOCK,
		KEY_SCROLLOCK,
		KEY_RSHIFT,
		KEY_LSHIFT,
		KEY_RCTRL,
		KEY_LCTRL,
		KEY_RALT,
		KEY_LALT,
		KEY_RMETA,
		KEY_LMETA,
		KEY_HELP,
		KEY_PRINT,
		KEY_BREAK,
		KEY_MENU,
		KEY_MAX = 323
	};

	class Platform
	{
	public:
		Platform();

		void Init();
		void Init(int w, int h, int bits, bool fullscreen);
		void Update();

		static long GetMilliseconds();
		static void ShowBuffer();
		static bool IsKeyPressed(KeyCode keyCode);
		static int GetWidth();
		static int GetHeight();

		bool keys[KEY_MAX];

		int localKeymap[KEY_MAX];

		void WindowSizeChanged(int w, int h);

		static Platform *instance; //TODO: refactor this (make static accessor methods instead)

	private:
		
		int width, height;
		
	};
}