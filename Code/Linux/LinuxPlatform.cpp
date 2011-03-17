#ifdef MONOCLE_LINUX

#include "LinuxPlatform.h"

#include<X11/X.h>
#include<X11/extensions/Xrandr.h>

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../Debug.h"
#include "../Core.h"
#include "../Graphics.h"

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
	LinuxPlatform *LinuxPlatform::instance = NULL;

	//TODO: cleanup code, replace message boxes
	bool LinuxPlatform::CreatePlatformWindow(const char* title, int width, int height, int bits, bool fullscreenflag)
	{
		const char *windowName = "Monocle Engine\0";
		Window hRootWindow;
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, bits, GLX_DOUBLEBUFFER, None };
		XVisualInfo *vi;
		Colormap cmap;
		XSetWindowAttributes swa;

		fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

		hDisplay = XOpenDisplay(NULL);
		if (hDisplay == NULL) {
			fprintf(stderr, "Cannot open display\n");
			return(false);
		}

		hRootWindow = DefaultRootWindow(hDisplay);
		hScreen = DefaultScreen(hDisplay);

		vi = glXChooseVisual(hDisplay, 0, att);

		if(vi == NULL) {
			fprintf(stderr, "No visual was found.\n");
			return(false);
		}
		cmap = XCreateColormap(hDisplay, hRootWindow, vi->visual, AllocNone);

		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask;

		hWindow = XCreateWindow(
		    hDisplay,                 // display
		    hRootWindow,              // parent
		    0, 0, width, height,      // rect
		    1,                        // border width
		    bits,                     // depth
		    InputOutput,              // class
		    vi->visual,               // visual
		    CWColormap | CWEventMask, // value mask
		    &swa);
		XMapWindow(hDisplay, hWindow);
		XStoreName(hDisplay, hWindow, title);

		GC = glXCreateContext(hDisplay, vi, NULL, GL_TRUE);
		glXMakeCurrent(hDisplay, hWindow, GC);
		glEnable(GL_DEPTH_TEST);

		// TODO: IMPLEMENT fullscreen

		CenterWindow();

		return true;
	}
	
	void LinuxPlatform::GetDesktopSize(int *width, int *height)
	{
		*width = 800;
		*height = 600;
		// TODO: IMPLEMENT
	}

	void LinuxPlatform::CenterWindow()
	{
		// TODO: IMPLEMENT
	}

	void LinuxPlatform::KillPlatformWindow()
	{
		glXMakeCurrent(hDisplay, None, NULL);
		glXDestroyContext(hDisplay, GC);
		XDestroyWindow(hDisplay, hWindow);
		XCloseDisplay(hDisplay);
	}

	Platform *Platform::instance = NULL;

	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;

	Platform::Platform()
	{
		LinuxPlatform::instance = new LinuxPlatform();
		instance = this;
		LinuxPlatform::instance->platform = this;

		for (int i = 0; i < KEY_MAX; i++)
		{
			keys[i] = false;
			localKeymap[i] = KEY_UNDEFINED;
		}
	}

	void Platform::Init()
	{
		Init(800, 600, 24, false);
	}

	void Platform::Init(int w, int h, int bits, bool fullscreen)
	{
		LinuxPlatform::instance->CreatePlatformWindow("Monocle Powered", w, h, bits, fullscreen);
		width = w;
		height = h;
	}

	void Platform::Update()
	{
		XEvent event;

		while (XPending(LinuxPlatform::instance->hDisplay)) {
			XNextEvent(LinuxPlatform::instance->hDisplay, &event);
			switch(event.type) {
			case Expose:
				glViewport(0, 0, width, height);
				ShowBuffer();
				break;
			case KeyPress:
				break;
			}
		}
	}

	void Platform::SetMouseButton(int button, bool on)
	{
		mouseButtons[button] = on;
	}

	long Platform::GetMilliseconds()
	{
		return (long)time(0);
	}

	bool Platform::IsKeyPressed(KeyCode keyCode)
	{
		return instance->keys[(int)keyCode];
	}

	void Platform::ShowBuffer()
	{
		glXSwapBuffers(LinuxPlatform::instance->hDisplay, LinuxPlatform::instance->hWindow);
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
	}
}

#endif
