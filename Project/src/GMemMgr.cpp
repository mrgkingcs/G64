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


#include "GMemMgr.h"

#include "GRom.h"
#include "GCharRom.h"





typedef struct _ZERO_PAGE_CONFIG {
	word romSwitches;		// each bit: 1 = ROM active, 0 = RAM page active
	byte gpuFlags;			// bit 0 : 1 = don't display main screen; 0 = display
							// bit 4-7 : border colour
	byte mainScreenColours;	// fg = low nibble, bg = high nibble
	word charSetPtr;		// RAM location of charset data (indexed by PETSCII codes)
	word charDisplayPtr;	// RAM location of PETSCII char codes to draw on-screen
} ZeroPageConfig;



GMemMgr::GMemMgr()
{
    RAM = new unsigned char[RAM_SIZE];

    for(int pageIndex = 0; pageIndex < MAX_PAGE_CHIPS; pageIndex++) {
		pageChips[pageIndex] = NULL;
		activePageChips[pageIndex] = NULL;
    }
}

GMemMgr::~GMemMgr()
{
    if(RAM != NULL) {
        delete [] RAM;
        RAM = NULL;
    }
}

const byte GMemMgr::getByte(int offset) const
{
	int page = offset >> 12;

	if(activePageChips[page] != NULL) {
		return activePageChips[page]->getByte(offset & 0x0FFF);
	}

	return RAM[offset];
}


void GMemMgr::setByte(int offset, byte value)
{
	int page = offset >> 12;

	if(activePageChips[page] != NULL) {
		activePageChips[page]->setByte(offset & 0x0FFF, value);
	} else {
		RAM[offset] = value;
	}

	if(offset == 0 || offset == 1) {
		updateActivePageChips();
	}
}


const byte* const GMemMgr::getMem(int offset, int size) const
{
	int page = offset >> 12;

	if(activePageChips[page] != NULL) {
		activePageChips[page]->getMem(offset & 0x0FFF, size);
	} else if (offset < RAM_SIZE && (offset+size) <= RAM_SIZE) {
		return RAM+offset;
	}
	return NULL;
}


void GMemMgr::setPageChip(byte pageID, GRom* chip) {
	if(pageID > 0 && pageID < MAX_PAGE_CHIPS) {
		pageChips[pageID] = chip;
	}
	updateActivePageChips();
}

void GMemMgr::setRomSwitches(word switches) {
	RAM[0] = switches & 0xff;
	RAM[1] = switches >> 8;
	updateActivePageChips();
}

void GMemMgr::updateActivePageChips() {
	word bitField = RAM[0] | ((word)(RAM[1]) << 8);
	for(int pageIndex = 0; pageIndex < MAX_PAGE_CHIPS; pageIndex++) {
		word mask = 1 << pageIndex;
		if ((bitField & mask) != 0) {
			activePageChips[pageIndex] = pageChips[pageIndex];
		} else {
			activePageChips[pageIndex] = NULL;
		}
	}
}
