#ifdef MONOCLE_WINDOWS

#include "../Debug.h"
#include "../Game.h"
#include "../Graphics.h"

#include "WindowsPlatform.h"
#include "Mmsystem.h"
#include <WindowsX.h>

// opengl/windows init code baesd on http://nehe.gamedev.net
// keyboard code based on SDL http://www.libsdl.org/


#ifndef VK_0
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'
#endif

#define VK_SEMICOLON		0xBA
#define VK_EQUALS		0xBB
#define VK_COMMA			0xBC
#define VK_MINUS			0xBD
#define VK_PERIOD		0xBE
#define VK_SLASH		0xBF
#define VK_GRAVE		0xC0
#define VK_LBRACKET		0xDB
#define VK_BACKSLASH	0xDC
#define VK_RBRACKET		0xDD
#define VK_APOSTROPHE	0xDE
#define VK_BACKTICK		0xDF
#define VK_OEM_102		0xE2

#define REPEATED_KEYMASK	(1<<30)
#define EXTENDED_KEYMASK	(1<<24)


namespace Monocle
{	
	WindowsPlatform *WindowsPlatform::instance = NULL;

	//TODO: cleanup code, replace message boxes
	bool WindowsPlatform::CreatePlatformWindow(const char* title, int width, int height, int bits, bool fullscreenflag)
	{
		const char *windowName = "Monocle Engine\0";
		unsigned int		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

		fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

		hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance			= hInstance;							// Set The Instance
		wc.hIcon				= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground		= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName		= windowName;								// Set The Class Name

		if(bits < 0)
		{
			DEVMODE dmScreenSettings;
			if(!EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings ))
			{
				Debug::Log("Could not get display settings");
				return false;
			}
			
			bits = dmScreenSettings.dmBitsPerPel;
		}

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			Debug::Log("Failed To Register The Window Class.");
			return false;
		}
	
		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				Debug::Log("The Requested Fullscreen Mode Is Not Supported");
				return false;
				//TODO: fallback on windowed?
			}
		}

		if (fullscreen)												// Are We Still In Fullscreen Mode?
		{
			dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
			dwStyle=WS_POPUP;										// Windows Style
			//ShowCursor(FALSE);										// Hide Mouse Pointer
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;								// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									windowName,							// Class Name
									title,								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillPlatformWindow();								// Reset The 
			Debug::Log("Window Creation Error");
			return false;								// Return FALSE
		}

		static PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
	
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillPlatformWindow();								// Reset The Display
			Debug::Log("Can't Create Device Context");
			return false;								// Return FALSE
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillPlatformWindow();								// Reset The Display
			Debug::Log("Can't Find A Suitable PixelFormat");
			return false;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			KillPlatformWindow();								// Reset The Display
			Debug::Log("Can't Set The PixelFormat");
			return false;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillPlatformWindow();								// Reset The Display
			Debug::Log("Can't Create a GL Rendering Context");
			return false;								// Return FALSE
		}

		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			KillPlatformWindow();								// Reset The Display
			Debug::Log("Can't Activate The GL Rendering Context");
			return FALSE;								// Return FALSE
		}

		ShowWindow(hWnd,SW_SHOW);						// Show The Window

		CenterWindow();

		SetForegroundWindow(hWnd);
		SetFocus(hWnd);

		

		/*
		if (fullscreen)
		{
			DEVMODE settings;
			settings.dmBitsPerPel = bits;
			settings.dmPelsWidth = width;
			settings.dmPelsHeight = height;
			settings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
			bool changed = (ChangeDisplaySettings(&settings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
		}
		*/

		return true;
	}
	
	void WindowsPlatform::GetDesktopSize(int *width, int *height)
	{
		RECT	rWorkArea;
		BOOL    bResult;

		// Get the limits of the 'workarea'
		bResult = SystemParametersInfo(
			SPI_GETWORKAREA,    // system parameter to query or set
			sizeof(RECT),
			&rWorkArea,
			0);

		if (!bResult)
		{
			rWorkArea.left = rWorkArea.top = 0;
			rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
			rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
		}

		*width = rWorkArea.right;
		*height = rWorkArea.bottom;
	}

	void WindowsPlatform::CenterWindow()
	{
		if (!fullscreen)
		{
			RECT    rChild,  rWorkArea;
			int     wChild, hChild;
			int     xNew, yNew;
			BOOL    bResult;

			// Get the Height and Width of the child window
			GetWindowRect (hWnd, &rChild);
			wChild = rChild.right - rChild.left;
			hChild = rChild.bottom - rChild.top;

			// Get the limits of the 'workarea'
			bResult = SystemParametersInfo(
				SPI_GETWORKAREA,    // system parameter to query or set
				sizeof(RECT),
				&rWorkArea,
				0);
			if (!bResult) {
				rWorkArea.left = rWorkArea.top = 0;
				rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
				rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
			}

			// Calculate new X position, then adjust for workarea
			xNew = (rWorkArea.right /2) - wChild/2;

			// Calculate new Y position, then adjust for workarea
			yNew = (rWorkArea.bottom/2) - hChild/2;

			// Set it, and return
			SetWindowPos (hWnd, HWND_TOPMOST, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

	}

	void WindowsPlatform::KillPlatformWindow()								// Properly Kill The Window
	{
		if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}

		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				Debug::Log("Release Of DC And RC Failed");
			}

			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				Debug::Log("Release Rendering Context Failed");
			}
			hRC=NULL;										// Set RC To NULL
		}

		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			Debug::Log("Release Device Context Failed");
			hDC=NULL;										// Set DC To NULL
		}

		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			Debug::Log("Could Not Release hWnd");
			hWnd=NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
		{
			Debug::Log("Could Not Unregister Class");
			hInstance=NULL;									// Set hInstance To NULL
		}
	}

	LRESULT CALLBACK WindowsPlatform::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)									// Check For Windows Messages
		{
			case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;
			}

			case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			}

			case WM_SIZE:
			{
				instance->platform->WindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				instance->platform->mouseScroll = (short)HIWORD(wParam);
			}
			return(0);
		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}

	Platform *Platform::instance = NULL;

	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;
	int Platform::mouseScroll = 0;

	Platform::Platform()
	{
		WindowsPlatform::instance = new WindowsPlatform();
		instance = this;
		WindowsPlatform::instance->platform = this;

		// 1ms timer
		timeBeginPeriod(1);

		for (int i = 0; i < KEY_MAX; i++)
		{
			keys[i] = false;
			localKeymap[i] = KEY_UNDEFINED;
		}

		localKeymap[KEY_BACKSPACE] = VK_BACK;
		localKeymap[KEY_TAB] = VK_TAB;
		localKeymap[KEY_CLEAR] = VK_CLEAR;
		localKeymap[KEY_RETURN] = VK_RETURN;
		localKeymap[KEY_PAUSE] = VK_PAUSE;
		localKeymap[KEY_ESCAPE] = VK_ESCAPE;
		localKeymap[KEY_SPACE] = VK_SPACE;
		localKeymap[KEY_QUOTE] = VK_APOSTROPHE;
		localKeymap[KEY_COMMA] = VK_COMMA;
		localKeymap[KEY_MINUS] = VK_MINUS;
		localKeymap[KEY_PERIOD] = VK_PERIOD;
		localKeymap[KEY_SLASH] = VK_SLASH;
		localKeymap[KEY_0] = VK_0;
		localKeymap[KEY_1] = VK_1;
		localKeymap[KEY_2] = VK_2;
		localKeymap[KEY_3] = VK_3;
		localKeymap[KEY_4] = VK_4;
		localKeymap[KEY_5] = VK_5;
		localKeymap[KEY_6] = VK_6;
		localKeymap[KEY_7] = VK_7;
		localKeymap[KEY_8] = VK_8;
		localKeymap[KEY_9] = VK_9;
		localKeymap[KEY_SEMICOLON] = VK_SEMICOLON;
		localKeymap[KEY_EQUALS] = VK_EQUALS;
		localKeymap[KEY_LEFTBRACKET] = VK_LBRACKET;
		localKeymap[KEY_BACKSLASH] = VK_BACKSLASH;
		localKeymap[KEY_LESS] = VK_OEM_102;
		localKeymap[KEY_RIGHTBRACKET] = VK_RBRACKET;
		localKeymap[KEY_BACKQUOTE] = VK_GRAVE;
		localKeymap[KEY_BACKQUOTE] = VK_BACKTICK;
		localKeymap[KEY_A] = VK_A;
		localKeymap[KEY_B] = VK_B;
		localKeymap[KEY_C] = VK_C;
		localKeymap[KEY_D] = VK_D;
		localKeymap[KEY_E] = VK_E;
		localKeymap[KEY_F] = VK_F;
		localKeymap[KEY_G] = VK_G;
		localKeymap[KEY_H] = VK_H;
		localKeymap[KEY_I] = VK_I;
		localKeymap[KEY_J] = VK_J;
		localKeymap[KEY_K] = VK_K;
		localKeymap[KEY_L] = VK_L;
		localKeymap[KEY_M] = VK_M;
		localKeymap[KEY_N] = VK_N;
		localKeymap[KEY_O] = VK_O;
		localKeymap[KEY_P] = VK_P;
		localKeymap[KEY_Q] = VK_Q;
		localKeymap[KEY_R] = VK_R;
		localKeymap[KEY_S] = VK_S;
		localKeymap[KEY_T] = VK_T;
		localKeymap[KEY_U] = VK_U;
		localKeymap[KEY_V] = VK_V;
		localKeymap[KEY_W] = VK_W;
		localKeymap[KEY_X] = VK_X;
		localKeymap[KEY_Y] = VK_Y;
		localKeymap[KEY_Z] = VK_Z;
		localKeymap[KEY_DELETE] = VK_DELETE;

		localKeymap[KEY_KP0] = VK_NUMPAD0;
		localKeymap[KEY_KP1] = VK_NUMPAD1;
		localKeymap[KEY_KP2] = VK_NUMPAD2;
		localKeymap[KEY_KP3] = VK_NUMPAD3;
		localKeymap[KEY_KP4] = VK_NUMPAD4;
		localKeymap[KEY_KP5] = VK_NUMPAD5;
		localKeymap[KEY_KP6] = VK_NUMPAD6;
		localKeymap[KEY_KP7] = VK_NUMPAD7;
		localKeymap[KEY_KP8] = VK_NUMPAD8;
		localKeymap[KEY_KP9] = VK_NUMPAD9;
		localKeymap[KEY_KP_PERIOD] = VK_DECIMAL;
		localKeymap[KEY_KP_DIVIDE] = VK_DIVIDE;
		localKeymap[KEY_KP_MULTIPLY] = VK_MULTIPLY;
		localKeymap[KEY_KP_MINUS] = VK_SUBTRACT;
		localKeymap[KEY_KP_PLUS] = VK_ADD;

		localKeymap[KEY_UP] = VK_UP;
		localKeymap[KEY_DOWN] = VK_DOWN;
		localKeymap[KEY_RIGHT] = VK_RIGHT;
		localKeymap[KEY_LEFT] = VK_LEFT;
		localKeymap[KEY_INSERT] = VK_INSERT;
		localKeymap[KEY_HOME] = VK_HOME;
		localKeymap[KEY_END] = VK_END;
		localKeymap[KEY_PAGEUP] = VK_PRIOR;
		localKeymap[KEY_PAGEDOWN] = VK_NEXT;

		localKeymap[KEY_F1] = VK_F1;
		localKeymap[KEY_F2] = VK_F2;
		localKeymap[KEY_F3] = VK_F3;
		localKeymap[KEY_F4] = VK_F4;
		localKeymap[KEY_F5] = VK_F5;
		localKeymap[KEY_F6] = VK_F6;
		localKeymap[KEY_F7] = VK_F7;
		localKeymap[KEY_F8] = VK_F8;
		localKeymap[KEY_F9] = VK_F9;
		localKeymap[KEY_F10] = VK_F10;
		localKeymap[KEY_F11] = VK_F11;
		localKeymap[KEY_F12] = VK_F12;
		localKeymap[KEY_F13] = VK_F13;
		localKeymap[KEY_F14] = VK_F14;
		localKeymap[KEY_F15] = VK_F15;

		localKeymap[KEY_NUMLOCK] = VK_NUMLOCK;
		localKeymap[KEY_CAPSLOCK] = VK_CAPITAL;
		localKeymap[KEY_SCROLLOCK] = VK_SCROLL;
		localKeymap[KEY_RSHIFT] = VK_RSHIFT;
		localKeymap[KEY_LSHIFT] = VK_LSHIFT;
		localKeymap[KEY_RCTRL] = VK_RCONTROL;
		localKeymap[KEY_LCTRL] = VK_LCONTROL;
		localKeymap[KEY_RALT] = VK_RMENU;
		localKeymap[KEY_LALT] = VK_LMENU;
		localKeymap[KEY_RMETA] = VK_RWIN;
		localKeymap[KEY_LMETA] = VK_LWIN;

		localKeymap[KEY_HELP] = VK_HELP;

		//localKeymap[VK_PRINT] = KEY_PRINT;
		localKeymap[KEY_PRINT] = VK_SNAPSHOT;
		localKeymap[KEY_BREAK] = VK_CANCEL;
		localKeymap[KEY_MENU] = VK_APPS;
	}

	void Platform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
	{
		WindowsPlatform::instance->CreatePlatformWindow(name.c_str(), w, h, bits, fullscreen);
	}

	void Platform::Update()
	{
		mouseScroll = 0;
		// Pump messages
		MSG msg;	
		while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				Game::Quit();
				break;
			}

			if (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		mouseButtons[0] = GetAsyncKeyState(VK_LBUTTON);
		mouseButtons[1] = GetAsyncKeyState(VK_RBUTTON);
		mouseButtons[2] = GetAsyncKeyState(VK_MBUTTON);

		for (int i = 0; i < KEY_MAX; i++)
		{
			keys[i] = GetAsyncKeyState(localKeymap[i]);
		}

		POINT mousePoint;
		GetCursorPos(&mousePoint);
		ScreenToClient(WindowsPlatform::instance->hWnd, &mousePoint);
		mousePosition.x = mousePoint.x;
		mousePosition.y = mousePoint.y;
	}

	long Platform::GetMilliseconds()
	{
		return (long)timeGetTime();
	}

	bool Platform::IsKeyPressed(KeyCode keyCode)
	{
		return instance->keys[(int)keyCode];
	}

	void Platform::ShowBuffer()
	{
		SwapBuffers(WindowsPlatform::instance->hDC);
	}

	int Platform::GetWidth()
	{
		return instance->width;
	}

	int Platform::GetHeight()
	{
		return instance->height;
	}

	void Platform::WindowSizeChanged(int w, int h)
	{
		instance->width = w;
		instance->height = h;
		Graphics::Resize(w, h);
	}

	void Platform::SetMouseButton(int button, bool on)
	{
		mouseButtons[button] = on;
	}

	void Platform::SetLocalKey(int key, bool on)
	{
		KeyCode keyCode = (KeyCode)instance->localKeymap[key];
		if (keyCode == KEY_UNDEFINED)
		{
			Debug::Log("Received undefined KeyCode");
			Debug::Log(key);
		}
		else
		{
			instance->keys[instance->localKeymap[key]] = on;
		}
	}

	// does windows support touches? NO! >:(
	bool Platform::IsTouchEnabled()
	{
		return false;
	}

	bool Platform::IsActive()
	{
		return GetActiveWindow() == WindowsPlatform::instance->hWnd;
	}

	std::string Platform::GetDefaultContentPath()
	{
		return "../../Content/";
	}
}

#endif
