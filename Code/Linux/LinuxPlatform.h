#ifdef MONOCLE_LINUX

#pragma once

#include "../Platform.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace Monocle
{
    class LinuxPlatform
    {
    public:

        Display   *hDisplay;
        int        hScreen;
        Window     hWindow;
        GLXContext GC;

        bool	active;	// Window Active Flag Set To TRUE By Default
        bool	fullscreen; // Fullscreen Flag Set To Fullscreen Mode By Default

        static LinuxPlatform* instance;
        Platform *platform;

        LinuxPlatform()
            :	hDisplay(NULL),
            hScreen(NULL),
            hWindow(NULL),
            GC(NULL)
        {
            active = true;
            fullscreen = true;
            instance = this;
        }

        bool CreatePlatformWindow(const char* title, int width, int height, int bits, bool fullscreenflag);
        void KillPlatformWindow();
        void CenterWindow();
        static void GetDesktopSize(int *width, int *height);
    };

}
#endif
