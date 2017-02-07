// 
// 
// 
#include <avr/io.h>
#include <util/delay.h>

// Include Dictionary
#include "..\Dictionary\Header\header.h"
#include "..\Dictionary\Flag\flag.h"
#include "..\Dictionary\Predefine\Predefine.h"
#include "OS_JOBS.h"



void ping(){
PORTD |= _BV(1);
}
void clearPing(){
PORTD &= ~_BV(1);
}
void delay5u()
{
	_delay_us(5);
}
void sensor1(){
	digitalWrite(8, LOW);
	delayMicroseconds(2);
	digitalWrite(8, HIGH);
	delayMicroseconds(5);
	digitalWrite(8, LOW);
}
void sensor2(){
	digitalWrite(4, HIGH);
	delayMicroseconds(5);
	digitalWrite(4, LOW);
}
void sensor3(){
	digitalWrite(6, HIGH);
	delayMicroseconds(5);
	digitalWrite(6, LOW);
}
/*********JOBS 10ms**********/
#if NO_JOB_10MS != 0
static void (*const JOBS10ms[])() =
{
	&sensor1
};
#endif


/*********JOBS 100ms**********/
#if NO_JOB_100MS != 0
static void (*const JOBS100ms[])() =
{

};
#endif

/*********JOBS APP**********/
#if NO_JOB_APP != 0
static void (*const JOBSAPP[])() =
{

};
#endif

/********************************************************************/
/* Function			: execute_JOB									*/
/* Description		: Call all functions based on JOB mode			*/
/* Argument			: void											*/
/* Return			: void											*/
/********************************************************************/
void execute_JOB(void)
{
	uchar8 cUC8_indexArr; /* loop counter */

	if (TSTBIT(fUC8_JOBRunning)) /* IO JOB is running now! */
	{
		/* nothing if have JOB running */
	}
	else
	{
		SETBIT(fUC8_JOBRunning);
		do
		{
			/* Check flag fJOB10ms = 1 */
			if ((fUC8_JOB10ms != OFF))	
			{
#if NO_JOB_10MS != 0
				for (cUC8_indexArr = 0U; cUC8_indexArr < NO_JOB_10MS; cUC8_indexArr++)
				{
					(*JOBS10ms[cUC8_indexArr])();			/* Call all JOB 10ms*/
				}
#endif
				CLRBIT(fUC8_JOB10ms);						/* Clear flag fJOB10ms */
			}
			else if (fUC8_JOB100ms != OFF)					/* Check flag fJOB100ms = 1 */
			{
#if NO_JOB_100MS != 0
				for (cUC8_indexArr = 0U; cUC8_indexArr < NO_JOB_100MS; cUC8_indexArr++)
				{
					(*JOBS100ms[cUC8_indexArr])();			/* Call all JOB 100ms*/
				}
#endif
				CLRBIT(fUC8_JOB100ms);						/* Clear flag fJOB100ms */
			}
			else if (fUC8_JOBApp != OFF)					/* Check flag fUC8_JOBApp = 1 */
			{
#if NO_JOB_APP != 0
				for (cUC8_indexArr = 0U; cUC8_indexArr < NO_JOB_APP; cUC8_indexArr++)
				{
					(*JOBSAPP[cUC8_indexArr])();			/* Call all JOB App*/
				}
#endif
				CLRBIT(fUC8_JOBApp);						/* Clear flag fUC8_JOBApp */
			}
			else
			{
				CLRBIT(fUC8_JOBRunning);					/* Clear flag fJOBRunning */
			}
		}while(TSTBIT(fUC8_JOBRunning));
	}
} /* End of execute_JOB() */
