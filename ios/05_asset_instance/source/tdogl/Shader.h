/*
 tdogl::Shader
 
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

#include <OpenGLES/ES2/gl.h>
#include <string>

namespace tdogl {

    /**
     Represents a compiled OpenGL shader.
     */
    class Shader { 
    public:
        
        /**
         Creates a shader from a text file.
         
         @param filePath    The path to the text file containing the shader source.
         @param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
                            or GL_FRAGMENT_SHADER.
         
         @throws std::exception if an error occurs.
         */
        static Shader shaderFromFile(const std::string& filePath, GLenum shaderType);
        
        
        /**
         Creates a shader from a string of shader source code.
         
         @param shaderCode  The source code for the shader.
         @param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
                            or GL_FRAGMENT_SHADER.
         
         @throws std::exception if an error occurs.
         */
        Shader(const std::string& shaderCode, GLenum shaderType);
        
        
        /**
         @result The shader's object ID, as returned from glCreateShader
         */
        GLuint object() const;
        
        // tdogl::Shader objects can be copied and assigned because they are reference counted
        // like a shared pointer
        Shader(const Shader& other);
        Shader& operator =(const Shader& other);
        ~Shader();
        
    private:
        GLuint _object;
        unsigned* _refCount;
        
        void _retain();
        void _release();
    };
    
}
