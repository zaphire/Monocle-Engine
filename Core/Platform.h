#pragma once

#include "Vector2.h"
#include <string>

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
		KEY_APOSTROPHE,
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

	enum MouseButton
	{
        MOUSE_BUTTON_UNDEFINED=-1,
		MOUSE_BUTTON_LEFT=0,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_MAX
	};

	#define MONOCLE_DETECT_COLOR_DEPTH	-1

	// placeholder class for Android/iOS touch input
	// based on Unity's method for now
	class Touch
	{
	public:
		Vector2 position;
		//fingerID
		//deltaPosition
		//deltaTime
		//tapCount
		//stage
	};

	// add an option to automatically use touches as "mouse position" and "mouse buttons"

	class Platform
	{
	public:
		Platform();

		//void Init();
		void Init(const std::string &name, int w, int h, int bits, bool fullscreen);
		void Update();

		static long GetMilliseconds();
		static void ShowBuffer();
		static bool IsKeyPressed(KeyCode keyCode);
		static int GetWidth();
		static int GetHeight();

		static bool keys[KEY_MAX];
		static bool mouseButtons[MOUSE_BUTTON_MAX];
		static Vector2 mousePosition;
		static int mouseScroll;

		static void SetLocalKey(int key, bool on);
		static void SetMouseButton(int button, bool on);

		// does this platform support touches?
		static bool IsTouchEnabled();

		static bool IsActive();

		void WindowSizeChanged(int w, int h);
        
        static std::string GetDefaultContentPath();

	private:
		static Platform *instance;
		int localKeymap[KEY_MAX];
		int width, height;
        void BindLocalKey(int local, int global);
	};
}
