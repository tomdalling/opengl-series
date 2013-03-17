//
//  WLViewController.m
//  01_project_skeleton
//
//  Created by Sid on 17/03/13.
//  Copyright (c) 2013 whackylabs. All rights reserved.
//

#import "WLViewController.h"
#import "iOS_main.h"

//GL_State masks flags
#define kGL_StateUnknown			(1<<0)	//Unknown
#define	kGL_StateInit			(1<<1)	//Init
#define	kGL_StateSetup			(1<<2)	//Setup
#define	kGL_StateRender			(1<<3)	//Ready to render
#define	kGL_StateUpdate			(1<<4)	//Ready to update
@interface WLViewController(){
	int glState;
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
	
	NSLog(@"State: Unknown");
	glState = kGL_StateUnknown;
    [self GL_WillSetup];
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

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
}

@end
