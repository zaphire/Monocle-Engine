/*
 
 ===== IMPORTANT =====
 
 This is sample code demonstrating API, technology or techniques in development.
 Although this sample code has been reviewed for technical accuracy, it is not
 final. Apple is supplying this information to help you plan for the adoption of
 the technologies and programming interfaces described herein. This information
 is subject to change, and software implemented based on this sample code should
 be tested with final operating system software and final documentation. Newer
 versions of this sample code may be provided with future seeds of the API or
 technology. For information about updates to this and other developer
 documentation, view the New & Updated sidebars in subsequent documentation
 seeds.
 
 =====================
 
 File: EAGLView.mm
 Abstract: Convenience class that wraps the CAEAGLLayer from CoreAnimation into a
 UIView subclass.
 
 Version: 1.3
 
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc.
 ("Apple") in consideration of your agreement to the following terms, and your
 use, installation, modification or redistribution of this Apple software
 constitutes acceptance of these terms.  If you do not agree with these terms,
 please do not use, install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject
 to these terms, Apple grants you a personal, non-exclusive license, under
 Apple's copyrights in this original Apple software (the "Apple Software"), to
 use, reproduce, modify and redistribute the Apple Software, with or without
 modifications, in source and/or binary forms; provided that if you redistribute
 the Apple Software in its entirety and without modifications, you must retain
 this notice and the following text and disclaimers in all such redistributions
 of the Apple Software.
 Neither the name, trademarks, service marks or logos of Apple Inc. may be used
 to endorse or promote products derived from the Apple Software without specific
 prior written permission from Apple.  Except as expressly stated in this notice,
 no other rights or licenses, express or implied, are granted by Apple herein,
 including but not limited to any patent rights that may be infringed by your
 derivative works or by other works in which the Apple Software may be
 incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR
 DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF
 CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
 APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 Copyright (C) 2008 Apple Inc. All Rights Reserved.
 
 */



#ifdef MONOCLE_IOS

// Only compile this code on iOS. These files should NOT be included on your Mac project.
// But in case they are included, it won't be compiled.
#import <Availability.h>
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED

#import <QuartzCore/QuartzCore.h>

#import "EAGLView.h"
#import "ES1Renderer.h"
#import "AudioToolbox/AudioToolbox.h"

#import <sys/time.h>
#include "Game.h"
#include "Platform.h"
#include "Audio.h"

using namespace Monocle;
Monocle::Scene *GetIOSFirstScene();

//CLASS IMPLEMENTATIONS:

@interface EAGLView (Private)
- (BOOL) setupSurfaceWithSharegroup:(EAGLSharegroup*)sharegroup;
- (unsigned int) convertPixelFormat:(NSString*) pixelFormat;
@end

@implementation EAGLView

@synthesize surfaceSize=size_;
@synthesize pixelFormat=pixelformat_, depthFormat=depthFormat_;
@synthesize touchDelegate=touchDelegate_;
@synthesize context=context_;
@synthesize multiSampling=multiSampling_;
@synthesize displayLink=displayLink_;
@synthesize game=game_;

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

