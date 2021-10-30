//=======================================================================================
//
// GVICII.h
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


#ifndef GVICII_H
#define GVICII_H

#include "type.h"
#include <thread>

#include "GColour.h"

class GMemMgr;

class GVICII
{
    public:
        GVICII(GMemMgr* _mem);
        virtual ~GVICII();

        // calls from outside machine to get rendered result
        const word* getFrameBuffer();
        const int getFrameBufferHeight();
        const int getFrameBufferWidth();

        const GColour getBorderColour();

        // calls from inside machine to interact with chip
        void setFlags(byte value)	{ registers.flags = value; }
        byte getFlags()				{ return registers.flags; }

        void setMainColours(byte value)	{ registers.mainColours = value; }
        byte getMainColours()			{ return registers.mainColours; }

        void setCharSetPtrLo(byte value)	{ registers.charSetPtr = ((registers.charSetPtr & 0xff00) | (value)); }
        byte getCharSetPtrLo()				{ return registers.charSetPtr & 0xff; }

        void setCharSetPtrHi(byte value)	{ registers.charSetPtr = ((registers.charSetPtr & 0x00ff) | ((word)value << 8)); }
        byte getCharSetPtrHi()				{ return registers.charSetPtr >> 8; }

        void setCharDisplayPtrLo(byte value)	{ registers.charDisplayPtr = ((registers.charDisplayPtr & 0xff00) | (value)); }
        byte getCharDisplayPtrLo()				{ return registers.charDisplayPtr & 0xff; }

        void setCharDisplayPtrHi(byte value)	{ registers.charDisplayPtr = ((registers.charDisplayPtr & 0x00ff) | ((word)value << 8)); }
        byte getCharDisplayPtrHi()				{ return registers.charDisplayPtr >> 8; }


    protected:

    private:
		struct _Registers {
			byte flags;
			byte mainColours;
			word charSetPtr;
			word charDisplayPtr;
		};

		_Registers registers;


        GMemMgr* mem;
        word* frameBuffer;
        std::thread updateThread;

        void updateFrameBuffer();

};

#endif // GVICII_H
