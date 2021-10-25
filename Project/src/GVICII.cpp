//=======================================================================================
//
// GVICII.cpp
// Copyright Greg King 2021
//
// Distributed under the MIT licence
//
//    MIT License
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is
//    furnished to do so, subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//
//=======================================================================================


#include "GVICII.h"

#include "GColour.h"
#include "type.h"

GVICII::GVICII(G6510* _cpu)
{
    cpu = _cpu;
    frameBuffer = new byte[getFrameBufferWidth()*getFrameBufferHeight()*3];

    // dumb fill with blue for now
    GColour testCol = GColour::getColourByIndex(6);
    byte* currByte = frameBuffer;
    for(int y = 0; y < getFrameBufferHeight(); y++) {
		for(int x = 0; x < getFrameBufferWidth(); x++) {
			*(currByte++) = testCol.getRedByte();
			*(currByte++) = testCol.getGreenByte();
			*(currByte++) = testCol.getBlueByte();
		}
    }
}

GVICII::~GVICII()
{
    if(frameBuffer != NULL) {
		delete [] frameBuffer;
		frameBuffer = NULL;
    }
}

//========================================================================
//
// Return the byte buffer of RGB24s
//
//========================================================================
const byte* GVICII::getFrameBuffer() {
	return frameBuffer;
}

//========================================================================
//
// Return the number of pixels high the framebuffer is
//
//========================================================================
const int GVICII::getFrameBufferHeight() {
	return 320;
}


//========================================================================
//
// Return the number of pixels wide the framebuffer is
//
//========================================================================
const int GVICII::getFrameBufferWidth() {
	return 200;
}



//========================================================================
//
// Return the colour to be used as the border
//
//========================================================================
const GColour GVICII::getBorderColour() {
	return GColour::getColourByIndex(GColour::LIGHT_BLUE);
}

