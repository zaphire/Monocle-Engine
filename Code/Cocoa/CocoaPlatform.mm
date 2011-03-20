#include "CocoaPlatform.h"
#include "../Graphics.h"
#include "Debug.h"

#include "CocoaEvents.h"
#include "CocoaOpenGL.h"
//#include "CocoaWindow.h"

#include <time.h> // nanosleep

static struct timeval startTime;

static NSWindow* CreateWindowCocoa(int w, int h)
{
    NSAutoreleasePool *pool;
    NSWindow *nswindow;
    NSRect rect;
    unsigned int style;

    pool = [[NSAutoreleasePool alloc] init];

    rect.origin.x = 0;
    rect.origin.y = 0;
    rect.size.width = w;
    rect.size.height = h;
    rect.origin.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - rect.origin.y - rect.size.height;

    style = (NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask);

    nswindow = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:FALSE];
    [nswindow makeKeyAndOrderFront:nil];

    [pool release];

    return nswindow;
}

static void DestroyWindowCocoa(void *window)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSWindow *nswindow = (NSWindow *)window;

    [nswindow close];
    [pool release];
}

namespace Monocle
{	
    CocoaPlatform* CocoaPlatform::instance;
    
    CocoaPlatform::CocoaPlatform()
    {
    }

	bool CocoaPlatform::Init(int w, int h, int bits, bool fullscreen)
	{
		//  Init event loop
		Cocoa_RegisterApp();

		//  Create window
        window = CreateWindowCocoa(w, h);        

		//  Create and attach GL context
		NSOpenGLContext* context = Cocoa_GL_CreateContext();
		if (Cocoa_GL_MakeCurrent(window, context) < 0) {
			Cocoa_GL_DeleteContext(context);
			return false;
		}

		//  Set swap interval sync to vblank
		Cocoa_GL_SetSwapInterval(1);

		//  Init internal timer
        gettimeofday(&startTime, NULL);

		return true;
	}
    
    Platform* Platform::instance;
	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;

	Platform::Platform()
	{
        CocoaPlatform::instance = new CocoaPlatform();
		instance = this;
	}

	void Platform::Init()
	{
		Init(800, 600, 32, false);
	}

	void Platform::Init(int w, int h, int bits, bool fullscreen)
	{
		if (!CocoaPlatform::instance->Init(w, h, bits, fullscreen))
			Debug::Log("Error initializing Monocle window/context");

		// XXX temporary hack
		instance->width  = w;
		instance->height = h;
	}

	void Platform::Update()
	{
        Cocoa_PumpEvents();
	}

	void Platform::SetMouseButton(int button, bool on)
	{
		mouseButtons[button] = on;
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
		// XXX fill in args
        Cocoa_GL_SwapWindow();
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
		KeyCode keyCode = (KeyCode)(instance->localKeymap[key]);
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
