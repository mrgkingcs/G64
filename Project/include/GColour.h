//=======================================================================================
//
// GColour.h
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


#ifndef GCOLOUR_H
#define GCOLOUR_H

#include "type.h"

#define NUM_COLOURS (16)

class GColour
{
    public:
		enum {
			BLACK = 0,
			WHITE = 1,
			RED = 2,
			CYAN = 3,
			PURPLE = 4,
			GREEN = 5,
			BLUE = 6,
			YELLOW = 7,
			ORANGE = 8,
			BROWN = 9,
			LIGHT_RED = 10,
			DARK_GREY = 11,
			MID_GREY = 12,
			LIGHT_GREEN = 13,
			LIGHT_BLUE = 14,
			LIGHT_GREY = 15
		};

        GColour(byte r, byte g, byte b);
        GColour(const GColour& other);
        virtual ~GColour();

        byte getRedByte() const		{ return rgb[0];	}
        byte getGreenByte() const	{ return rgb[1];	}
        byte getBlueByte() const	{ return rgb[2];	}

        float getRedFloat() const	{ return (float)(rgb[0])/255.f; }
        float getGreenFloat() const	{ return (float)(rgb[1])/255.f; }
        float getBlueFloat() const	{ return (float)(rgb[2])/255.f; }

        static GColour getColourByIndex(int index);

    private:
		static GColour COLOUR_DEFS[NUM_COLOURS];

        byte rgb[3];
};

#endif // GCOLOUR_H
