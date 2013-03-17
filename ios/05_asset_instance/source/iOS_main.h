//
//  iOS_main.h
//  01_project_skeleton
//
//  Created by Sid on 17/03/13.
//  Copyright (c) 2013 whackylabs. All rights reserved.
//

#ifndef _1_project_skeleton_iOS_main_h
#define _1_project_skeleton_iOS_main_h

// third-party libraries
#include <glm/glm.hpp>

typedef enum{
	eGesture_Move_Forward,	//W
	eGesture_Move_Back,		//S
	eGesture_Move_Left,		//A
	eGesture_Move_Right,	//D
	eGesture_Move_Up,		//Z
	eGesture_Move_Down,		//X
	eGesture_None
}eGesture;

extern glm::vec2 SCREEN_SIZE;
int iOS_main();
void Render();
void Update(float secondsElapsed);
void RegisterGesture(eGesture g);
#endif
