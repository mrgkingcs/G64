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


//=======================================================================================
//
//	Offset mapping:
//
//	$0000-$0010 = GPU
//		$0000 -> byte flags (bit 0 : 1 = don't display main screen; 0 = display)
// 							bit 4-7 : border colour
//		$0001 -> main colours: fg = low nibble, bg = high nibble
//		$0002/3 -> char set ptr : RAM location of charset data (indexed by PETSCII codes) LO/HI
//		$0004/5 -> char display ptr : RAM location of PETSCII char codes to draw on-screen LO/HI
//
//=======================================================================================



#ifndef GIO_H
#define GIO_H

#include "type.h"
#include "GRom.h"

class GVICII;

class GIO : public GRom
{
	public:
		GIO();
		virtual ~GIO();

		void setGpu(GVICII* _gpu) { gpu = _gpu; }

		virtual const byte getByte(int offset);
		virtual void setByte(int offset, byte value);
		virtual const byte* const getMem(int offset, int size) { return NULL; }

	protected:

	private:
		GVICII* gpu;
};

#endif // GIO_H
