#import <OpenGL/OpenGL.h>
#import <OpenGL/CGLRenderers.h>
#import <AppKit/AppKit.h>

#include "../Debug.h"

using Monocle::Debug;

NSOpenGLContext*
Cocoa_GL_CreateContext()
{
    NSAutoreleasePool *pool;
    NSOpenGLPixelFormatAttribute attr[32];
    NSOpenGLPixelFormat *fmt;
    NSOpenGLContext *context;
    int i = 0;

    pool = [[NSAutoreleasePool alloc] init];

    // XXX initialize more pixel format members
    
    attr[i++] = NSOpenGLPFADoubleBuffer;
    attr[i++] = NSOpenGLPFAAccelerated;
    attr[i++] = NSOpenGLPFAColorSize;
    attr[i++] = 32.0;
    attr[i++] = NSOpenGLPFADepthSize;
    attr[i++] = 32.0;
    attr[i] = 0;

    fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
    if (fmt == nil) {
		Debug::Log("Failed creating OpenGL pixel format");
        [pool release];
        return NULL;
    }

    context = [[NSOpenGLContext alloc] initWithFormat:fmt shareContext:nil];

    [fmt release];

    if (context == nil) {
		Debug::Log("Failed creating OpenGL context");
        [pool release];
        return NULL;
    }

    /*
     * Wisdom from Apple engineer in reference to UT2003's OpenGL performance:
     *  "You are blowing a couple of the internal OpenGL function caches. This
     *  appears to be happening in the VAO case.  You can tell OpenGL to up
     *  the cache size by issuing the following calls right after you create
     *  the OpenGL context.  The default cache size is 16."    --ryan.
     */

    #ifndef GLI_ARRAY_FUNC_CACHE_MAX
    #define GLI_ARRAY_FUNC_CACHE_MAX 284
    #endif

    #ifndef GLI_SUBMIT_FUNC_CACHE_MAX
    #define GLI_SUBMIT_FUNC_CACHE_MAX 280
    #endif

    {
        GLint cache_max = 64;
        CGLContextObj ctx = (CGLContextObj) [context CGLContextObj];
        CGLSetParameter (ctx, (CGLContextParameter) GLI_SUBMIT_FUNC_CACHE_MAX, &cache_max);
        CGLSetParameter (ctx, (CGLContextParameter) GLI_ARRAY_FUNC_CACHE_MAX, &cache_max);
    }

    /* End Wisdom from Apple Engineer section. --ryan. */

    [pool release];

    return context;
}

int
Cocoa_GL_MakeCurrent(NSWindow* window, NSOpenGLContext* context)
{
    NSAutoreleasePool *pool;

    pool = [[NSAutoreleasePool alloc] init];

	if (context) {
		[context setView:[window contentView]];
		[context update];
		[context makeCurrentContext];
	} else {
        [NSOpenGLContext clearCurrentContext];
    }

    [pool release];
    return 0;
}

void
Cocoa_GL_SwapWindow()
{
	NSAutoreleasePool *pool;
	NSOpenGLContext *context;

	pool = [[NSAutoreleasePool alloc] init];

	/* FIXME: Do we need to get the context for the window? */
	context = [NSOpenGLContext currentContext];
	if (context != nil) {
		[context flushBuffer];
	}

	[pool release];
}

void
Cocoa_GL_DeleteContext(NSOpenGLContext* context)
{
	NSAutoreleasePool *pool;

	pool = [[NSAutoreleasePool alloc] init];

	[context clearDrawable];
	[context release];

	[pool release];
}

int
Cocoa_GL_SetSwapInterval(int interval)
{
    NSAutoreleasePool *pool;
    NSOpenGLContext *nscontext;
    GLint value;
    int status;

    pool = [[NSAutoreleasePool alloc] init];

    nscontext = [NSOpenGLContext currentContext];
    if (nscontext != nil) {
        value = interval;
        [nscontext setValues:&value forParameter:NSOpenGLCPSwapInterval];
        status = 0;
    } else {
        Debug::Log("No current OpenGL context");
        status = -1;
    }

    [pool release];
    return status;
}

int
Cocoa_GL_GetSwapInterval()
{
    NSAutoreleasePool *pool;
    NSOpenGLContext *nscontext;
    GLint value;
    int status;

    pool = [[NSAutoreleasePool alloc] init];

    nscontext = [NSOpenGLContext currentContext];
    if (nscontext != nil) {
        [nscontext getValues:&value forParameter:NSOpenGLCPSwapInterval];
        status = (int)value;
    } else {
        Debug::Log("No current OpenGL context");
        status = -1;
    }

    [pool release];
    return status;
}

