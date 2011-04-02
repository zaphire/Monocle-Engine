#ifdef MONOCLE_WINDOWS

#pragma once

#include "../Platform.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Monocle
{
	class WindowsPlatform
	{
	public:

		HGLRC           hRC;							// Permanent Rendering Context
		HDC             hDC;							// Private GDI Device Context
		HWND            hWnd;							// Holds Our Window Handle
		HINSTANCE       hInstance;						// Holds The Instance Of The Application

		//bool	active;									// Window Active Flag Set To TRUE By Default
		bool	fullscreen;								// Fullscreen Flag Set To Fullscreen Mode By Default

		static WindowsPlatform* instance;
		Platform *platform;

		WindowsPlatform()
			:	hRC(NULL),
				hDC(NULL),
				hWnd(NULL),
				hInstance(NULL)
		{
			//active = true;
			fullscreen = true;
			instance = this;
		}

		bool CreatePlatformWindow(const char* title, int width, int height, int bits, bool fullscreenflag);
		void KillPlatformWindow();
		static LRESULT CALLBACK WndProc(	HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam);
		void CenterWindow();
		static void GetDesktopSize(int *width, int *height);
	};

}

#endif