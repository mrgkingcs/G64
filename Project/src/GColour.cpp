//=======================================================================================
//
// GColour.cpp
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


#include "GColour.h"

GColour GColour::COLOUR_DEFS[NUM_COLOURS] = {
	GColour(0, 0, 0),
	GColour(0xff, 0xff, 0xff),
	GColour(0x9f, 0x4e, 0x44),
	GColour(0x6a, 0xbf, 0xc6),
	GColour(0xa0, 0x57, 0xa3),
	GColour(0x5c, 0xab, 0x5e),
	GColour(0x50, 0x45, 0x9b),
	GColour(0xc9, 0xd4, 0x87),
	GColour(0xa1, 0x68, 0x3c),
	GColour(0x6d, 0x54, 0x12),
	GColour(0xcb, 0x7e, 0x75),
	GColour(0x62, 0x62, 0x62),
	GColour(0x89, 0x89, 0x89),
	GColour(0x9a, 0xe2, 0x9b),
	GColour(0x88, 0x7e, 0xcb),
	GColour(0xad, 0xad, 0xad)
};

GColour::GColour(byte r, byte g, byte b)
{
	rgb = pack(r, g, b);
}

GColour::GColour(const GColour& other) {
	rgb = other.rgb;
}

GColour::~GColour()
{
    //dtor
}


GColour GColour::getColourByIndex(int index) {
	return COLOUR_DEFS[index];
}
