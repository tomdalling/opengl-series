//
//  WLViewController.m
//  01_project_skeleton
//
//  Created by Sid on 17/03/13.
//  Copyright (c) 2013 whackylabs. All rights reserved.
//

#import "WLViewController.h"

#import <CoreMotion/CoreMotion.h>

#import "iOS_main.h"

//GL_State masks flags
#define kGL_StateUnknown			(1<<0)	//Unknown
#define	kGL_StateInit			(1<<1)	//Init
#define	kGL_StateSetup			(1<<2)	//Setup
#define	kGL_StateRender			(1<<3)	//Ready to render
#define	kGL_StateUpdate			(1<<4)	//Ready to update
@interface WLViewController(){
	int glState;
	CMMotionManager *motionMgr;
	double lastPinchScale;	// To monitor change in scale
}
@property (strong, nonatomic) EAGLContext *context;
//	Research shows that screen's width and height are prepared after the update call.
//	And glkView:drawInRect: gets invoked first.
//	Let's wait for all the setup to get over with.
- (void)GL_WillSetup;
- (void)GL_DidSetup;
- (void)GL_Destroy;
@end

@implementation WLViewController

- (void)dealloc{
    [self GL_Destroy];
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    [_context release];
	[motionMgr release];
    [super dealloc];
}

- (void)viewDidLoad{
    [super viewDidLoad];
    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    if (!self.context) {
		[NSException raise:@"Failed to create ES context" format:@""];
    }
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	view.multipleTouchEnabled = YES;
	
	motionMgr = [[CMMotionManager alloc]init];
	
	NSLog(@"State: Unknown");
	glState = kGL_StateUnknown;
    [self GL_WillSetup];
	
	// Resgistering Gestures
	UILongPressGestureRecognizer *singleTapDownGesture = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(singleTapDownGestureAction:)];
	[self.view addGestureRecognizer:singleTapDownGesture];
	[singleTapDownGesture release];

	UITapGestureRecognizer *singleTapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(singleTapGestureAction:)];
	[self.view addGestureRecognizer:singleTapGesture];
	[singleTapGesture release];

	lastPinchScale = 1.0;
	UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchGestureAction:)];
	[self.view addGestureRecognizer:pinchGesture];
	[pinchGesture release];

	UIPanGestureRecognizer *dragGesture = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(dragGestureAction:)];
	[self.view addGestureRecognizer:dragGesture];
	[dragGesture release];

}

- (void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];
	
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        [self GL_Destroy];
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
}

//	Doesn't has right value for screen dimensions
- (void)GL_WillSetup{
    [EAGLContext setCurrentContext:self.context];
	glState |= kGL_StateInit;
	NSLog(@"State: Init");
}

//	Could be called multiple times. Do check.
//	We've the screen ready
-(void)GL_DidSetup{
	// already in setup state = do nothing
	if(glState & kGL_StateSetup){
		NSLog(@"State: Already setup");
		return;
	}
	// or is not in ready state = do nothing
	if(!(glState & kGL_StateInit)){
		NSLog(@"State: NOT Init");
		return;
	}
	if(!(glState & kGL_StateRender)){
		NSLog(@"State: NOT Render");
		return;
	}
	if(!(glState &  kGL_StateUpdate)){
		NSLog(@"State: NOT Update");
		return;
	}
		
	// constants
	SCREEN_SIZE = glm::vec2(self.view.bounds.size.width, self.view.bounds.size.height);
	iOS_main();
	glState |= kGL_StateSetup;
	NSLog(@"State: Setup");
}

- (void)GL_Destroy{
    [EAGLContext setCurrentContext:self.context];
	glState = kGL_StateUnknown;
	
	[motionMgr stopAccelerometerUpdates];
	NSLog(@"State: Unknown");
}

//Loop
- (void)update{
	//Skip if not in setup state.
	if(!(glState & kGL_StateSetup)){
		glState |= kGL_StateUpdate;
		NSLog(@"State: Update");
		[self GL_DidSetup];
		return;
	}
	Update(self.timeSinceLastUpdate);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
	//Skip if not in setup state.
	if(!(glState & kGL_StateSetup)){
		glState |= kGL_StateRender;
		NSLog(@"State: Render");
		[self GL_DidSetup];
		return;
	}
	
	Render();
}

//Dividing the Screen into a rect of 3x3, with key mapping as
//	-------------
//	| - | W | - |
//	-------------
//	| A | - | D |
//	-------------
//	| - | S | - |
//	-------------
-(eGestureAction)getGestureTapActionAtPoint:(CGPoint)tPt{
	// Register action
	double scr_w_one_third = self.view.bounds.size.width/3;
	double scr_h_one_third = self.view.bounds.size.height/3;
	int touch_index_x = tPt.x/scr_w_one_third;
	int touch_index_y = tPt.y/scr_h_one_third;
	
	switch(touch_index_x){
		case 0:
			if(touch_index_y == 1)		{ return eA;}
			break;
			
		case 1:
			if(touch_index_y == 0)		{ return eW;}
			else if(touch_index_y == 2) { return eS;}
			break;
			
		case 2:
			if(touch_index_y == 1)		{ return eD;}
			break;
	}
	return eNone;
}

-(void)singleTapDownGestureAction:(UILongPressGestureRecognizer *)gesture{
	if(gesture.state == UIGestureRecognizerStateBegan){
		gGesture.continious = true;
		gGesture.fingers = [gesture numberOfTouches];
		gGesture.taps = 1;
		gGesture.action = [self getGestureTapActionAtPoint:[gesture locationInView:self.view]];
	}else if(gesture.state == UIGestureRecognizerStateEnded){
		gGesture.continious = false;
		gGesture.fingers = 0;
		gGesture.taps = 0;
		gGesture.action = eNone;
	}
}

-(void)singleTapGestureAction:(UITapGestureRecognizer *)gesture{
	if(gesture.state == UIGestureRecognizerStateEnded){
		gGesture.continious = false;
		gGesture.fingers = [gesture numberOfTouches];
		gGesture.taps = 1;
		gGesture.action = [self getGestureTapActionAtPoint:[gesture locationInView:self.view]];
	}
}

-(void) pinchGestureAction:(UIPinchGestureRecognizer *)gesture{
	if(gesture.state == UIGestureRecognizerStateBegan){
		if(gesture.scale < lastPinchScale){	//zoom out
			gGesture.action = eZoomOut;
		}else{	// zoom in
			gGesture.action = eZoomIn;
		}
		gGesture.continious = true;
		gGesture.fingers = 2;
		gMoveSpeed = fabs(gesture.velocity) * DEFAULT_MOVE_SPEED;
		lastPinchScale = gesture.scale;
	}else if(gesture.state == UIGestureRecognizerStateEnded){
		gGesture.action = eNone;
		gGesture.continious = false;
		gGesture.fingers = 0;
		gMoveSpeed = DEFAULT_MOVE_SPEED;
		lastPinchScale = 1.0;
	}
}

-(void)dragGestureAction:(UIPanGestureRecognizer *)gesture{
	if(gesture.state == UIGestureRecognizerStateBegan || gesture.state == UIGestureRecognizerStateChanged){
		CGPoint pt = [gesture translationInView:self.view];
		gDragPoint.x = pt.x;
		gDragPoint.y = pt.y;
		gGesture.action = eDrag;
		gGesture.continious = true;
	}else{
		gDragPoint.x = gDragPoint.y = 0;
		gGesture.action = eNone;
		gGesture.continious = false;
		gGesture.fingers = 0;
		gMoveSpeed = DEFAULT_MOVE_SPEED;
	}
}
@end
