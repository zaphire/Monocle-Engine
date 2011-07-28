//
//  lolAppDelegate.h
//  lol
//
//  Created by Josh Whelchel on 7/27/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#import <UIKit/UIKit.h>

@class lolViewController;

@interface lolAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet lolViewController *viewController;

@end
