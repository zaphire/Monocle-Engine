#ifdef MONOCLE_LINUX

#include "LinuxPlatform.h"

#include <X11/extensions/Xrandr.h>
#include <X11/X.h>
#include <X11/keysymdef.h>

#include <sys/time.h>

#include <cstdio>
#include <ctime>
#include <unistd.h>

#include "../Debug.h"
#include "../Game.h"
#include "../Graphics.h"

namespace Monocle
{
	struct KeySymParts {
		union Data {
			uint16_t bits;
			struct { uint8_t low, high; };
		};

		static Data data;
		
		static Data & Get(KeySym ks)
		{ data.bits = ks; return data; }
	};
	KeySymParts::Data KeySymParts::data;

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
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
            ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

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
		Window hRootWindow = DefaultRootWindow(LinuxPlatform::instance->hDisplay);
        XWindowAttributes attrs;
        XGetWindowAttributes(LinuxPlatform::instance->hDisplay, hRootWindow, &attrs);

		*width = attrs.width;
		*height = attrs.height;
	}

	void LinuxPlatform::CenterWindow()
	{
        int desktop_width;
        int desktop_height;
        LinuxPlatform::GetDesktopSize(&desktop_width, &desktop_height);

        XWindowAttributes attrs;
        XGetWindowAttributes(LinuxPlatform::instance->hDisplay, hWindow, &attrs);
        int window_width = attrs.width;
        int window_height = attrs.height;

        XWindowChanges changes;
        changes.x = (desktop_width-window_width)/2;
        changes.y = (desktop_height-window_height)/2;

        XConfigureWindow(LinuxPlatform::instance->hDisplay, hWindow,
                CWX | CWY, &changes);
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
		localKeymap[KeySymParts::Get(XK_Up).low] = KEY_UP;
		localKeymap[KeySymParts::Get(XK_Down).low] = KEY_DOWN;
		localKeymap[KeySymParts::Get(XK_Left).low] = KEY_LEFT;
		localKeymap[KeySymParts::Get(XK_Right).low] = KEY_RIGHT;

		localKeymap[KeySymParts::Get(XK_BackSpace).low] = KEY_BACKSPACE;
		localKeymap[KeySymParts::Get(XK_Tab).low] = KEY_TAB;
		localKeymap[KeySymParts::Get(XK_Clear).low] = KEY_CLEAR;
		localKeymap[KeySymParts::Get(XK_Return).low] = KEY_RETURN;
		localKeymap[KeySymParts::Get(XK_Pause).low] = KEY_PAUSE;
		localKeymap[KeySymParts::Get(XK_Escape).low] = KEY_ESCAPE;

		localKeymap[XK_space] = KEY_SPACE;
		localKeymap[XK_quotedbl] = KEY_QUOTE;
		localKeymap[XK_comma] = KEY_COMMA;
		localKeymap[XK_minus] = KEY_MINUS;
		localKeymap[XK_period] = KEY_PERIOD;
		localKeymap[XK_slash] = KEY_SLASH;

		localKeymap[XK_0] = KEY_0;
		localKeymap[XK_1] = KEY_1;
		localKeymap[XK_2] = KEY_2;
		localKeymap[XK_3] = KEY_3;
		localKeymap[XK_4] = KEY_4;
		localKeymap[XK_5] = KEY_5;
		localKeymap[XK_6] = KEY_6;
		localKeymap[XK_7] = KEY_7;
		localKeymap[XK_8] = KEY_8;
		localKeymap[XK_9] = KEY_9;

		localKeymap[XK_a] = KEY_A;
		localKeymap[XK_b] = KEY_B;
		localKeymap[XK_c] = KEY_C;
		localKeymap[XK_d] = KEY_D;
		localKeymap[XK_e] = KEY_E;
		localKeymap[XK_f] = KEY_F;
		localKeymap[XK_g] = KEY_G;
		localKeymap[XK_h] = KEY_H;
		localKeymap[XK_i] = KEY_I;
		localKeymap[XK_j] = KEY_J;
		localKeymap[XK_k] = KEY_K;
		localKeymap[XK_l] = KEY_L;
		localKeymap[XK_m] = KEY_M;
		localKeymap[XK_n] = KEY_N;
		localKeymap[XK_o] = KEY_O;
		localKeymap[XK_p] = KEY_P;
		localKeymap[XK_q] = KEY_Q;
		localKeymap[XK_r] = KEY_R;
		localKeymap[XK_s] = KEY_S;
		localKeymap[XK_t] = KEY_T;
		localKeymap[XK_u] = KEY_U;
		localKeymap[XK_v] = KEY_V;
		localKeymap[XK_w] = KEY_W;
		localKeymap[XK_x] = KEY_X;
		localKeymap[XK_y] = KEY_Y;
		localKeymap[XK_z] = KEY_Z;

		localKeymap[XK_semicolon] = KEY_SEMICOLON;
		localKeymap[XK_equal] = KEY_EQUALS;
		localKeymap[XK_bracketleft] = KEY_LEFTBRACKET;
		localKeymap[XK_backslash] = KEY_BACKSLASH;
		localKeymap[XK_less] = KEY_LESS;
		localKeymap[XK_bracketright] = KEY_RIGHTBRACKET;
		localKeymap[XK_grave] = KEY_BACKQUOTE;

		localKeymap[KeySymParts::Get(XK_Delete).low] = KEY_DELETE;

		localKeymap[KeySymParts::Get(XK_KP_0).low] = KEY_KP0;
		localKeymap[KeySymParts::Get(XK_KP_1).low] = KEY_KP1;
		localKeymap[KeySymParts::Get(XK_KP_2).low] = KEY_KP2;
		localKeymap[KeySymParts::Get(XK_KP_3).low] = KEY_KP3;
		localKeymap[KeySymParts::Get(XK_KP_4).low] = KEY_KP4;
		localKeymap[KeySymParts::Get(XK_KP_5).low] = KEY_KP5;
		localKeymap[KeySymParts::Get(XK_KP_6).low] = KEY_KP6;
		localKeymap[KeySymParts::Get(XK_KP_7).low] = KEY_KP7;
		localKeymap[KeySymParts::Get(XK_KP_8).low] = KEY_KP8;
		localKeymap[KeySymParts::Get(XK_KP_9).low] = KEY_KP9;
		localKeymap[KeySymParts::Get(XK_KP_Decimal).low] = KEY_KP_PERIOD;
		localKeymap[KeySymParts::Get(XK_KP_Divide).low] = KEY_KP_DIVIDE;
		localKeymap[KeySymParts::Get(XK_KP_Multiply).low] = KEY_KP_MULTIPLY;
		localKeymap[XK_minus] = KEY_KP_MINUS;
		localKeymap[XK_plus] = KEY_KP_PLUS;

		localKeymap[KeySymParts::Get(XK_Insert).low] = KEY_INSERT;
		localKeymap[KeySymParts::Get(XK_Home).low] = KEY_HOME;
		localKeymap[KeySymParts::Get(XK_End).low] = KEY_END;
		localKeymap[KeySymParts::Get(XK_Page_Up).low] = KEY_PAGEUP;
		localKeymap[KeySymParts::Get(XK_Page_Down).low] = KEY_PAGEDOWN;

		localKeymap[KeySymParts::Get(XK_Num_Lock).low] = KEY_NUMLOCK;
		localKeymap[KeySymParts::Get(XK_Caps_Lock).low] = KEY_CAPSLOCK;
		localKeymap[KeySymParts::Get(XK_Scroll_Lock).low] = KEY_SCROLLOCK;
		localKeymap[KeySymParts::Get(XK_Shift_R).low] = KEY_RSHIFT;
		localKeymap[KeySymParts::Get(XK_Shift_L).low] = KEY_LSHIFT;
		localKeymap[KeySymParts::Get(XK_Control_R).low] = KEY_RCTRL;
		localKeymap[KeySymParts::Get(XK_Control_L).low] = KEY_LCTRL;
		localKeymap[KeySymParts::Get(XK_Alt_R).low] = KEY_RALT;
		localKeymap[KeySymParts::Get(XK_Alt_L).low] = KEY_LALT;
		localKeymap[KeySymParts::Get(XK_Meta_R).low] = KEY_RMETA;
		localKeymap[KeySymParts::Get(XK_Meta_L).low] = KEY_LMETA;

		localKeymap[KeySymParts::Get(XK_Help).low] = KEY_HELP;

		localKeymap[KeySymParts::Get(XK_Print).low] = KEY_PRINT;
		localKeymap[KeySymParts::Get(XK_Break).low] = KEY_BREAK;
		localKeymap[KeySymParts::Get(XK_Menu).low] = KEY_MENU;

		localKeymap[KeySymParts::Get(XK_F1).low] = KEY_F1;
		localKeymap[KeySymParts::Get(XK_F2).low] = KEY_F2;
		localKeymap[KeySymParts::Get(XK_F3).low] = KEY_F3;
		localKeymap[KeySymParts::Get(XK_F4).low] = KEY_F4;
		localKeymap[KeySymParts::Get(XK_F5).low] = KEY_F5;
		localKeymap[KeySymParts::Get(XK_F6).low] = KEY_F6;
		localKeymap[KeySymParts::Get(XK_F7).low] = KEY_F7;
		localKeymap[KeySymParts::Get(XK_F8).low] = KEY_F8;
		localKeymap[KeySymParts::Get(XK_F9).low] = KEY_F9;
		localKeymap[KeySymParts::Get(XK_F10).low] = KEY_F10;
		localKeymap[KeySymParts::Get(XK_F11).low] = KEY_F11;
		localKeymap[KeySymParts::Get(XK_F12).low] = KEY_F12;
		localKeymap[KeySymParts::Get(XK_F13).low] = KEY_F13;
		localKeymap[KeySymParts::Get(XK_F14).low] = KEY_F14;
		localKeymap[KeySymParts::Get(XK_F15).low] = KEY_F15;

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
			case Expose: {
				XExposeEvent &expose = event.xexpose;
				glViewport(0, 0, expose.width, expose.height);
				ShowBuffer();
				} break;
			case KeyPress:
			case KeyRelease: {
				KeySym keysym = XLookupKeysym(&event.xkey, 0);
				Platform::SetLocalKey(KeySymParts::Get(keysym).low,
				    event.type == KeyPress ? true : false);
				} break;
            case ButtonPress:
            case ButtonRelease: {
                int button = MOUSE_BUTTON_UNDEFINED;

                switch(event.xbutton.button) {
                case Button1: // left click
                    button = MOUSE_BUTTON_LEFT; break;
                case Button2: // middle click
                    button = MOUSE_BUTTON_MIDDLE; break;
                case Button3: // right click
                    button = MOUSE_BUTTON_RIGHT; break;
                }

                Platform::SetMouseButton(button,
                    event.type == ButtonPress);
                Platform::mousePosition = Vector2(event.xbutton.x,
                    event.xbutton.y);
                } break;
            case MotionNotify: {
                Platform::mousePosition = Vector2(event.xmotion.x,
                    event.xmotion.y);
                } break;
			}
		}
	}

	void Platform::SetMouseButton(int button, bool on)
	{
        if(button == MOUSE_BUTTON_UNDEFINED)
        {
            Debug::Log("Received unknown button");
            Debug::Log(button);
        } else {
            mouseButtons[button] = on;
        }
	}

	long Platform::GetMilliseconds()
	{
		timeval ts;
		gettimeofday(&ts, NULL);
		return (long)((ts.tv_sec * 1000) + (ts.tv_usec/1000.0) + 0.5);
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
