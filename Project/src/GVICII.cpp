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

#include "GMemMgr.h"
#include "GColour.h"
#include "type.h"

#include "plf_nanotimer.h"

#define FRAME_MS (1000/50)

GVICII::GVICII(GMemMgr* _mem) : updateThread(&GVICII::updateFrameBuffer, this)
{
    mem = _mem;
    frameBuffer = new word[getFrameBufferWidth()*getFrameBufferHeight()];

    registers.flags = 1 | (14 << 4);
	registers.mainColours = 14 | (6 << 4);
	registers.charSetPtr = 0xD000;
	registers.charDisplayPtr = 0xC000;

    // dumb fill with blue for now
    GColour testCol = GColour::getColourByIndex(6);
    word* currPx = frameBuffer;
    for(int y = 0; y < getFrameBufferHeight(); y++) {
		for(int x = 0; x < getFrameBufferWidth(); x++) {
			*(currPx++) = testCol.getRGB555();

		}
    }
}

GVICII::~GVICII()
{
    updateThread.join();
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
const word* GVICII::getFrameBuffer() {
	return frameBuffer;
}

//========================================================================
//
// Return the number of pixels high the framebuffer is
//
//========================================================================
const int GVICII::getFrameBufferHeight() {
	return 200;
}


//========================================================================
//
// Return the number of pixels wide the framebuffer is
//
//========================================================================
const int GVICII::getFrameBufferWidth() {
	return 320;
}



//========================================================================
//
// Return the colour to be used as the border
//
//========================================================================
const GColour GVICII::getBorderColour() {
	return GColour::getColourByIndex(GColour::LIGHT_BLUE);
}



//========================================================================
//
// Thread function to update frameBuffer from CPU RAM
//
//========================================================================
void GVICII::updateFrameBuffer()
{
	plf::nanotimer timer;
	timer.start();

	while(1) {
		if (timer.get_elapsed_ms() > FRAME_MS && frameBuffer != NULL) {
			timer.start();

			// render the actual characters (unoptimised)
			const byte* charDisplayMem = mem->getMem(registers.charDisplayPtr, 40*25);
			const byte* charSetMem = mem->getMem(registers.charSetPtr, 256*8);

			byte colours = registers.mainColours;
			GColour fgColour = GColour::getColourByIndex(colours & 15);
			GColour bgColour = GColour::getColourByIndex(colours >> 4);

			word* baseFramePtr = frameBuffer;

			const byte* charCode = charDisplayMem;

			for(int row = 0; row < 25; row++) {
				for(int col = 0; col < 40; col++) {
					const byte* charDefByte = charSetMem + (*charCode)*8;

					for(int charRow = 0; charRow < 8; charRow++) {
						word* currFramePtr = baseFramePtr + 40*8*charRow;

						for(byte mask = 128; mask > 0; mask >>= 1) {
							if((*charDefByte) & mask) {
								*(currFramePtr++) = fgColour.getRGB555();
							} else {
								*(currFramePtr++) = bgColour.getRGB555();
							}
						}

						charDefByte++;
					}

					baseFramePtr += 8;
					charCode++;
				}
				baseFramePtr += 7*320;
			}
		}
		std::this_thread::yield();
	}
}
