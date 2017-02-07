// OS_JOBS.h

#ifndef _OS_JOBS_h
#define _OS_JOBS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/* Number of job with unit 10ms, 100ms */
#define NO_JOB_10MS 1U
#define NO_JOB_100MS 0U
#define NO_JOB_APP 0U

void execute_JOB(void);

#endif

