#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include "Platforms/Backend.h"
#include <windows.h>

namespace Native
{
	namespace Windows
	{
		class Backend : Native::Backend
		{
		public:
			void Update();
			void ShowBuffer();
			long GetMilliseconds();

			// This makes me want to stab my eyes out, but it's necessary >_>!

			HGLRC           hRC;							// Permanent Rendering Context
			HDC             hDC;							// Private GDI Device Context
			HWND            hWnd;							// Holds Our Window Handle
			HINSTANCE       hInstance;						// Holds The Instance Of The Application

			static Backend * instance;
			bool	active;									// Window Active Flag Set To TRUE By Default
			bool	fullscreen;								// Fullscreen Flag Set To Fullscreen Mode By Default

			Backend()
				:	hRC(NULL),
					hDC(NULL),
					hWnd(NULL),
					hInstance(NULL)
			{
				active = true;
				fullscreen = true;
				instance = this;

				instance->CreatePlatformWindow("Title", 800, 600, 32, false);
			}

			bool CreatePlatformWindow(const char* title, int width, int height, int bits, bool fullscreenflag);
			void KillPlatformWindow();
			static LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam);
		};
	}
}
#endif