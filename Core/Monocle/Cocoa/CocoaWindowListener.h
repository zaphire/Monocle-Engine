#pragma once

#import <Cocoa/Cocoa.h>

typedef struct WindowData WindowData;

#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_6
@interface CocoaWindowListener : NSResponder <NSWindowDelegate> {
#else
@interface CocoaWindowListener : NSResponder {
#endif
	WindowData* _data;
}

-(void) listen:(WindowData *) data;
-(void) close;

/* Window delegate functionality */
-(BOOL) windowShouldClose:(id) sender;
-(void) windowDidResize:(NSNotification *) aNotification;

/* Window event handling */
-(void) mouseDown:(NSEvent *) theEvent;
-(void) rightMouseDown:(NSEvent *) theEvent;
-(void) otherMouseDown:(NSEvent *) theEvent;
-(void) mouseUp:(NSEvent *) theEvent;
-(void) rightMouseUp:(NSEvent *) theEvent;
-(void) otherMouseUp:(NSEvent *) theEvent;
-(void) mouseEntered:(NSEvent *)theEvent;
-(void) mouseExited:(NSEvent *)theEvent;
-(void) mouseMoved:(NSEvent *) theEvent;
-(void) mouseDragged:(NSEvent *) theEvent;
-(void) rightMouseDragged:(NSEvent *) theEvent;
-(void) otherMouseDragged:(NSEvent *) theEvent;
-(void) scrollWheel:(NSEvent *) theEvent;

/* Touch event handling */
typedef enum {
	COCOA_TOUCH_DOWN,
	COCOA_TOUCH_UP,
	COCOA_TOUCH_MOVE,
	COCOA_TOUCH_CANCELLED
} cocoaTouchType;
-(void) handleTouches:(cocoaTouchType)type withEvent:(NSEvent*) event;

@end

struct WindowData
{
	bool created;
	NSWindow *nswindow;
	CocoaWindowListener *listener;
};

void Cocoa_DestroyWindow(WindowData* data);

