//=======================================================================================
//
// G64.cpp
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

#include "G64.h"

#include "G6510.h"
#include "GMemMgr.h"
#include "GVICII.h"

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
//	$E000-$EFFF - (I/O registers)
//	$F000-$FFFF - (KERNAL)
//=======================================================================================
#define CHARSET_ROM (0xD)
#define IO_CHIP (0xE)

G64::G64()
{
	mem = new GMemMgr();

	mem->setPageChip(CHARSET_ROM, &charRom);
	mem->setPageChip(IO_CHIP, &io);

	mem->setRomSwitches((1<<CHARSET_ROM) | (1<<IO_CHIP));

    cpu = new G6510(mem);
    gpu = new GVICII(mem);

    io.setGpu(gpu);
}

G64::~G64()
{
    if (mem != NULL) {
        delete mem;
        mem = NULL;
    }
    if (cpu != NULL) {
        delete cpu;
        cpu = NULL;
    }
    if (gpu != NULL) {
        delete gpu;
        gpu = NULL;
    }
}


//========================================================================
//
// Load a C64-format PRG file and set 6510's program counter
//
//========================================================================
int G64::loadPrg(const char* const filePath) {
	if(cpu == NULL) {
		return -1;
	}

	FILE* fptr = fopen(filePath, "rb");

	if(fptr == NULL) {
		return -2;
	}

	byte fileByte;
	word codePtr = 0;

	// read program location

	// low byte
	fread(&fileByte, 1, 1, fptr);
	codePtr |= fileByte;
	cpu->setProgramCounterLo(fileByte);

	// high byte
	fread(&fileByte, 1, 1, fptr);
	codePtr |= (word)fileByte << 8;
	cpu->setProgramCounterHi(fileByte);

	while (fread(&fileByte, 1, 1, fptr) == 1) {
		mem->setByte(codePtr++, fileByte);
	}

	fclose(fptr);

	return 0;
}

//========================================================================
//
// Start running the G6510
//
//========================================================================
void G64::start() {
	if (cpu != NULL) {
		cpu->start();
	}
}

//========================================================================
//
// Stop running the G6510
//
//========================================================================
void G64::stop() {
	if (cpu != NULL) {
		cpu->stop();
	}
}
