#ifdef MONOCLE_WINDOWS

#include "../Debug.h"
#include "../Core.h"
#include "../Graphics.h"

#include "WindowsPlatform.h"
#include "Mmsystem.h"

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
#endif /* VK_0 */

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
			ShowCursor(FALSE);										// Hide Mouse Pointer
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;								// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
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

		SetForegroundWindow(hWnd);
		SetFocus(hWnd);

		CenterWindow();

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
		SetWindowPos (hWnd, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
			case WM_ACTIVATE:							// Watch For Window Activate Message
			{
				if (!HIWORD(wParam))						// Check Minimization State
				{
					instance->active = true;						// Program Is Active
				}
				else
				{
					instance->active = false;						// Program Is No Longer Active
				}

				return 0;								// Return To The Message Loop
			}

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

			//TODO: clean this up

			case WM_KEYDOWN:
			{
				switch (wParam)
				{
					case VK_CONTROL:
						if ( lParam&EXTENDED_KEYMASK )
							wParam = VK_RCONTROL;
						else
							wParam = VK_LCONTROL;
						break;
					case VK_SHIFT:
						/* EXTENDED trick doesn't work here */
						if ((GetKeyState(VK_LSHIFT) & 0x8000))
						{
							wParam = VK_LSHIFT;
						}
						else if ((GetKeyState(VK_RSHIFT) & 0x8000))
						{
							wParam = VK_RSHIFT;
						}
						break;
					case VK_MENU:
						if ( lParam&EXTENDED_KEYMASK )
							wParam = VK_RMENU;
						else
							wParam = VK_LMENU;
						break;
				}

				//Debug::Log("Down!");
				//Debug::Log((int)wParam);
				if (wParam >= KEY_MAX || wParam < 0)
				{
					Debug::Log("Received KeyCode out of range");
					Debug::Log((int)wParam);
				}
				else
				{
					Platform::SetLocalKey((int)wParam, true);
				}
				return 0;
			}

			case WM_KEYUP:
			{
				switch (wParam)
				{
					case VK_CONTROL:
						if ( lParam&EXTENDED_KEYMASK )
							wParam = VK_RCONTROL;
						else
							wParam = VK_LCONTROL;
						break;
					case VK_SHIFT:
						/* EXTENDED trick doesn't work here */
						if (!(GetKeyState(VK_LSHIFT) & 0x8000))
						{
							wParam = VK_LSHIFT;
						}
						else if (!(GetKeyState(VK_RSHIFT) & 0x8000))
						{
							wParam = VK_RSHIFT;
						}
						break;
					case VK_MENU:
						if ( lParam&EXTENDED_KEYMASK )
							wParam = VK_RMENU;
						else
							wParam = VK_LMENU;
						break;
				}

				//Debug::Log("Up!");
				//Debug::Log((int)wParam);
				if (wParam >= KEY_MAX || wParam < 0)
				{
					Debug::Log("Received KeyCode out of range");
					Debug::Log((int)wParam);
				}
				else
				{
					Platform::SetLocalKey((int)wParam, false);
				}
				return 0;
			}

			case WM_LBUTTONDOWN:
			{
				Platform::SetMouseButton(0, true);
				return 0;
			}

			case WM_LBUTTONUP:
			{
				Platform::SetMouseButton(0, false);
				return 0;
			}

			case WM_RBUTTONDOWN:
			{
				Platform::SetMouseButton(1, true);
				return 0;
			}
			
			case WM_RBUTTONUP:
			{
				Platform::SetMouseButton(1, false);
				return 0;
			}

			case WM_MBUTTONDOWN:
			{
				Platform::SetMouseButton(2, true);
				return 0;
			}
			
			case WM_MBUTTONUP:
			{
				Platform::SetMouseButton(2, false);
				return 0;
			}

			case WM_SIZE:
			{
				instance->platform->WindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
				return 0;
			}
		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}

	Platform *Platform::instance = NULL;

	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;

	Platform::Platform()
	{
		WindowsPlatform::instance = new WindowsPlatform();
		instance = this;
		WindowsPlatform::instance->platform = this;

		for (int i = 0; i < KEY_MAX; i++)
		{
			keys[i] = false;
			localKeymap[i] = KEY_UNDEFINED;
		}

		localKeymap[VK_BACK] = KEY_BACKSPACE;
		localKeymap[VK_TAB] = KEY_TAB;
		localKeymap[VK_CLEAR] = KEY_CLEAR;
		localKeymap[VK_RETURN] = KEY_RETURN;
		localKeymap[VK_PAUSE] = KEY_PAUSE;
		localKeymap[VK_ESCAPE] = KEY_ESCAPE;
		localKeymap[VK_SPACE] = KEY_SPACE;
		localKeymap[VK_APOSTROPHE] = KEY_QUOTE;
		localKeymap[VK_COMMA] = KEY_COMMA;
		localKeymap[VK_MINUS] = KEY_MINUS;
		localKeymap[VK_PERIOD] = KEY_PERIOD;
		localKeymap[VK_SLASH] = KEY_SLASH;
		localKeymap[VK_0] = KEY_0;
		localKeymap[VK_1] = KEY_1;
		localKeymap[VK_2] = KEY_2;
		localKeymap[VK_3] = KEY_3;
		localKeymap[VK_4] = KEY_4;
		localKeymap[VK_5] = KEY_5;
		localKeymap[VK_6] = KEY_6;
		localKeymap[VK_7] = KEY_7;
		localKeymap[VK_8] = KEY_8;
		localKeymap[VK_9] = KEY_9;
		localKeymap[VK_SEMICOLON] = KEY_SEMICOLON;
		localKeymap[VK_EQUALS] = KEY_EQUALS;
		localKeymap[VK_LBRACKET] = KEY_LEFTBRACKET;
		localKeymap[VK_BACKSLASH] = KEY_BACKSLASH;
		localKeymap[VK_OEM_102] = KEY_LESS;
		localKeymap[VK_RBRACKET] = KEY_RIGHTBRACKET;
		localKeymap[VK_GRAVE] = KEY_BACKQUOTE;
		localKeymap[VK_BACKTICK] = KEY_BACKQUOTE;
		localKeymap[VK_A] = KEY_A;
		localKeymap[VK_B] = KEY_B;
		localKeymap[VK_C] = KEY_C;
		localKeymap[VK_D] = KEY_D;
		localKeymap[VK_E] = KEY_E;
		localKeymap[VK_F] = KEY_F;
		localKeymap[VK_G] = KEY_G;
		localKeymap[VK_H] = KEY_H;
		localKeymap[VK_I] = KEY_I;
		localKeymap[VK_J] = KEY_J;
		localKeymap[VK_K] = KEY_K;
		localKeymap[VK_L] = KEY_L;
		localKeymap[VK_M] = KEY_M;
		localKeymap[VK_N] = KEY_N;
		localKeymap[VK_O] = KEY_O;
		localKeymap[VK_P] = KEY_P;
		localKeymap[VK_Q] = KEY_Q;
		localKeymap[VK_R] = KEY_R;
		localKeymap[VK_S] = KEY_S;
		localKeymap[VK_T] = KEY_T;
		localKeymap[VK_U] = KEY_U;
		localKeymap[VK_V] = KEY_V;
		localKeymap[VK_W] = KEY_W;
		localKeymap[VK_X] = KEY_X;
		localKeymap[VK_Y] = KEY_Y;
		localKeymap[VK_Z] = KEY_Z;
		localKeymap[VK_DELETE] = KEY_DELETE;

		localKeymap[VK_NUMPAD0] = KEY_KP0;
		localKeymap[VK_NUMPAD1] = KEY_KP1;
		localKeymap[VK_NUMPAD2] = KEY_KP2;
		localKeymap[VK_NUMPAD3] = KEY_KP3;
		localKeymap[VK_NUMPAD4] = KEY_KP4;
		localKeymap[VK_NUMPAD5] = KEY_KP5;
		localKeymap[VK_NUMPAD6] = KEY_KP6;
		localKeymap[VK_NUMPAD7] = KEY_KP7;
		localKeymap[VK_NUMPAD8] = KEY_KP8;
		localKeymap[VK_NUMPAD9] = KEY_KP9;
		localKeymap[VK_DECIMAL] = KEY_KP_PERIOD;
		localKeymap[VK_DIVIDE] = KEY_KP_DIVIDE;
		localKeymap[VK_MULTIPLY] = KEY_KP_MULTIPLY;
		localKeymap[VK_SUBTRACT] = KEY_KP_MINUS;
		localKeymap[VK_ADD] = KEY_KP_PLUS;

		localKeymap[VK_UP] = KEY_UP;
		localKeymap[VK_DOWN] = KEY_DOWN;
		localKeymap[VK_RIGHT] = KEY_RIGHT;
		localKeymap[VK_LEFT] = KEY_LEFT;
		localKeymap[VK_INSERT] = KEY_INSERT;
		localKeymap[VK_HOME] = KEY_HOME;
		localKeymap[VK_END] = KEY_END;
		localKeymap[VK_PRIOR] = KEY_PAGEUP;
		localKeymap[VK_NEXT] = KEY_PAGEDOWN;

		localKeymap[VK_F1] = KEY_F1;
		localKeymap[VK_F2] = KEY_F2;
		localKeymap[VK_F3] = KEY_F3;
		localKeymap[VK_F4] = KEY_F4;
		localKeymap[VK_F5] = KEY_F5;
		localKeymap[VK_F6] = KEY_F6;
		localKeymap[VK_F7] = KEY_F7;
		localKeymap[VK_F8] = KEY_F8;
		localKeymap[VK_F9] = KEY_F9;
		localKeymap[VK_F10] = KEY_F10;
		localKeymap[VK_F11] = KEY_F11;
		localKeymap[VK_F12] = KEY_F12;
		localKeymap[VK_F13] = KEY_F13;
		localKeymap[VK_F14] = KEY_F14;
		localKeymap[VK_F15] = KEY_F15;

		localKeymap[VK_NUMLOCK] = KEY_NUMLOCK;
		localKeymap[VK_CAPITAL] = KEY_CAPSLOCK;
		localKeymap[VK_SCROLL] = KEY_SCROLLOCK;
		localKeymap[VK_RSHIFT] = KEY_RSHIFT;
		localKeymap[VK_LSHIFT] = KEY_LSHIFT;
		localKeymap[VK_RCONTROL] = KEY_RCTRL;
		localKeymap[VK_LCONTROL] = KEY_LCTRL;
		localKeymap[VK_RMENU] = KEY_RALT;
		localKeymap[VK_LMENU] = KEY_LALT;
		localKeymap[VK_RWIN] = KEY_RMETA;
		localKeymap[VK_LWIN] = KEY_LMETA;

		localKeymap[VK_HELP] = KEY_HELP;

		//localKeymap[VK_PRINT] = KEY_PRINT;
		localKeymap[VK_SNAPSHOT] = KEY_PRINT;
		localKeymap[VK_CANCEL] = KEY_BREAK;
		localKeymap[VK_APPS] = KEY_MENU;
	}

	void Platform::Init()
	{
		Init(800, 600, 32, false);
	}

	void Platform::Init(int w, int h, int bits, bool fullscreen)
	{
		WindowsPlatform::instance->CreatePlatformWindow("Monocle Powered", w, h, bits, fullscreen);
		//TEMP: hack
		//width = w;
		//height = h;
	}

	void Platform::Update()
	{
		// Pump messages
		MSG msg;	
		while (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				Core::Quit();
				break;
			}

			if (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		POINT mouse;
		// get current mouse position
		GetCursorPos(&mouse);

		// get current size of the desktop
		int desktopWidth, desktopHeight;
		WindowsPlatform::GetDesktopSize(&desktopWidth, &desktopHeight);

		// adjust mouse position to fit into window
		// TODO: this might need refining
		mousePosition.x = mouse.x - (desktopWidth/2 - width/2);
		mousePosition.y = mouse.y - (desktopHeight/2 - height/2);

		/*
		printf("mouse (%d, %d)\n", mouse.x, mouse.y);
		printf("desktop (%d, %d)\n", desktopWidth, desktopHeight);
		printf("mousePosition (%f, %f)\n", mousePosition.x, mousePosition.y);
		printf("mouseButtons: %d, %d, %d", mouseButtons[0], mouseButtons[1], mouseButtons[2]);
		*/
	}

	void Platform::SetMouseButton(int button, bool on)
	{
		mouseButtons[button] = on;
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
}

#endif