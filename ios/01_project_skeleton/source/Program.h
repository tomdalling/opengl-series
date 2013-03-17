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

#pragma once

#include "Shader.h"
#include <vector>

namespace tdogl {

    /**
     Represents an OpenGL program made by linking shaders.
     */
    class Program { 
    public:
        /**
         Creates a program by linking a list of tdogl::Shader objects
         
         @param shaders  The shaders to link together to make the program
         
         @throws std::exception if an error occurs.
         
         @see tdogl::Shader
         */
        Program(const std::vector<Shader>& shaders);
        ~Program();
        
        
        /**
         @result The program's object id, as returned from glCreateProgram
         */
        GLuint object() const;
        
        
        /**
         @result The attribute index for the given name, as returned from glGetAttribLocation.
         */
        GLint attrib(const GLchar* attribName) const;
        
        
        /**
         @result The uniform index for the given name, as returned from glGetUniformLocation.
         */
        GLint uniform(const GLchar* uniformName) const;

        
    private:
        GLuint _object;
        
        //copying disabled
        Program(const Program&);
        const Program& operator=(const Program&);
    };

}
