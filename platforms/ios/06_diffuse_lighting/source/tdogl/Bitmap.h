/*
 tdogl::Bitmap
 
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

#include <string>

namespace tdogl {
    
    /**
     A bitmap image (i.e. a grid of pixels).
     
     This is not really related to OpenGL, but can be used to make OpenGL textures using
     tdogl::Texture.
     */
    class Bitmap {
    public:
        /**
         Represents the number of channels per pixel, and the order of the channels.
         
         Each channel is one byte (unsigned char).
         */
        enum Format {
            Format_Grayscale = 1, /**< one channel: grayscale */
            Format_GrayscaleAlpha = 2, /**< two channels: grayscale and alpha */
            Format_RGB = 3, /**< three channels: red, green, blue */
            Format_RGBA = 4 /**< four channels: red, green, blue, alpha */
        };
        
        /**
         Creates a new image with the specified width, height and format.
         
         Width and height are in pixels. Image will contain random garbage if
         pixels = NULL.
         */
        Bitmap(unsigned width, 
               unsigned height, 
               Format format,
               const unsigned char* pixels = NULL);
        ~Bitmap();
        
        /**
         Tries to load the given file into a tdogl::Bitmap.
         */
        static Bitmap bitmapFromFile(std::string filePath);
                
        /** width in pixels */
        unsigned width() const;
        
        /** height in pixels */
        unsigned height() const;
        
        /** the pixel format of the bitmap */
        Format format() const;
        
        /**
         Pointer to the raw pixel data of the bitmap.
         
         Each channel is 1 byte. The number and meaning of channels per pixel is specified
         by the `Format` of the image. The pointer points to all the columns of
         the top row of the image, followed by each remaining row down to the bottom.
         i.e. c0r0, c1r0, c2r0, ..., c0r1, c1r1, c2r1, etc
         */
        unsigned char* pixelBuffer() const;
        
        /**
         Returns a pointer to the start of the pixel at the given coordinates. 
         
         The size of the pixel depends on the `Format` of the image.
         */
        unsigned char* getPixel(unsigned int column, unsigned int row) const;
        
        /**
         Sets the raw pixel data at the given coordinates.
         
         The size of the pixel depends on the `Format` of the bitmap.
         */
        void setPixel(unsigned int column, unsigned int row, const unsigned char* pixel);
        
        /**
         Reverses the row order of the pixels, so the bitmap will be upside down.
         */
        void flipVertically();
        
        /**
         Rotates the image 90 degrees counter clockwise.
         */
        void rotate90CounterClockwise();
        
        /**
         Copies a rectangular area from the given source bitmap into this bitmap.
         
         If srcCol, srcRow, width, and height are all zero, the entire source
         bitmap will be copied (full width and height).
         
         If the source bitmap has a different format to the destination bitmap, 
         the pixels will be converted to match the destination format.
         
         Will throw and exception if the source and destination bitmaps are the 
         same, and the source and destination rectangles overlap. If you want to
         copy a bitmap onto itself, then make a copy of the bitmap first.
         */
        void copyRectFromBitmap(const Bitmap& src, 
                                unsigned srcCol, 
                                unsigned srcRow, 
                                unsigned destCol, 
                                unsigned destRow,
                                unsigned width,
                                unsigned height);
        
        /** Copy constructor */
        Bitmap(const Bitmap& other);
        
        /** Assignment operator */
        Bitmap& operator = (const Bitmap& other);
        
    private:
        Format _format;
        unsigned _width;
        unsigned _height;
        unsigned char* _pixels;
        
        void _set(unsigned width, unsigned height, Format format, const unsigned char* pixels);
        static void _getPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Format format);
    };
    
}
