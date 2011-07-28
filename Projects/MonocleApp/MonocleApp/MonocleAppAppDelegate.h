//
//  MonocleAppAppDelegate.h
//  MonocleApp
//
//  Created by Josh Whelchel on 7/27/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MonocleAppViewController;

@interface MonocleAppAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet MonocleAppViewController *viewController;

@end
