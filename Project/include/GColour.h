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

#define CHANNEL_MASK 	((1<<5)-1)

#define R_SHIFT		(0)
#define G_SHIFT		(5)
#define B_SHIFT		(10)

#define MASK_R 			(CHANNEL_MASK)
#define MASK_G 			(CHANNEL_MASK<<G_SHIFT)
#define MASK_B 			(CHANNEL_MASK<<B_SHIFT)

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

        byte getRedByte() const		{ return unpackR()<<3; }
        byte getGreenByte() const	{ return unpackG()<<3; }
        byte getBlueByte() const	{ return unpackB()<<3; }

        float getRedFloat() const	{ return (float)(unpackR()<<3)/255.f; }
        float getGreenFloat() const	{ return (float)(unpackG()<<3)/255.f; }
        float getBlueFloat() const	{ return (float)(unpackB()<<3)/255.f; }

        static GColour getColourByIndex(int index);

    private:
		static GColour COLOUR_DEFS[NUM_COLOURS];

        //byte rgb[3];

        inline byte unpackR() const {	return (byte)((rgb & MASK_R)>>R_SHIFT);	}
        inline byte unpackG() const {	return (byte)((rgb & MASK_G)>>G_SHIFT);	}
        inline byte unpackB() const {	return (byte)((rgb & MASK_B)>>B_SHIFT);	}

        inline word pack(byte r, byte g, byte b ) {
			return (word)(
							((r>>3) << R_SHIFT)   |
							((g>>3) << G_SHIFT)   |
							((b>>3) << B_SHIFT)   |
							0x8000	// alpha
					);
        }

        word rgb;	// GL_UNSIGNED_SHORT_5_5_5_1
};

#endif // GCOLOUR_H
