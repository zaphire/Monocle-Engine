//
//  MonocleIOSAppDelegate.m
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/27/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#import "MonocleIOSAppDelegate.h"
#import "IOSPlatform.h"
#import "Graphics.h"
#import "Game.h"

using namespace Monocle;

/** YOUR GAME-SPECIFIC HEADERS (for your initial scene, at least) **/
#import "TestSelector.h"

// Allows building DisplayLinkDirector for pre-3.1 SDKS
// without getting compiler warnings.
@interface NSObject(CADisplayLink)
+ (id) displayLinkWithTarget:(id)arg1 selector:(SEL)arg2;
- (void) addToRunLoop:(id)arg1 forMode:(id)arg2;
- (void) setFrameInterval:(int)interval;
- (void) invalidate;
@end

Monocle::Scene *GetIOSFirstScene()
{
    return new TestSelector::GameScene();
}

@implementation MonocleIOSAppDelegate

@synthesize window = _window;
@synthesize glView = _glView;

/** ADD YOUR GAME STUFF HERE: **/

- (void) applicationDidFinishLaunching:(UIApplication*)application
{
    // Override point for customization after application launch.
    _window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    // create an OpenGL view
	_glView = [EAGLView viewWithFrame:[_window bounds]];
    
    [_glView retain];
	
	// enable multiple touches
	[_glView setMultipleTouchEnabled:YES];
	
	// glview is a child of the main window
	[_window addSubview:_glView];
	
	// Make the window visible
	[_window makeKeyAndVisible];
    
    [_glView prepare];
    
    IOSPlatform::view = _glView;
    IOSPlatform::window = _window;
    
    /** HERE IS WHERE SET UP THE INITIAL SCENE **/
    Game *game = new Game();
    game->SetScene(GetIOSFirstScene());

    [_glView setGame:game];
    
    [_glView startAnimation];
    
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
    [_glView stopAnimation];
}

-(void) applicationWillEnterForeground:(UIApplication*)application
{
    [_glView startAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
//    [glView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
    [_glView stopAnimation];
    
    [_glView release];
    _glView = nil;
}

- (void)dealloc
{
    [_window release];
    [super dealloc];
}

@end
