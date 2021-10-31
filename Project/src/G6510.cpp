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

#include "plf_nanotimer.h"

G6510::G6510(GMemMgr* _mem) : executionThread(&G6510::fetchExecute, this)
{
	mem = _mem;

	registers.control = CONTROL_STOPPED;

	// DEBUG: put the entire charset onto screen
	int offset = 0xC028;
    for(int counter = 0; counter < 256; counter++) {
		mem->setByte(offset++, counter);
    }

}

G6510::~G6510()
{
	executionThread.join();
}



//============================================================================
//
// The following subroutines are ONLY to be called from the execution thread
//
//============================================================================
void G6510::fetchExecute() {
	plf::nanotimer timer;
	timer.start();

	while(1) {
		if (timer.get_elapsed_us() > 1 && registers.control == CONTROL_RUNNING) {
			timer.start();

			byte opCode = fetch();
			switch(opCode) {
				case 0x60:	// RTS
				{
					// for now, just stop execution
					registers.control = CONTROL_STOPPED;
					break;
				}
				case 0x8d:	// STA absolute
				{
					word address = fetchAddress();
					mem->setByte(address, registers.acc);
					break;
				}
				case 0x91: // STA (zero page),Y
				{
					byte zeroPageAddr = fetch();
					word address = mem->getByte(zeroPageAddr);
					address |= ((word)mem->getByte(zeroPageAddr+1)) << 8;
					address += registers.y;
					mem->setByte(address, registers.acc);
					break;
				}
				case 0xa0:	// LDY #imm
				{
					registers.y = fetch();
					setFlagTo(FLAG_NEGATIVE, registers.y & 0x80);
					setFlagTo(FLAG_ZERO, registers.y == 0);
					break;
				}
				case 0xa2:	// LDX #imm
				{
					registers.x = fetch();
					setFlagTo(FLAG_NEGATIVE, registers.x & 0x80);
					setFlagTo(FLAG_ZERO, registers.x == 0);
					break;
				}
				case 0xa9:	// LDA #imm
				{
					registers.acc = fetch();
					setFlagTo(FLAG_NEGATIVE, registers.acc & 0x80);
					setFlagTo(FLAG_ZERO, registers.acc == 0);
					break;
				}
				case 0xbd:	// LDA absolute, X
				{
					word address = fetchAddress();
					address += registers.x;
					registers.acc = mem->getByte(address);
					setFlagTo(FLAG_NEGATIVE, registers.acc & 0x80);
					setFlagTo(FLAG_ZERO, registers.acc == 0);
					break;
				}
				case 0xc8:	// INY
				{
					registers.y++;
					setFlagTo(FLAG_NEGATIVE, registers.y & 0x80);
					setFlagTo(FLAG_ZERO, registers.y == 0);
					break;
				}
				case 0xe8:	// INX
				{
					registers.x++;
					setFlagTo(FLAG_NEGATIVE, registers.x & 0x80);
					setFlagTo(FLAG_ZERO, registers.x == 0);
					break;
				}
				case 0xd0:	// BNE
				{
					char relative = fetch();
					if(!getFlag(FLAG_ZERO)) {
						registers.programCounter += relative;
					}
					break;
				}
				case 0xf0:	// BEQ
				{
					char relative = fetch();
					if(getFlag(FLAG_ZERO)) {
						registers.programCounter += relative;
					}
					break;
				}
			}
		}

		std::this_thread::yield();
	}
}

byte G6510::fetch() {
	return mem->getByte(registers.programCounter++);
}

word G6510::fetchAddress() {
	word address = fetch();
	address |= ((word)fetch()) << 8;
	return address;
}