+ (id) viewWithFrame:(CGRect)frame
{
	return [[[self alloc] initWithFrame:frame] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format
{
	return [[[self alloc] initWithFrame:frame pixelFormat:format] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth
{
	return [[[self alloc] initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0] autorelease];
}

+ (id) viewWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained sharegroup:(EAGLSharegroup*)sharegroup multiSampling:(BOOL)multisampling numberOfSamples:(unsigned int)samples
{
	return [[[self alloc] initWithFrame:frame pixelFormat:format depthFormat:depth preserveBackbuffer:retained sharegroup:sharegroup multiSampling:multisampling numberOfSamples:samples] autorelease];
}

- (id) initWithFrame:(CGRect)frame
{
	return [self initWithFrame:frame pixelFormat:kEAGLColorFormatRGB565 depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(NSString*)format 
{
	return [self initWithFrame:frame pixelFormat:format depthFormat:0 preserveBackbuffer:NO sharegroup:nil multiSampling:NO numberOfSamples:0];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(NSString*)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained sharegroup:(EAGLSharegroup*)sharegroup multiSampling:(BOOL)sampling numberOfSamples:(unsigned int)nSamples
{
	if((self = [super initWithFrame:frame]))
	{
		pixelformat_ = format;
		depthFormat_ = depth;
		multiSampling_ = sampling;
		requestedSamples_ = nSamples;
		preserveBackbuffer_ = retained;
		
		if( ! [self setupSurfaceWithSharegroup:sharegroup] ) {
			[self release];
			return nil;
		}
	}
    
    allowedOrientations_[0] = true;
    allowedOrientations_[1] = true;
    allowedOrientations_[2] = true;
    allowedOrientations_[3] = true;
    
	return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
	if( (self = [super initWithCoder:aDecoder]) ) {
		
		CAEAGLLayer*			eaglLayer = (CAEAGLLayer*)[self layer];
		
		pixelformat_ = kEAGLColorFormatRGB565;
		depthFormat_ = 0; // GL_DEPTH_COMPONENT24_OES;
		multiSampling_= YES;
		requestedSamples_ = 0;
		size_ = [eaglLayer bounds].size;
        
		if( ! [self setupSurfaceWithSharegroup:nil] ) {
			[self release];
			return nil;
		}
    }
	
    return self;
}

-(BOOL) setupSurfaceWithSharegroup:(EAGLSharegroup*)sharegroup
{
	CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
	
	eaglLayer.opaque = YES;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
									[NSNumber numberWithBool:preserveBackbuffer_], kEAGLDrawablePropertyRetainedBacking,
									pixelformat_, kEAGLDrawablePropertyColorFormat, nil];
	
	
	renderer_ = [[ES1Renderer alloc] initWithDepthFormat:depthFormat_
										 withPixelFormat:[self convertPixelFormat:pixelformat_]
										  withSharegroup:sharegroup
									   withMultiSampling:multiSampling_
									 withNumberOfSamples:requestedSamples_];
	if (!renderer_)
		return NO;
	
	context_ = [renderer_ context];
	[context_ renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
    
	discardFramebufferSupported_ = YES;
    
    // Check for device orientation
    // Tell the UIDevice to send notifications when the orientation changes
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:@"UIDeviceOrientationDidChangeNotification" object:nil];
	
	return YES;
}

- (void) allowOrientation:(UIDeviceOrientation)orientation valid:(BOOL)allow
{
    switch (orientation)
    {
        case UIDeviceOrientationPortrait:
            allowedOrientations_[0] = allow;
            break;
        case UIDeviceOrientationPortraitUpsideDown:
            allowedOrientations_[1] = allow;
            break;
        case UIDeviceOrientationLandscapeLeft:
            allowedOrientations_[2] = allow;
            break;
        case UIDeviceOrientationLandscapeRight:
            allowedOrientations_[3] = allow;
            break;
        default:
            return;
    }
}

- (void) setDeviceOrientation:(UIDeviceOrientation)orientation
{
    PlatformOrientation orient;
    
    switch (orientation)
    {
        case UIDeviceOrientationPortrait:
            if (!allowedOrientations_[0]) return;
            orient = PLATFORM_ORIENTATION_PORTRAIT;
            [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortrait animated:NO];
            break;
        case UIDeviceOrientationPortraitUpsideDown:
            if (!allowedOrientations_[1]) return;
            orient = PLATFORM_ORIENTATION_PORTRAIT_UPSIDEDOWN;
            [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationPortraitUpsideDown animated:NO];
            break;
        case UIDeviceOrientationLandscapeLeft:
            if (!allowedOrientations_[2]) return;
            orient = PLATFORM_ORIENTATION_LANDSCAPE_LEFT;
            [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeRight animated:NO];
            break;
        case UIDeviceOrientationLandscapeRight:
            if (!allowedOrientations_[3]) return;
            orient = PLATFORM_ORIENTATION_LANDSCAPE_RIGHT;
            [[UIApplication sharedApplication] setStatusBarOrientation: UIInterfaceOrientationLandscapeLeft animated:NO];
            break;
        default:
            return;
    }
    
    Platform::PlatformOrientationChanged( orient );
    return;
}

// tell the director that the orientation has changed
- (void) orientationChanged:(NSNotification *)notification
{
	UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
	[self setDeviceOrientation:orientation];
}

void interruptionListenerCallback (void   *inUserData, UInt32    interruptionState ) 
{
	if (interruptionState == kAudioSessionBeginInterruption) {
        Audio::SystemHalt();
        AudioSessionSetActive(NO);
	} else if (interruptionState == kAudioSessionEndInterruption) {
        Audio::SystemResume();
        AudioSessionSetActive(YES);
	}
}

- (void) prepare
{
    UInt32 category = kAudioSessionCategory_AmbientSound; //TODO: Change this value to whatever makes sense for this app
    OSStatus result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
    
    result = AudioSessionInitialize(NULL, NULL, interruptionListenerCallback, self);
}

- (void) dealloc
{
    [touchHoldTimer invalidate];
    touchHoldTimer = nil;
    [firstTouchTime release];
    [doubleTapTime release];
    [activeTouches release];
    [super dealloc];
    
    delete game_;
	[renderer_ release];
	[super dealloc];
}



- (void) layoutSubviews
{
	size_ = [renderer_ backingSize];
    
	[renderer_ resizeFromLayer:(CAEAGLLayer*)self.layer];

    /*
	// Issue #914 #924
	CCDirector *director = [CCDirector sharedDirector];
	[director reshapeProjection:size_];
	
	// Avoid flicker. Issue #350
	[director performSelectorOnMainThread:@selector(drawScene) withObject:nil waitUntilDone:YES];
     */
}	

- (void) swapBuffers
{
	// IMPORTANT:
	// - preconditions
	//	-> context_ MUST be the OpenGL context
	//	-> renderbuffer_ must be the the RENDER BUFFER
    
#ifdef __IPHONE_4_0
	
	if (multiSampling_)
	{
		/* Resolve from msaaFramebuffer to resolveFramebuffer */
		//glDisable(GL_SCISSOR_TEST);     
		glBindFramebufferOES(GL_READ_FRAMEBUFFER_APPLE, [renderer_ msaaFrameBuffer]);
		glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, [renderer_ defaultFrameBuffer]);
		glResolveMultisampleFramebufferAPPLE();
	}
	
	if( discardFramebufferSupported_)
	{	
		if (multiSampling_)
		{
			if (depthFormat_)
			{
				GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES, GL_DEPTH_ATTACHMENT_OES};
				glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, attachments);
			}
			else
			{
				GLenum attachments[] = {GL_COLOR_ATTACHMENT0_OES};
				glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 1, attachments);
			}
			
			glBindRenderbufferOES(GL_RENDERBUFFER_OES, [renderer_ colorRenderBuffer]);
            
		}	
		
		// not MSAA
		else if (depthFormat_ ) {
			GLenum attachments[] = { GL_DEPTH_ATTACHMENT_OES};
			glDiscardFramebufferEXT(GL_FRAMEBUFFER_OES, 1, attachments);
		}
	}
	
#endif // __IPHONE_4_0
	
	[context_ presentRenderbuffer:GL_RENDERBUFFER_OES];
	
	// We can safely re-bind the framebuffer here, since this will be the
	// 1st instruction of the new main loop
	if( multiSampling_ )
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, [renderer_ msaaFrameBuffer]);
}

- (unsigned int) convertPixelFormat:(NSString*) pixelFormat
{
	// define the pixel format
	GLenum pFormat;
	
	
	if([pixelFormat isEqualToString:@"EAGLColorFormat565"]) 
		pFormat = GL_RGB565_OES;
	else 
		pFormat = GL_RGBA8_OES;
	
	return pFormat;
}

#pragma mark EAGLView - Point conversion

- (CGPoint) convertPointFromViewToSurface:(CGPoint)point
{
	CGRect bounds = [self bounds];
	
	return CGPointMake((point.x - bounds.origin.x) / bounds.size.width * size_.width, (point.y - bounds.origin.y) / bounds.size.height * size_.height);
}

- (CGRect) convertRectFromViewToSurface:(CGRect)rect
{
	CGRect bounds = [self bounds];
	
	return CGRectMake((rect.origin.x - bounds.origin.x) / bounds.size.width * size_.width, (rect.origin.y - bounds.origin.y) / bounds.size.height * size_.height, rect.size.width / bounds.size.width * size_.width, rect.size.height / bounds.size.height * size_.height);
}

// Pass the touches to the superview
#pragma mark EAGLView - Touch Delegate

CGSize CGSizeDistanceBetween2Points(CGPoint point1, CGPoint point2)
{
    return CGSizeMake(point1.x -point2.x, point1.y - point2.y);
}

-(CGPoint)pointToGL:(CGPoint)uiPoint
{
	CGSize s = [self bounds].size;
	float newY = s.height - uiPoint.y;
	float newX = s.width - uiPoint.x;
    
    float res;
    CGPoint ret;
    
	switch ( Platform::GetOrientation() ) {
        default:
		case Monocle::PLATFORM_ORIENTATION_PORTRAIT:
            res = float(Graphics::GetVirtualWidth())/s.width;
			ret = CGPointMake( uiPoint.x, uiPoint.y );
			break;
		case Monocle::PLATFORM_ORIENTATION_PORTRAIT_UPSIDEDOWN:
            res = float(Graphics::GetVirtualWidth())/s.width;
			ret = CGPointMake(newX, newY);
			break;
		case Monocle::PLATFORM_ORIENTATION_LANDSCAPE_LEFT:
            res = float(Graphics::GetVirtualHeight())/s.width;
			ret.x = uiPoint.y;
			ret.y = newX;
			break;
		case Monocle::PLATFORM_ORIENTATION_LANDSCAPE_RIGHT:
            res = float(Graphics::GetVirtualHeight())/s.width;
			ret.x = newY;
			ret.y = uiPoint.x;
			break;
	}
    
    ret.x *= res;
    ret.y *= res;
	return ret;
}

-(void)UpdateTouches
{
    NSMutableArray *touches = activeTouches;
    int count = [touches count];
    
    if (count > TOUCHES_MAX) count = TOUCHES_MAX;
    
    Platform::numTouches = count;
    
    for (int i=0; i<count; i++)
    {
        TouchPhase newPhase;
        UITouch *touch = [touches objectAtIndex:i];
        Touch *oTouch = &Platform::touches[i];
        
        oTouch->fingerID = i;
        CGPoint p = [self pointToGL:[touch locationInView:self]];
        oTouch->position = Vector2(p.x,p.y);
        
//        printf("Touch: (%f,%f)\n",oTouch->position.x,oTouch->position.y);
        
        // (There is no vertical stretching)
        
        switch ([touch phase]){
            case UITouchPhaseBegan:
                newPhase = TOUCH_PHASE_BEGIN;
                break;
            case UITouchPhaseCancelled:
                newPhase = TOUCH_PHASE_CANCELLED;
                break;
            case UITouchPhaseEnded:
                newPhase = TOUCH_PHASE_ENDED;
                break;
            case UITouchPhaseMoved:
                newPhase = TOUCH_PHASE_MOVED;
                break;
            case UITouchPhaseStationary:
                newPhase = TOUCH_PHASE_STATIONARY;
                break;
            default:
                newPhase = TOUCH_PHASE_NONE;
        }
        
        if (newPhase != oTouch->phase){
            oTouch->phase = newPhase;
            oTouch->touched = 0;
        }
        
        oTouch->deltaTime = 0; // TODO: deltaTime in touch
        oTouch->tapCount = [touch tapCount];
        
        CGPoint prv = [self pointToGL:[touch previousLocationInView:self]];
        
        oTouch->deltaPosition = Vector2(prv.x,prv.y) - oTouch->position;
    }
}

- (void)touchIsBeingPinchedOrStretched:(NSSet *)touches;
{
    // calculate the distance between the two touches    
//    CGSize difference = CGSizeDistanceBetween2Points([[activeTouches objectAtIndex:0] locationInView:self], 
//                                                     [[activeTouches objectAtIndex:1] locationInView:self]);    
//    CGFloat x_scale_factor = originalDifference.width/difference.width;
//    CGFloat y_scale_factor = originalDifference.height/difference.height;
//    NSLog(@"Scale Factor: %x:f, y:%f", x_scale_factor, y_scale_factor);
//    
}

#define TOUCHUPDATETIME (1.0/15.0)

- (void)touchIsBeingHeld:(NSTimer *)timer;
{
    NSSet *touches = [timer userInfo];
    NSInteger count = [[touches anyObject] tapCount];
//    NSTimeInterval hold_time = -1*[firstTouchTime timeIntervalSinceNow];
    if (count == 1) {
        touchAndHoldCounter += 1;
        
    }
    else if (count == 2) {
        doubleTapAndHoldCounter += 1;
        //        NSLog(@"Held for %d counts and %g seconds.", DoubleTapAndHoldCounter, -1*[DoubleTapTime timeIntervalSinceNow]);
    }
}
- (void)cleanupTimers;
{
    [touchHoldTimer invalidate];
    touchHoldTimer = nil;
    [firstTouchTime release];
    firstTouchTime = nil;
    [doubleTapTime release];
    doubleTapTime = nil;
}

/*********************************************************************/
#pragma mark -
#pragma mark ** Touch Handlers **

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event 
{
    // NSSet *touches is the set of touches that began now.  We want to 
    // keep track of ALL touches, including touches that may have began 
    // sometime ago.  We have our own NSMutableArray* ActiveTouches to 
    // do so.
    if (activeTouches == nil)
        activeTouches = [[NSMutableArray alloc] init];
    
    for (UITouch *touch in touches) {
        if (![activeTouches containsObject:touch])
            [activeTouches addObject:touch];
    }
    
    if ([activeTouches count] == 1) { //single touch
        //Start touch timer
        touchAndHoldCounter = 0.0;
        firstTouchTime = [[NSDate alloc] init];
        touchHoldTimer = [NSTimer scheduledTimerWithTimeInterval:TOUCHUPDATETIME
                                                          target:self
                                                        selector:@selector(touchIsBeingHeld:) 
                                                        userInfo:touches
                                                         repeats:YES];
        if ([[touches anyObject] tapCount] == 2)
            doubleTapTime = [[NSDate alloc] init];
    } else if ([activeTouches count] == 2) { //two finger touch
        originalDifference = CGSizeDistanceBetween2Points([[activeTouches objectAtIndex:0] locationInView:self], 
                                                          [[activeTouches objectAtIndex:1] locationInView:self]);    
    }
    
    [self UpdateTouches];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event 
{
    if ([activeTouches count] == 1) { //single touch
        // do nothing
    } else if ([activeTouches count] == 2) { //two finger touch
        [self cleanupTimers];
        [self touchIsBeingPinchedOrStretched:touches];
    }
    
    [self UpdateTouches];
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [self UpdateTouches];
    
    for (UITouch *touch in touches) {
        [activeTouches removeObject:touch];
        
        if ([touch tapCount] == 3){
            // Right now, 3 taps triggers a restart.
//            game_->SetScene(GetIOSFirstScene());
        }
    }
    
    [self cleanupTimers];
    
    if ([touches count] == 1) { //single touch
        // do nothing
    } else if ([touches count] == 2) { //two finger touch
        
    }        
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event 
{
    [self cleanupTimers];
    [activeTouches removeAllObjects];
    
    [self UpdateTouches];
}

/*- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	if(touchDelegate_)
	{
		[touchDelegate_ touchesBegan:touches withEvent:event];
	}
    
    NSSet *allTouches = [event allTouches];
    int count = [allTouches count];
    
    if (count > TOUCHES_MAX) count = TOUCHES_MAX;
    
    Platform::numTouches = count;
    
    for (int i=0; i<count; i++)
    {
        UITouch *touch = [[allTouches allObjects] objectAtIndex:i];
        Touch *oTouch = &Platform::touches[i];
        
        oTouch->fingerID = i;
        CGPoint p = [touch locationInView:self];
        oTouch->position = Vector2(p.x,p.y);
        
        switch ([touch phase]){
            case UITouchPhaseBegan:
                oTouch->phase = TOUCH_PHASE_BEGIN;
                break;
            case UITouchPhaseCancelled:
                oTouch->phase = TOUCH_PHASE_CANCELLED;
                break;
            case UITouchPhaseEnded:
                oTouch->phase = TOUCH_PHASE_ENDED;
                break;
            case UITouchPhaseMoved:
                oTouch->phase = TOUCH_PHASE_MOVED;
                break;
            case UITouchPhaseStationary:
                oTouch->phase = TOUCH_PHASE_STATIONARY;
                break;
            default:
                oTouch->phase = TOUCH_PHASE_NONE;
        }
        
        oTouch->deltaTime = 0; // TODO: deltaTime in touch
        oTouch->tapCount = [touch tapCount];

        CGPoint prv = [touch previousLocationInView:self];
        
        oTouch->deltaPosition = Vector2(prv.x,prv.y) - oTouch->position;
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	if(touchDelegate_)
	{
		[touchDelegate_ touchesMoved:touches withEvent:event];
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	if(touchDelegate_)
	{
		[touchDelegate_ touchesEnded:touches withEvent:event];
	}
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
	if(touchDelegate_)
	{
		[touchDelegate_ touchesCancelled:touches withEvent:event];
	}
}*/

- (void) startAnimation
{
	NSAssert( displayLink_ == nil, @"displayLink must be nil. Calling startAnimation twice?");
    
    if ( gettimeofday( &lastUpdate_, NULL) != 0 ) {

	}
    
	displayLink_ = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(mainLoop:)];
	[displayLink_ setFrameInterval:1];
	[displayLink_ addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void) mainLoop:(id)sender
{
    game_->OneLoop();
    
    if (game_->IsDone()){
        game_->PlatformReset();
        game_->SetScene(GetIOSFirstScene());
    }
}

- (void) stopAnimation
{
	[displayLink_ invalidate];
	displayLink_ = nil;
}

@end

#endif // __IPHONE_OS_VERSION_MAX_ALLOWED

#endif // MONOCLE_IOS
