/*
 tdogl::Program
 
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

#include "Program.h"
#include <stdexcept>

using namespace tdogl;

Program::Program(const std::vector<Shader>& shaders) :
    _object(0)
{
    if(shaders.size() <= 0)
        throw std::runtime_error("No shaders were provided to create the program");
    
    //create the program object
    _object = glCreateProgram();
    if(_object == 0)
        throw std::runtime_error("glCreateProgram failed");
    
    //attach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glAttachShader(_object, shaders[i].object());
    
    //link the shaders together
    glLinkProgram(_object);
    
    //detach all the shaders
    for(unsigned i = 0; i < shaders.size(); ++i)
        glDetachShader(_object, shaders[i].object());
    
    //throw exception if linking failed
    GLint status;
    glGetProgramiv(_object, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");
        
        GLint infoLogLength;
        glGetProgramiv(_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteProgram(_object); _object = 0;
        throw std::runtime_error(msg);
    }
}

Program::~Program() {
    //might be 0 if ctor fails by throwing exception
    if(_object != 0) glDeleteProgram(_object);
}

GLuint Program::object() const {
    return _object;
}

GLint Program::attrib(const GLchar* attribName) const {
    if(!attribName)
        throw std::runtime_error("attribName was NULL");
    
    GLint attrib = glGetAttribLocation(_object, attribName);
    if(attrib == -1)
        throw std::runtime_error(std::string("Program attribute not found: ") + attribName);
    
    return attrib;
}

GLint Program::uniform(const GLchar* uniformName) const {
    if(!uniformName)
        throw std::runtime_error("uniformName was NULL");
    
    GLint uniform = glGetUniformLocation(_object, uniformName);
    if(uniform == -1)
        throw std::runtime_error(std::string("Program uniform not found: ") + uniformName);
    
    return uniform;
}
