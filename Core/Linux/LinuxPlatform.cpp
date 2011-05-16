#ifdef MONOCLE_LINUX

#include "LinuxPlatform.h"

#include <X11/extensions/Xrandr.h>
#include <X11/extensions/XInput2.h>
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
    //Internal time
    static struct timeval startTime;

    struct KeySymParts {
        union Data {
            uint16_t bits;
            struct { uint8_t low, high; };
        };
        static Data data;
        static Data & Get(KeySym ks) {
            data.bits = ks;
            return data;
        }
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

        fullscreen=fullscreenflag; // Set The Global Fullscreen Flag

        hDisplay = XOpenDisplay(NULL);

        if (hDisplay == NULL) {
            Debug::Log("Cannot open display\n");
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

        //Init internal time
        gettimeofday(&startTime, NULL);
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
    int Platform::mouseWheel = 0;

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

    void Platform::BindLocalKey(int local, int global) {
        //Needed to avoid key redefinition
        if (localKeymap[local] == KEY_UNDEFINED) {
            localKeymap[local] = global;
        }
    }

    void Platform::Init()
    {
        BindLocalKey(KeySymParts::Get(XK_Up).low, KEY_UP);
        BindLocalKey(KeySymParts::Get(XK_Down).low, KEY_DOWN);
        BindLocalKey(KeySymParts::Get(XK_Left).low, KEY_LEFT);
        BindLocalKey(KeySymParts::Get(XK_Right).low, KEY_RIGHT);

        BindLocalKey(KeySymParts::Get(XK_BackSpace).low, KEY_BACKSPACE);
        BindLocalKey(KeySymParts::Get(XK_Tab).low, KEY_TAB);
        BindLocalKey(KeySymParts::Get(XK_Clear).low, KEY_CLEAR);
        BindLocalKey(KeySymParts::Get(XK_Return).low, KEY_RETURN);
        BindLocalKey(KeySymParts::Get(XK_Pause).low, KEY_PAUSE);
        BindLocalKey(KeySymParts::Get(XK_Escape).low, KEY_ESCAPE);

        BindLocalKey(XK_space, KEY_SPACE);
        BindLocalKey(XK_quotedbl, KEY_QUOTE);
        BindLocalKey(XK_apostrophe, KEY_APOSTROPHE);
        BindLocalKey(XK_comma, KEY_COMMA);
        BindLocalKey(XK_minus, KEY_MINUS);
        BindLocalKey(XK_period, KEY_PERIOD);
        BindLocalKey(XK_slash, KEY_SLASH);

        BindLocalKey(XK_0, KEY_0);
        BindLocalKey(XK_1, KEY_1);
        BindLocalKey(XK_2, KEY_2);
        BindLocalKey(XK_3, KEY_3);
        BindLocalKey(XK_4, KEY_4);
        BindLocalKey(XK_5, KEY_5);
        BindLocalKey(XK_6, KEY_6);
        BindLocalKey(XK_7, KEY_7);
        BindLocalKey(XK_8, KEY_8);
        BindLocalKey(XK_9, KEY_9);

        BindLocalKey(XK_a, KEY_A);
        BindLocalKey(XK_b, KEY_B);
        BindLocalKey(XK_c, KEY_C);
        BindLocalKey(XK_d, KEY_D);
        BindLocalKey(XK_e, KEY_E);
        BindLocalKey(XK_f, KEY_F);
        BindLocalKey(XK_g, KEY_G);
        BindLocalKey(XK_h, KEY_H);
        BindLocalKey(XK_i, KEY_I);
        BindLocalKey(XK_j, KEY_J);
        BindLocalKey(XK_k, KEY_K);
        BindLocalKey(XK_l, KEY_L);
        BindLocalKey(XK_m, KEY_M);
        BindLocalKey(XK_n, KEY_N);
        BindLocalKey(XK_o, KEY_O);
        BindLocalKey(XK_p, KEY_P);
        BindLocalKey(XK_q, KEY_Q);
        BindLocalKey(XK_r, KEY_R);
        BindLocalKey(XK_s, KEY_S);
        BindLocalKey(XK_t, KEY_T);
        BindLocalKey(XK_u, KEY_U);
        BindLocalKey(XK_v, KEY_V);
        BindLocalKey(XK_w, KEY_W);
        BindLocalKey(XK_x, KEY_X);
        BindLocalKey(XK_y, KEY_Y);
        BindLocalKey(XK_z, KEY_Z);
        BindLocalKey(XK_semicolon, KEY_SEMICOLON);
        BindLocalKey(XK_equal, KEY_EQUALS);
        BindLocalKey(XK_bracketleft, KEY_LEFTBRACKET);
        BindLocalKey(XK_backslash, KEY_BACKSLASH);
        BindLocalKey(XK_less, KEY_LESS);
        BindLocalKey(XK_bracketright, KEY_RIGHTBRACKET);
        BindLocalKey(XK_grave, KEY_BACKQUOTE);

        BindLocalKey(KeySymParts::Get(XK_Delete).low, KEY_DELETE);

        BindLocalKey(KeySymParts::Get(XK_KP_0).low, KEY_KP0);
        BindLocalKey(KeySymParts::Get(XK_KP_1).low, KEY_KP1);
        BindLocalKey(KeySymParts::Get(XK_KP_2).low, KEY_KP2);
        BindLocalKey(KeySymParts::Get(XK_KP_3).low, KEY_KP3);
        BindLocalKey(KeySymParts::Get(XK_KP_4).low, KEY_KP4);
        BindLocalKey(KeySymParts::Get(XK_KP_5).low, KEY_KP5);
        BindLocalKey(KeySymParts::Get(XK_KP_6).low, KEY_KP6);
        BindLocalKey(KeySymParts::Get(XK_KP_7).low, KEY_KP7);
        BindLocalKey(KeySymParts::Get(XK_KP_8).low, KEY_KP8);
        BindLocalKey(KeySymParts::Get(XK_KP_9).low, KEY_KP9);
        BindLocalKey(KeySymParts::Get(XK_KP_Decimal).low, KEY_KP_PERIOD);
        BindLocalKey(KeySymParts::Get(XK_KP_Divide).low, KEY_KP_DIVIDE);
        BindLocalKey(KeySymParts::Get(XK_KP_Multiply).low, KEY_KP_MULTIPLY);
        BindLocalKey(XK_minus, KEY_KP_MINUS);
        BindLocalKey(XK_plus, KEY_KP_PLUS);

        BindLocalKey(KeySymParts::Get(XK_Insert).low, KEY_INSERT);
        BindLocalKey(KeySymParts::Get(XK_Home).low, KEY_HOME);
        BindLocalKey(KeySymParts::Get(XK_End).low, KEY_END);
        BindLocalKey(KeySymParts::Get(XK_Page_Up).low, KEY_PAGEUP);
        BindLocalKey(KeySymParts::Get(XK_Page_Down).low, KEY_PAGEDOWN);

        BindLocalKey(KeySymParts::Get(XK_Num_Lock).low, KEY_NUMLOCK);
        BindLocalKey(KeySymParts::Get(XK_Caps_Lock).low, KEY_CAPSLOCK);
        BindLocalKey(KeySymParts::Get(XK_Scroll_Lock).low, KEY_SCROLLOCK);
        BindLocalKey(KeySymParts::Get(XK_Shift_R).low, KEY_RSHIFT);
        BindLocalKey(KeySymParts::Get(XK_Shift_L).low, KEY_LSHIFT);
        BindLocalKey(KeySymParts::Get(XK_Control_R).low, KEY_RCTRL);
        BindLocalKey(KeySymParts::Get(XK_Control_L).low, KEY_LCTRL);
        BindLocalKey(KeySymParts::Get(XK_Alt_R).low, KEY_RALT);
        BindLocalKey(KeySymParts::Get(XK_Alt_L).low, KEY_LALT);
        BindLocalKey(KeySymParts::Get(XK_Meta_R).low, KEY_RMETA);
        BindLocalKey(KeySymParts::Get(XK_Meta_L).low, KEY_LMETA);

        BindLocalKey(KeySymParts::Get(XK_Help).low, KEY_HELP);

        BindLocalKey(KeySymParts::Get(XK_Print).low, KEY_PRINT);
        BindLocalKey(KeySymParts::Get(XK_Break).low, KEY_BREAK);
        BindLocalKey(KeySymParts::Get(XK_Menu).low, KEY_MENU);

        BindLocalKey(KeySymParts::Get(XK_F1).low, KEY_F1);
        BindLocalKey(KeySymParts::Get(XK_F2).low, KEY_F2);
        BindLocalKey(KeySymParts::Get(XK_F3).low, KEY_F3);
        BindLocalKey(KeySymParts::Get(XK_F4).low, KEY_F4);
        BindLocalKey(KeySymParts::Get(XK_F5).low, KEY_F5);
        BindLocalKey(KeySymParts::Get(XK_F6).low, KEY_F6);
        BindLocalKey(KeySymParts::Get(XK_F7).low, KEY_F7);
        BindLocalKey(KeySymParts::Get(XK_F8).low, KEY_F8);
        BindLocalKey(KeySymParts::Get(XK_F9).low, KEY_F9);
        BindLocalKey(KeySymParts::Get(XK_F10).low, KEY_F10);
        BindLocalKey(KeySymParts::Get(XK_F11).low, KEY_F11);
        BindLocalKey(KeySymParts::Get(XK_F12).low, KEY_F12);
        BindLocalKey(KeySymParts::Get(XK_F13).low, KEY_F13);
        BindLocalKey(KeySymParts::Get(XK_F14).low, KEY_F14);
        BindLocalKey(KeySymParts::Get(XK_F15).low, KEY_F15);
        Init("Monocle Powered", 800, 600, 24, false);
    }

    void Platform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
    {
        LinuxPlatform::instance->CreatePlatformWindow(name.c_str(), w, h, bits, fullscreen);
        width = w;
        height = h;
    }

    void Platform::Update()
    {
        mouseWheel = 0; //reset mouse wheel state
        XEvent event;

        while (XPending(LinuxPlatform::instance->hDisplay)) { XNextEvent(LinuxPlatform::instance->hDisplay, &event);
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
                    // TODO check if 120 is the right amount and generalise code
                    case Button4:
                        mouseWheel += 120; break;
                    case Button5:
                        mouseWheel -= 120; break;
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
        long ticks;
        struct timeval now;
        gettimeofday(&now, NULL);
        ticks = (now.tv_sec - startTime.tv_sec) * 1000 + (now.tv_usec - startTime.tv_usec) / 1000;
        return ticks;
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
            // Debug::Log("Received undefined KeyCode");
            // Debug::Log(key);
        }
        else
        {
            instance->keys[instance->localKeymap[key]] = on;
        }
    }

    bool Platform::IsActive()
    {
        return LinuxPlatform::instance->active;
    }

	std::string Platform::GetDefaultContentPath()
	{
		return "../../Content/";
	}
}

#endif
