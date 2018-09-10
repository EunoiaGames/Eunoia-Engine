#pragma once

#define NULL 0
#include <iostream>

namespace Eunoia {

	typedef signed char int8;
	typedef unsigned char uint8;
	typedef signed short int int16;
	typedef unsigned short int uint16;
	typedef signed int int32;
	typedef unsigned int uint32;
	typedef signed long long int64;
	typedef unsigned long long uint64;

#define EU_PAUSE() system("PAUSE")
#define EU_FORCE_SHUTDOWN(Message) std::cout << Message << std::endl; EU_PAUSE(); std::exit(-1)
#define EU_ASSERT(Condition, Message) if(!(Condition)) { EU_FORCE_SHUTDOWN(Message); }

}