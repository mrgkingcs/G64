//=======================================================================================
//
// G6510.h
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

#ifndef G6510_H
#define G6510_H

#include "type.h"
#include <thread>



class GMemMgr;

class G6510
{
    public:
        G6510(GMemMgr* _mem);
        virtual ~G6510();

        void setProgramCounterLo(byte value)	{ registers.programCounter = (registers.programCounter & 0xff00) | value; }
        void setProgramCounterHi(byte value)	{ registers.programCounter = (registers.programCounter & 0xff) | ((word)value << 8); }

        void start()	{ registers.control = CONTROL_RUNNING;	}
        void stop()		{ registers.control = CONTROL_STOPPED;	}

    private:
		enum {
			CONTROL_STOPPED = 0,
			CONTROL_RUNNING = 1
		};

		enum {
			FLAG_NEGATIVE = 	(1<<7),
			FLAG_OVERFLOW = 	(1<<6),
			FLAG_BREAK = 		(1<<4),
			FLAG_DECIMAL = 		(1<<3), // not going to use this
			FLAG_IRQ_DISABLE = 	(1<<2),
			FLAG_ZERO = 		(1<<1),
			FLAG_CARRY = 		(1<<0)
		};

		struct _Registers {
			byte control;
			byte status_flags;
			word programCounter;
			byte acc;
			byte x;
			byte y;
		};

		_Registers registers;

		GMemMgr* mem;

        std::thread executionThread;

		// execution thread methods
        void fetchExecute();
        byte fetch();
        word fetchAddress();

        bool getFlag(byte flagMask)		{ return registers.status_flags & flagMask;	}
        void setFlag(byte flagMask)		{ registers.status_flags |= flagMask;		}
        void clearFlag(byte flagMask)	{ registers.status_flags &= (~flagMask);	}
        void setFlagTo(byte flagMask, bool value) {
			if(value)
				setFlag(flagMask);
			else
				clearFlag(flagMask);
        }
};

#endif // G6510_H
