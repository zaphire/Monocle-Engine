#include "CocoaPlatform.h"
#include "CocoaWindowListener.h"

#include "../Vector2.h"
#include "../Game.h"

using Monocle::CocoaPlatform;
using Monocle::Vector2;

static __inline__ void ConvertNSRect(NSRect *r)
{
	r->origin.y = CGDisplayPixelsHigh(kCGDirectMainDisplay) - r->origin.y - r->size.height;
}

@implementation CocoaWindowListener

- (void)listen:(WindowData *)data
{
	NSNotificationCenter *center;
	NSWindow *window = data->nswindow;
	NSView *view = [window contentView];

	_data = data;

	center = [NSNotificationCenter defaultCenter];

	if ([window delegate] != nil) {
		[center addObserver:self selector:@selector(windowDidExpose:) name:NSWindowDidExposeNotification object:window];
		[center addObserver:self selector:@selector(windowDidMove:) name:NSWindowDidMoveNotification object:window];
		[center addObserver:self selector:@selector(windowDidResize:) name:NSWindowDidResizeNotification object:window];
		[center addObserver:self selector:@selector(windowDidMiniaturize:) name:NSWindowDidMiniaturizeNotification object:window];
		[center addObserver:self selector:@selector(windowDidDeminiaturize:) name:NSWindowDidDeminiaturizeNotification object:window];
		[center addObserver:self selector:@selector(windowDidBecomeKey:) name:NSWindowDidBecomeKeyNotification object:window];
		[center addObserver:self selector:@selector(windowDidResignKey:) name:NSWindowDidResignKeyNotification object:window];
	} else {
		[window setDelegate:self];
	}

	[window setNextResponder:self];
	[window setAcceptsMouseMovedEvents:YES];

	[view setNextResponder:self];
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
	[view setAcceptsTouchEvents:YES];
#endif
}

- (void)close
{
	NSNotificationCenter *center;
	NSWindow *window = _data->nswindow;
	NSView *view = [window contentView];

	center = [NSNotificationCenter defaultCenter];

	if ([window delegate] != self) {
		[center removeObserver:self name:NSWindowDidExposeNotification object:window];
		[center removeObserver:self name:NSWindowDidMoveNotification object:window];
		[center removeObserver:self name:NSWindowDidResizeNotification object:window];
		[center removeObserver:self name:NSWindowDidMiniaturizeNotification object:window];
		[center removeObserver:self name:NSWindowDidDeminiaturizeNotification object:window];
		[center removeObserver:self name:NSWindowDidBecomeKeyNotification object:window];
		[center removeObserver:self name:NSWindowDidResignKeyNotification object:window];
	} else {
		[window setDelegate:nil];
	}

	if ([window nextResponder] == self) {
		[window setNextResponder:nil];
	}
	if ([view nextResponder] == self) {
		[view setNextResponder:nil];
	}
}

- (BOOL)windowShouldClose:(id)sender
{
    Monocle::Game::Quit();
	Cocoa_DestroyWindow(_data);
	return NO;
}

- (void)windowDidResize:(NSNotification *)aNotification
{
	int w, h;
	NSRect rect = [_data->nswindow contentRectForFrameRect:[_data->nswindow frame]];
	w = (int)rect.size.width;
	h = (int)rect.size.height;
	CocoaPlatform::platform->WindowSizeChanged(w, h);
}

- (void)mouseDown:(NSEvent *)theEvent
{
	NSInteger buttonNumber = [theEvent buttonNumber];
    if([theEvent modifierFlags] & NSControlKeyMask)
    {
        buttonNumber = 1;
    }
	CocoaPlatform::platform->SetMouseButton((int)buttonNumber, true);
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	[self mouseDown:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	[self mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	NSInteger buttonNumber = [theEvent buttonNumber];
    if([theEvent modifierFlags] & NSControlKeyMask)
    {
        buttonNumber = 1;
    }
	CocoaPlatform::platform->SetMouseButton((int)buttonNumber, false);
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	[self mouseUp:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	[self mouseUp:theEvent];
}

- (void)mouseEntered:(NSEvent *)theEvent
{
	// XXX focus window, hide mouse cursor
}

- (void)mouseExited:(NSEvent *)theEvent
{
	// XXX release focus, show mouse cursor
}

- (void)mouseMoved:(NSEvent *)theEvent
{
	NSPoint point;
	int x, y;

    //  Window width and height
    int ww, wh;
    ww = CocoaPlatform::platform->GetWidth();
    wh = CocoaPlatform::platform->GetHeight();
    
	point = [theEvent locationInWindow];
	x = (int)point.x;
	y = (int)(wh - point.y);

	if (x < 0 || x >= ww || y < 0 || y >= wh) {
		// if (SDL_GetMouseFocus() == window) {
		//     [self mouseExited:theEvent];
		// }
	} else {
		// if (SDL_GetMouseFocus() != window) {
		//     [self mouseEntered:theEvent];
		// }
		Vector2& mousePos = CocoaPlatform::platform->mousePosition;
		mousePos.x = x;
		mousePos.y = y;
	}
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	[self mouseMoved:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	[self mouseMoved:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	[self mouseMoved:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
}

-(void) handleTouches:(cocoaTouchType)type withEvent:(NSEvent*) event {
    
}

@end

void
Cocoa_DestroyWindow(WindowData* data)
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	if (data) {
		[data->listener close];
		[data->listener release];
		if (data->created) {
			[data->nswindow close];
		}
		free(data);
	}
	[pool release];
}


