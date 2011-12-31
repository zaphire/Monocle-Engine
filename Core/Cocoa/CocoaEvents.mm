#include "CocoaEvents.h"
#include "CocoaPlatform.h"
#include "CocoaWindowListener.h"
#include "CocoaKeys.h"

#if !defined(UsrActivity) && defined(__LP64__) && !defined(__POWER__)
/*
 * Workaround for a bug in the 10.5 SDK: By accident, OSService.h does
 * not include Power.h at all when compiling in 64bit mode. This has
 * been fixed in 10.6, but for 10.5, we manually define UsrActivity
 * to ensure compilation works.
 */
#define UsrActivity 1
#endif

using Monocle::CocoaPlatform;

/* setAppleMenu disappeared from the headers in 10.4 */
@interface NSApplication(NSAppleMenu)
- (void)setAppleMenu:(NSMenu *)menu;
@end

@interface MonocleAppDelegate : NSObject
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
@end

@implementation MonocleAppDelegate : NSObject
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
	// SDL_SendQuit();
    WindowData* data = CocoaPlatform::instance->windowData;
    if (data->created) {
        Cocoa_DestroyWindow(data);
    }
	// return NSTerminateCancel;
    return NSTerminateNow;
}
@end

static NSString *
GetApplicationName(void)
{
	NSDictionary *dict;
	NSString *appName = 0;

	/* Determine the application name */
	dict = (NSDictionary *)CFBundleGetInfoDictionary(CFBundleGetMainBundle());
	if (dict)
		appName = [dict objectForKey: @"CFBundleName"];

	if (![appName length])
		appName = [[NSProcessInfo processInfo] processName];

	return appName;
}

static void
CreateApplicationMenus(void)
{
	NSString *appName;
	NSString *title;
	NSMenu *appleMenu;
	NSMenu *windowMenu;
	NSMenuItem *menuItem;

	/* Create the main menu bar */
	[NSApp setMainMenu:[[NSMenu alloc] init]];

	/* Create the application menu */
	appName = GetApplicationName();
	appleMenu = [[NSMenu alloc] initWithTitle:@""];

	/* Add menu items */
	title = [@"About " stringByAppendingString:appName];
	[appleMenu addItemWithTitle:title action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];

//	[appleMenu addItem:[NSMenuItem separatorItem]];

//	[appleMenu addItemWithTitle:@"Preferences" action:nil keyEquivalent:@","];

//	[appleMenu addItem:[NSMenuItem separatorItem]];

	title = [@"Hide " stringByAppendingString:appName];
	[appleMenu addItemWithTitle:title action:@selector(hide:) keyEquivalent:@"h"];

	menuItem = (NSMenuItem *)[appleMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"];
	[menuItem setKeyEquivalentModifierMask:(NSAlternateKeyMask|NSCommandKeyMask)];

	[appleMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];

	[appleMenu addItem:[NSMenuItem separatorItem]];

	title = [@"Quit " stringByAppendingString:appName];
	[appleMenu addItemWithTitle:title action:@selector(terminate:) keyEquivalent:@"q"];

	/* Put menu into the menubar */
	menuItem = [[NSMenuItem alloc] initWithTitle:@"" action:nil keyEquivalent:@""];
	[menuItem setSubmenu:appleMenu];
	[[NSApp mainMenu] addItem:menuItem];
	[menuItem release];

	/* Tell the application object that this is now the application menu */
	[NSApp setAppleMenu:appleMenu];
	[appleMenu release];


//	/* Create the window menu */
//	windowMenu = [[NSMenu alloc] initWithTitle:@"Window"];
//
//	/* "Minimize" item */
//	menuItem = [[NSMenuItem alloc] initWithTitle:@"Minimize" action:@selector(performMiniaturize:) keyEquivalent:@/*"m"*/""];
//	[windowMenu addItem:menuItem];
//	[menuItem release];
//
//	/* Put menu into the menubar */
//	menuItem = [[NSMenuItem alloc] initWithTitle:@"Window" action:nil keyEquivalent:@""];
//	[menuItem setSubmenu:windowMenu];
//	[[NSApp mainMenu] addItem:menuItem];
//	[menuItem release];
//
//	/* Tell the application object that this is now the window menu */
//	[NSApp setWindowsMenu:windowMenu];
//	[windowMenu release];
}

void
Cocoa_RegisterApp(void)
{
	ProcessSerialNumber psn;
	NSAutoreleasePool *pool;

	if (!GetCurrentProcess(&psn)) {
		TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		SetFrontProcess(&psn);
	}

	pool = [[NSAutoreleasePool alloc] init];
	if (NSApp == nil) {
		[NSApplication sharedApplication];

		if ([NSApp mainMenu] == nil) {
			CreateApplicationMenus();
		}
		[NSApp finishLaunching];
	}
	if ([NSApp delegate] == nil) {
		[NSApp setDelegate:[[MonocleAppDelegate alloc] init]];
	}
	[pool release];
}

void
Cocoa_PumpEvents()
{
	NSAutoreleasePool *pool;

	/* TODO Update activity every 30 seconds to prevent screensaver */

	pool = [[NSAutoreleasePool alloc] init];
	for ( ; ; ) {
		NSEvent *event = [NSApp nextEventMatchingMask:NSAnyEventMask untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES ];
		if ( event == nil ) {
			break;
		}

		switch ([event type]) {
			case NSKeyDown:
			case NSKeyUp:
//			case NSFlagsChanged:
				// XXX handle key event
				Cocoa_HandleKeyEvent(event);
                
				/* Fall through to pass event to NSApp; er, nevermind... */

				/* Add to support system-wide keyboard shortcuts like CMD+Space */
				if (([event modifierFlags] & NSCommandKeyMask) || [event type] == NSFlagsChanged)
					[NSApp sendEvent: event];
				break;
            case NSFlagsChanged:
                if ([event modifierFlags] & NSControlKeyMask)
                {
                    printf("control key changed\n");
                }
                break;
			default:
				[NSApp sendEvent:event];
				break;
		}
	}
	[pool release];
}

