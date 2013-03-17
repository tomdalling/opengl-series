/*
 main
 
 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include "iOS_main.h"

// third-party libraries
#import <Foundation/Foundation.h>
#include <glm/gtc/matrix_transform.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

// tdogl classes
#include "tdogl/Program.h"
#include "tdogl/Texture.h"
#include "tdogl/Camera.h"

// constants
glm::vec2 SCREEN_SIZE(0, 0);

// globals
tdogl::Texture* gTexture = NULL;
tdogl::Program* gProgram = NULL;
tdogl::Camera gCamera;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;
eGesture gesture = eGesture_None;

// returns the full path to the file `fileName` in the resources directory of the app bundle
static std::string ResourcePath(std::string fileName) {
    NSString* fname = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString* path = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:fname];
    return std::string([path cStringUsingEncoding:NSUTF8StringEncoding]);
}


// loads the vertex shader and fragment shader, and links them to make the global gProgram
static void LoadShaders() {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("vertex-shader.txt"), GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(ResourcePath("fragment-shader.txt"), GL_FRAGMENT_SHADER));
    gProgram = new tdogl::Program(shaders);
}


// loads a cube into the VAO and VBO globals: gVAO and gVBO
static void LoadCube() {
    // make and bind the VAO
    glGenVertexArraysOES(1, &gVAO);
    glBindVertexArrayOES(gVAO);
    
    // make and bind the VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    
    // Make a cube out of triangles (two triangles per side)
    GLfloat vertexData[] = {
        //  X     Y     Z       U     V
        // bottom
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

        // top
        -1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // front
        -1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

        // left
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
        -1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
         1.0f, 1.0f, 1.0f,   0.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("vert"));
    glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);
        
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
    glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

    // unbind the VAO
    glBindVertexArrayOES(0);
}


// loads the file "wooden-crate.jpg" into gTexture
static void LoadTexture() {
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(ResourcePath("wooden-crate.jpg"));
    bmp.flipVertically();
    gTexture = new tdogl::Texture(bmp);
}


// draws a single frame
void Render() {
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // bind the program (the shaders)
    gProgram->use();

    // set the "camera" uniform
    gProgram->setUniform("camera", gCamera.matrix());

    // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
    gProgram->setUniform("model", glm::rotate(glm::mat4(), gDegreesRotated, glm::vec3(0,1,0)));
        
    // bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture->object());
    gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

    // bind the VAO (the triangle)
    glBindVertexArrayOES(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    
    // unbind the VAO, the program and the texture
    glBindVertexArrayOES(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    gProgram->stopUsing();
    
}


// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
    //rotate the cube
    const GLfloat degreesPerSecond = 180.0f;
    gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;

    //move position of camera based on WASD keys, and XZ keys for up and down
    const float moveSpeed = 2.0; //units per second
    if(gesture == eGesture_Move_Back){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
    } else if(gesture == eGesture_Move_Forward){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
    }
    if(gesture == eGesture_Move_Left){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
    } else if(gesture == eGesture_Move_Right){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
    }
    if(gesture == eGesture_Move_Up){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -glm::vec3(0,1,0));
    } else if(gesture == eGesture_Move_Down){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * glm::vec3(0,1,0));
    }

    //rotate camera based on mouse movement
//    const float mouseSensitivity = 0.1;
//    int mouseX, mouseY;
//    glfwGetMousePos(&mouseX, &mouseY);
//    gCamera.offsetOrientation(mouseSensitivity * mouseY, mouseSensitivity * mouseX);
//    glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window
//
//    //increase or decrease field of view based on mouse wheel
//    const float zoomSensitivity = -0.2;
//    float fieldOfView = gCamera.fieldOfView() + zoomSensitivity * (float)glfwGetMouseWheel();
//    if(fieldOfView < 5.0f) fieldOfView = 5.0f;
//    if(fieldOfView > 130.0f) fieldOfView = 130.0f;
//    gCamera.setFieldOfView(fieldOfView);
//    glfwSetMouseWheel(0);
}

void RegisterGesture(eGesture g){
	gesture = g;
}

// the program starts here
void AppMain() {
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;


    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // load vertex and fragment shaders into opengl
    LoadShaders();

    // load the texture
    LoadTexture();

    // create buffer and fill it with the points of the triangle
    LoadCube();

    // setup gCamera
    gCamera.setPosition(glm::vec3(0,0,10));
    gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
}


int iOS_main() {
    try {
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
