#include "CocoaPlatform.h"
#include "../Graphics.h"
#include "../Debug.h"

#include "CocoaEvents.h"
#include "CocoaOpenGL.h"

#include <time.h> // nanosleep

using Monocle::Debug;

//#define MONOCLE_MAC_USE_BUNDLE

@interface MonocleWindow : NSWindow
/* These are needed for borderless/fullscreen windows */
- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
@end

@implementation MonocleWindow
- (BOOL)canBecomeKeyWindow
{
	return YES;
}

- (BOOL)canBecomeMainWindow
{
	return YES;
}
@end

@interface MonocleView : NSView
/* The default implementation doesn't pass rightMouseDown to responder chain */
- (void)rightMouseDown:(NSEvent *)theEvent;
@end

@implementation MonocleView
- (void)rightMouseDown:(NSEvent *)theEvent
{
	[[self nextResponder] rightMouseDown:theEvent];
}
@end

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

	//  Create window
	nswindow = [[MonocleWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:FALSE];
	[nswindow makeKeyAndOrderFront:nil];

	//  Set content view
	NSView *contentView = [[MonocleView alloc] initWithFrame:rect];
	[nswindow setContentView: contentView];
	[contentView release];

	[pool release];

	return nswindow;
}

static WindowData* AttachWindowListener(NSWindow* window)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	WindowData* data = (WindowData*) calloc(1, sizeof(WindowData));
	if (!data) {
		Debug::Log("Error allocating window data");
		return NULL;
	}
	data->created = true;
	data->nswindow = window;
	data->listener = [[CocoaWindowListener alloc] init];
	[data->listener listen:data];

	[pool release];

	return data;
}

namespace Monocle
{	
	static struct timeval startTime;
    
	CocoaPlatform* CocoaPlatform::instance;
	Platform*	   CocoaPlatform::platform;
    
	void CocoaPlatform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
	{
		//  Init event loop
		Cocoa_RegisterApp();
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];        

        NSDictionary *env = [[NSProcessInfo processInfo] environment];
        if([[env objectForKey:@"MONOCLE_MAC_USE_CONTENT_PATH"] isEqualToString:@"true"]) {
            NSString *path = [env objectForKey:@"MONOCLE_CONTENT_PATH"];
            if(path) {
                this->bundleResourcesPath = [path fileSystemRepresentation] + std::string("/");
            }
        } else {
            this->bundleResourcesPath = [[[NSBundle mainBundle] resourcePath] fileSystemRepresentation];
            this->bundleResourcesPath = this->bundleResourcesPath + std::string("/Content/");
        }

		//  Create window
		window = CreateWindowCocoa(w, h);
        [window setTitle:[NSString stringWithCString:name.c_str() encoding:NSUTF8StringEncoding]];

		//  Create and attach GL context
		NSOpenGLContext* context = Cocoa_GL_CreateContext();
		if (Cocoa_GL_MakeCurrent(window, context) < 0) {
			Cocoa_GL_DeleteContext(context);
		}

		windowData = AttachWindowListener(window);

		//  Set swap interval sync to vblank
		Cocoa_GL_SetSwapInterval(1);

		//  Init internal timer
		gettimeofday(&startTime, NULL);

        [pool release];
		
		if(fullscreen)
		{	
			//CGConfigureDisplayWithDisplayMode (kCGDirectMainDisplay, CGDisplayBestModeForParameters (kCGDirectMainDisplay, 32, xRes, yRes, NULL) );						
			//[glView enterFullScreenMode:[NSScreen mainScreen] withOptions:[NSDictionary dictionaryWithObjectsAndKeys:
			//															   nil]];
			
		}
	}

	Platform* Platform::instance;
	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;
	int Platform::mouseScroll=0;

	Platform::Platform()
	{
		CocoaPlatform::instance = new CocoaPlatform();
		CocoaPlatform::platform = this;
		instance = this;

        for (int ic = 0; ic < (int) KEY_MAX; ++ic) {
            KeyCode kc = (KeyCode) ic;
            localKeymap[kc] = kc;
        }
	}

//	void Platform::Init()
//	{
//		Init("Monocle Powered", 1024, 768, 32, false);
//	}

	void Platform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
	{
		CocoaPlatform::instance->Init(name, w, h, bits, fullscreen);

		WindowData* data = CocoaPlatform::instance->windowData;
		NSRect rect = [data->nswindow contentRectForFrameRect:[data->nswindow frame]];
		instance->width  = (int)rect.size.width;
		instance->height = (int)rect.size.height;
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
	
	bool Platform::IsTouchEnabled()
	{
		return false;
	}
	
	bool Platform::IsActive()
	{
		return [NSApp isActive];
	}

    std::string Platform::GetDefaultContentPath() {
//        return CocoaPlatform::instance->bundleResourcesPath;
        return "../../Content/";
    }
}
