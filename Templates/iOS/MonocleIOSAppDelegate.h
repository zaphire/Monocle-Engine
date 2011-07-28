//
//  MonocleIOSAppDelegate.h
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/27/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"

@interface MonocleIOSAppDelegate : NSObject <UIApplicationDelegate>
{
	// main UIWindow
	// The OpenGL view will be a attached to this UIWindow
    UIWindow *_window;
    
    EAGLView *_glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) EAGLView *glView;

@end


