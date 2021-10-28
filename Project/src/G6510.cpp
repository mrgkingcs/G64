//=======================================================================================
//
// G6510.cpp
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


#include "G6510.h"

#include "GMemMgr.h"


G6510::G6510(GMemMgr* _mem)
{
	mem = _mem;

	// this whole thing should live somewhere else
	// set ROM switches
	mem->setByte(0, 0x00);
	mem->setByte(1, 0x20);

	// gpu flags
	mem->setByte(2, 1 | (14 << 4));

	// main screen colours
	mem->setByte(3, 14 | (6<<4));

	// charset ptr
	mem->setByte(4, 00);
	mem->setByte(5, 0xD0);

	// char display ptr
	mem->setByte(6, 00);
	mem->setByte(7, 0xC0);

	// DEBUG: put the entire charset onto screen
	int offset = 0xC000;
    for(int counter = 0; counter < 256; counter++) {
		mem->setByte(offset++, counter);
    }

}

G6510::~G6510()
{

}


