//
//  MonocleTestAppDelegate.h
//  MonocleTest
//
//  Created by Dan V. on 11-03-15.
//  Copyright 2011 Expanding Brain. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface MonocleTestAppDelegate : NSObject <NSApplicationDelegate> {
@private
    NSWindow *window;
}

@property (assign) IBOutlet NSWindow *window;

@end
