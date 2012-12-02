/*
 tdogl::Texture
 
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

#include <GL/glew.h>
#include "Bitmap.h"

namespace tdogl {
    
    /**
     Represents an OpenGL texture
     */
    class Texture {
    public:
        /**
         Creates a texture from a bitmap.
         
         The texture will be loaded upside down because tdogl::Bitmap pixel data
         is ordered from the top row down, but OpenGL expects the data to
         be from the bottom row up.
         
         @param bitmap  The bitmap to load the texture from
         @param minMagFiler  GL_NEAREST or GL_LINEAR
         @param wrapMode GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, or GL_CLAMP_TO_BORDER
         */
        Texture(const Bitmap& bitmap,
                GLint minMagFiler = GL_LINEAR,
                GLint wrapMode = GL_CLAMP_TO_EDGE);
        
        /**
         Deletes the texture object with glDeleteTextures
         */
        ~Texture();
        
        /**
         @result The texure object, as created by glGenTextures
         */
        GLuint object() const;
        
        /**
         @result The original width (in pixels) of the bitmap this texture was made from
         */
        GLfloat originalWidth() const;

        /**
         @result The original height (in pixels) of the bitmap this texture was made from
         */
        GLfloat originalHeight() const;
        
    private:
        GLuint _object;
        GLfloat _originalWidth;
        GLfloat _originalHeight;
        
        //copying disabled
        Texture(const Texture&);
        const Texture& operator=(const Texture&);
    };
    
}
