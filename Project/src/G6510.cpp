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

#define CHARSET_ROM (0xD)

//=======================================================================================
// Chip/ROM Layout:
//
//	(first two bytes in zero-page are bitfields: 1=Chip/ROM, 0=normal RAM)
//
//	$0000-$0FFF	- no ROM, just zero-page config
//	$1000-$1FFF -
//	$2000-$2FFF -
//	$3000-$3FFF -
//	$4000-$4FFF -
//	$5000-$5FFF -
//	$6000-$6FFF -
//	$7000-$7FFF -
//	$8000-$8FFF -
//	$9000-$9FFF -
//	$A000-$AFFF -
//	$B000-$BFFF -
//	$C000-$CFFF -
//	$D000-$DFFF - charset ROM
//	$E000-$EFFF - I/O registers
//	$F000-$FFFF - KERNAL
//=======================================================================================


#include "G6510.h"

#include "GCharRom.h"

typedef struct _ZERO_PAGE_CONFIG {
	word romSwitches;		// each bit: 1 = ROM active, 0 = RAM page active
	byte gpuFlags;			// bit 0 : 1 = don't display main screen; 0 = display
							// bit 4-7 : border colour
	byte mainScreenColours;	// fg = low nibble, bg = high nibble
	word charSetPtr;		// RAM location of charset data (indexed by PETSCII codes)
	word charDisplayPtr;	// RAM location of PETSCII char codes to draw on-screen
} ZeroPageConfig;

G6510::G6510()
{
    RAM = new unsigned char[RAM_SIZE];

    for(int romIndex = 0; romIndex < MAX_ROMS; romIndex++) {
		roms[romIndex] = NULL;
    }
    roms[CHARSET_ROM] = new GCharRom();

    // set up zero page config
    ZeroPageConfig* config = (ZeroPageConfig*)RAM;
    config->romSwitches = 0x2000;	// only char set ROM for now
    config->gpuFlags = 1 | (14 << 4);
    config->mainScreenColours = 14 | (6 << 4);
    config->charSetPtr = 0xD000;	// read charset directly from ROM
    config->charDisplayPtr = 0xC000;	// default screen memory
}

G6510::~G6510()
{
    if(RAM != NULL) {
        delete [] RAM;
        RAM = NULL;
    }

    for(int romIndex = 0; romIndex < MAX_ROMS; romIndex++) {
		if(roms[romIndex] != NULL) {
			delete roms[romIndex];
			roms[romIndex] = NULL;
		}
    }
}

const byte G6510::getByte(int offset)
{
	int page = offset >> 12;
	int romMask = 1 << page;

	ZeroPageConfig* config = (ZeroPageConfig*)RAM;
	if((config->romSwitches & romMask) != 0) {
		if(roms[page] != NULL) {
			return roms[page]->getByte(offset & 0x0FFF);
		}
	}
	return RAM[offset];
}

const byte* const G6510::getMem(int offset, int size)
{
	int page = offset >> 12;
	int romMask = 1 << page;

	ZeroPageConfig* config = (ZeroPageConfig*)RAM;
	if((config->romSwitches & romMask) != 0) {
		if(roms[page] != NULL) {
			return roms[page]->getMem(offset & 0x0FFF, size);
		}
	}
	if(offset < RAM_SIZE && (offset+size) <= RAM_SIZE) {
		return RAM+offset;
	}
	return NULL;
}
