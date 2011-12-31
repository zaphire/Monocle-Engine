//
//  main.m
//  MonocleIOS
//
//  Created by Josh Whelchel on 7/27/11.
//  Copyright 2011 jwmusic.org. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MonocleIOSAppDelegate.h"

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, @"MonocleIOSAppDelegate");
    [pool release];
    return retVal;
}
