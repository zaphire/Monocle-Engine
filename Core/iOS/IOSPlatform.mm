//
//  IOSPlatform.m
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/28/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#ifdef MONOCLE_IOS

#import "IOSPlatform.h"
#include "../Graphics.h"
#include "../Debug.h"
#include "../Audio/Audio.h"

namespace Monocle
{	
    AudioDecoder *makeCAFDecFunc( AudioAsset *asset );
    AudioDecoder *makeSTBOggFunc( AudioAsset *asset );
    
	static struct timeval startTime;
    
	IOSPlatform*   IOSPlatform::instance;
	Platform*	   IOSPlatform::platform;
    
    EAGLView *IOSPlatform::view;
    UIWindow *IOSPlatform::window;
    
	void IOSPlatform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
    {
		// We are already initiated
        //  Init internal timer
		gettimeofday(&startTime, NULL);
	}
    
	Platform* Platform::instance;
	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;
	int Platform::mouseScroll=0;
    Touch Platform::touches[TOUCHES_MAX];
    int Platform::numTouches=0;
    
	Platform::Platform()
	{
		IOSPlatform::instance = new IOSPlatform();
		IOSPlatform::platform = this;
		instance = this;
        orientation = PLATFORM_ORIENTATION_PORTRAIT;
        
        for (int ic = 0; ic < (int) KEY_MAX; ++ic) {
            KeyCode kc = (KeyCode) ic;
            localKeymap[kc] = kc;
        }
        
        for (int it = 0; it < (int) TOUCHES_MAX; it++ ) {
            Touch *t = &Platform::touches[it];
            t->phase = TOUCH_PHASE_NONE;
        }
	}
    
    //	void Platform::Init()
    //	{
    //		Init("Monocle Powered", 1024, 768, 32, false);
    //	}
    
	void Platform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
	{
		IOSPlatform::instance->Init(name, w, h, bits, fullscreen);
        
        UIWindow *window = IOSPlatform::instance->window;
        
		instance->width  = (int)[window bounds].size.width;
		instance->height = (int)[window bounds].size.height;
        instance->orientation = PLATFORM_ORIENTATION_PORTRAIT;
        
        Audio::RegisterDecoder(makeCAFDecFunc, "caf");
        Audio::RegisterDecoder(makeSTBOggFunc, "ogg g2m");
	}
    
    PlatformOrientation Platform::GetOrientation()
    {
        return instance->orientation;
    }
    
    void Platform::PlatformOrientationChanged( PlatformOrientation orientation )
    {
        if (instance->orientation == orientation)
            return;
        
        instance->orientation = orientation;
        
        /*switch (instance->orientation)
        {
            case PLATFORM_ORIENTATION_LANDSCAPE_LEFT:
            case PLATFORM_ORIENTATION_LANDSCAPE_RIGHT:
                Graphics::Resize(Platform::GetHeight(),Platform::GetWidth());
                break;
            default:
                Graphics::Resize(Platform::GetWidth(),Platform::GetHeight());
                break;
        }*/
        Graphics::Resize(Platform::GetWidth(),Platform::GetHeight());
    }
    
	void Platform::Update()
	{
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
//		Cocoa_GL_SwapWindow();
        EAGLView *view = IOSPlatform::view;
        [view swapBuffers];
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
        // This probably doesn't happen. :P
		/*instance->width = w;
		instance->height = h;
		Graphics::Resize(w, h);*/
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
		return true;
	}
	
	bool Platform::IsActive()
	{
		return true;
	}
    
    std::string Platform::GetDefaultContentPath() {
        //        return CocoaPlatform::instance->bundleResourcesPath;
//        return "../../Content/";
        NSBundle* myBundle = [NSBundle mainBundle];
        NSString* resourceDir = [myBundle resourcePath];
        const char *resourceDirCStyle = [resourceDir UTF8String];
        
        return std::string(resourceDirCStyle) + "/Content/";
    }
    
    void Platform::ErrorShutdown( std::string msg )
    {
        NSString *errorMessage = [NSString stringWithCString:msg.c_str() 
                                                    encoding:[NSString defaultCStringEncoding]];
        [NSException raise:errorMessage format:@"%s" , errorMessage];
    }
}

#endif
