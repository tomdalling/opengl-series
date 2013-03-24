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

const float DEFAULT_MOVE_SPEED = 2.0; //units per second

typedef enum{
	eW,		//W
	eS,		//S
	eA,		//A
	eD,		//D
	eZoomIn,
	eZoomOut,
	eDrag,
	eNone
}eGestureAction;

struct Gesture_{
	int fingers;
	int taps;
	eGestureAction action;
	bool continious;
};
typedef struct Gesture_ Gesture;

extern Gesture gGesture;
extern glm::vec2 SCREEN_SIZE;
extern float gMoveSpeed;
extern glm::vec2 gDragPoint;

int iOS_main();
void Render();
void Update(float secondsElapsed);
#endif
