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

#if CANMODE != 0
#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN1(SPI_CS_PIN);

void CAN_Receive() {
	unsigned char len = 0;
	unsigned char buf[8];
	unsigned char stmp[8] = { mode, 1, 2, 0, 0, 0, 0, 0};

	if (CAN_MSGAVAIL == CAN1.checkReceive())            // check if data coming
	{
		CAN1.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
		unsigned char canId = CAN1.getCanId();
		//Serial.println("-----------------");
		//Serial.print("Get data from ID: ");
		//Serial.println(canId, HEX);

		//for (int i=0; i<2; i++){
		d[0] = buf[0];
		d[1] = buf[1];
		d[2] = buf[2];
		d[3] = buf[3];
		d[4] = buf[4];			//_servo.x[i] = buf[i+2];
		//_time.x[i] = buf[i+4];
		//}
	}
	CAN1.sendMsgBuf(0x01, 0, 8, stmp);
}
#endif


/*********JOBS 10ms**********/
#if NO_JOB_10MS != 0
static void (*const JOBS10ms[])() =
{
	&CAN_Receive
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
