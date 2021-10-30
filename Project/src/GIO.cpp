//=======================================================================================
//
//
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


#include "GIO.h"

#include "GVICII.h"

GIO::GIO()
{
	gpu = NULL;
}

GIO::~GIO()
{
	//dtor
}


//========================================================================
//
// get a byte from the attached chips
//
//========================================================================
const byte GIO::getByte(int offset)
{
	switch(offset) {
		case 0x00:	if(gpu) return gpu->getFlags();
		case 0x01:	if(gpu) return gpu->getMainColours();
		case 0x02: 	if(gpu) return gpu->getCharSetPtrLo();
		case 0x03: 	if(gpu) return gpu->getCharSetPtrHi();
		case 0x04: 	if(gpu) return gpu->getCharDisplayPtrLo();
		case 0x05: 	if(gpu) return gpu->getCharDisplayPtrHi();
	}
	return 0xff;
}

//========================================================================
//
// Return the byte buffer of RGB24s
//
//========================================================================
void GIO::setByte(int offset, byte value)
{
	switch(offset) {
		case 0x00:	if(gpu) { gpu->setFlags(value); }
		case 0x01:	if(gpu) { gpu->setMainColours(value); }
		case 0x02: 	if(gpu) { gpu->setCharSetPtrLo(value); }
		case 0x03: 	if(gpu) { gpu->setCharSetPtrHi(value); }
		case 0x04: 	if(gpu) { gpu->setCharDisplayPtrLo(value); }
		case 0x05: 	if(gpu) { gpu->setCharDisplayPtrHi(value); }
	}
}
