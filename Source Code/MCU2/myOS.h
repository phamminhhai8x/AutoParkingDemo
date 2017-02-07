// myOS.h

#ifndef _MYOS_h
#define _MYOS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

void OS_control(void);

#endif

