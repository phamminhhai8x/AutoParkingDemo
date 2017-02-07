/*
 Name:		myOS.ino
 Created:	3/3/2016 8:50:44 AM
 Author:	Fresher104
*/
// Include Dictionary

#include "Dictionary\Header\header.h"
#include "Dictionary\Flag\flag.h"
#include "Dictionary\Predefine\Predefine.h"
extern "C" {
// Include OS
#include "myOS.h"
#include "myOS_hard_init.h"
}

#if CANMODE != 0
#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
#endif

#define _stop 3
#define forward 1
#define backward 0

uint32 duration1 = 0;
uint32 duration2 = 0;
uint32 duration3 = 0;
uint32 duration4 = 0;
uint32 duration5 = 0;
uint32 duration6 = 0;
uint32 duration7 = 0;
uint32 duration8 = 0;


uint32 dRR_S = 0;
uint32 dBR_S = 0;
uint32 dFF_S = 0;
uint32 dFR_S = 0;
uint32 dBB_S = 0;
uint32 dBL_S = 0;


uchar8 dFF = 0;
uchar8 dFR = 0;
uchar8 dRR = 0;
uchar8 dBR = 0;
uchar8 dBB = 0;
uchar8 dBL = 0;
uchar8 dLL = 0;
uchar8 dFL = 0;

uchar8 _dFF = 0;
uchar8 _dFR = 0;
uchar8 _dRR = 0;
uchar8 _dBR = 0;
uchar8 _dBB = 0;
uchar8 _dBL = 0;
uchar8 _dLL = 0;
uchar8 _dFL = 0;

uint16 i = 0;
uint16 cTime = 0;
uchar8 avoid = 0;

unsigned char len = 0;
unsigned char buf[8];

void setup_stepper();
void stepper_controller(schar8 vel);


// the setup function runs once when you press reset or power the board
void setup() {
	#if DEBUGMODE != 0
	Serial.begin(9600);
	#endif
	delay(1000);
	cli();					// Disable Interrupts
	#if CANMODE != 0
	START_INIT:

	while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
    }
    Serial.println("CAN BUS Shield init ok!");

#endif
	// Init OS, IO, Timer, Variable
	hardware_init();
	variable_init();
	digitalWrite(7, LOW);

	digitalWrite(9, LOW);

	setup_stepper();
	sei();					// Enable Interrupts
}

long _map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// the loop function runs over and over again until power down or reset
void loop() {
	if(millis() - cTime >= 10){
		cTime = millis();

	duration1 = pulseIn(5,HIGH, 20000L);
	_dFF = duration1 / 58;
	dFF = _dFF >= 2 ? _dFF : dFF;
	if (dFF >= 100) dFF = 100;

	duration2 = pulseIn(6,HIGH, 20000L);
	_dFR = duration2 / 58;
	dFR = _dFR >= 2 ? _dFR : dFR;
	if (dFR >= 100) dFR = 100;

	duration3 = pulseIn(0,HIGH, 20000L);
	_dRR = duration3 / 58;
	dRR = _dRR >= 2 ? _dRR : dRR;

	duration4 = pulseIn(1,HIGH, 20000L);
	_dBR = duration4 / 58;
	dBR = _dBR >= 2 ? _dBR : dBR;

	duration5 = pulseIn(3,HIGH, 20000L);
	_dBB = duration5 / 58;
	dBB = _dBB >= 2 ? _dBB : dBB;

	duration6 = pulseIn(4,HIGH, 20000L);
	_dBL = duration6 / 58;
	dBL = _dBL >= 2 ? _dBL : dBL;

	duration7 = pulseIn(A4,HIGH, 20000L);
	_dLL = duration7 / 58;
	dLL = _dLL >= 2 ? _dLL : dLL;

	duration8 = pulseIn(A5,HIGH, 20000L);
	_dFL = duration8 / 58;
	dFL = _dFL >= 2 ? _dFL : dFL;

	uchar8 all_Distance[8] = {dFF, dFR, dRR, dBR, dBB, dBL, dLL, dFL};
	uchar8* flag;
	flag = set_Distance_Flag(all_Distance, 20);
	bool obs = checkOBS(flag);

	dFF_S = smooth(dFF, 0.6, dFF_S);
	dFR_S = smooth(dFR, 0.6, dFR_S);
	dBB_S = smooth(dBB, 0.6, dBB_S);
	dBL_S = smooth(dBL, 0.6, dBL_S);

	byte message[8] = {dFF,dBB, dBL,dFR, dRR, 0, 0, 0};
	#if CANMODE!=0
	CAN.sendMsgBuf(0x00, 0, 8, message);


	if (CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
	{
	CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
		unsigned char canId = CAN.getCanId();
		Serial.println("-----------------");
		//Serial.print("Get data from ID: ");
		//Serial.println(canId, HEX);

		//for (int i=0; i<2; i++){
		stepper_mode1 = buf[0];
		kaka = buf[1];
			
		//_servo.x[i] = buf[i+2];
		//_time.x[i] = buf[i+4];
		//}
	}
	stepper_mode = map(stepper_mode1, 0, 8, -4, 4);
	stepper_controller(stepper_mode);

#endif

		
#if DEBUGMODE != 0
	if(obs)
		Serial.println("OBSTACLES");
	else
		Serial.println("GO");

	Serial.println("==============");
	//Serial.print("Sensor 1: ");
	//Serial.println(dFF);
	Serial.print("Sensor 1: ");
	Serial.println(dFF);
	Serial.print("Sensor 2: ");
	Serial.println(dBB);
	Serial.print("Sensor 3: ");
	Serial.println(dRR);
	Serial.print("Sensor 4: ");
	Serial.println(dBL);
	Serial.print("Sensor 5: ");
	Serial.println(dFR);
	Serial.print("Stepper_mode : ");
	Serial.println(stepper_mode);
	Serial.print("KAka : ");
	Serial.println(kaka);
	/*Serial.print("Sensor 6: ");
	Serial.println(dBL);
	Serial.print("Sensor 7: ");
	Serial.println(dLL);
	Serial.print("Sensor 8: ");
	Serial.println(dFL);*/
	Serial.println("==============================");

#endif
	}
	
}



void setup_stepper()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0;
	TIMSK1 = 0;

	TCCR1A |= (1 << COM1A0); // CTC (toggle)

	TCCR1B |= (1 << WGM12) | (1 << CS11); // scale 64 
}

void stepper_controller(schar8 vel)  // vel (revolutions / second)
{
	/*
	1 step = 1.8 degrees
	200 steps = 360 degrees
	stepper 1/8 (default: adjusted by MS1 and MS2): 1600 microsteps = 360 degrees
	period of clock scale = 1 / (16 * 10^6 / 8) = 0.5 * 10^-6
	pulse : Time HIGH = Time LOW = OCR1A * 0.5 * 10^-6
	period of 1 pulse = 2 * OCR1A * 0.5 * 10^-6 = OCR1A * 10^-6
	=> 10^6 / OCR1A (pulses / second)
	=> 10^6 / OCR1A / 1600 = 625 / OCR1A = vel (revolutions / second)
	=> OCR1A = 625 / vel
	*/
	if (vel < 0)
	{
		digitalWrite(7, LOW);
		vel = vel * (-1);
		OCR1A = (unsigned int)625 / vel;
	}
	else if (vel == 0)
	{
		OCR1A = 0;
	}
	else if (vel > 0)
	{
		digitalWrite(7, HIGH);
		OCR1A = (unsigned int)625 / vel;
	}

}

int smooth(int data, float filterVal, float smoothedVal) {


	if (filterVal > 1) {      // check to make sure param's are within range
		filterVal = .99;
	}
	else if (filterVal <= 0) {
		filterVal = 0;
	}

	smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

	return (int)smoothedVal;
}

uchar8* set_Distance_Flag(uchar8 di[8], uchar8 d){
	uchar8 * re;
	for(int i = 0; i<sizeof(di); i++)
	{
		if(di[i] > d)
			re[i] = 0;
		else
			re[i] = 1;
	}
	return re;
}

bool checkOBS(uchar8 di[8]){
	bool r = 0;
	for(int i = 0; i<sizeof(di); i++)
	{
		if(di[i]  == 1){
			r = 1;
			break;
		}	
	}
	return r;
}
/* Interrupt Vector for OS */

ISR (TIMER2_COMPA_vect) 
{
	OS_control();
}





















//unsigned long rangeB(uint8_t pin, unsigned long timeout )
//{
//
//	uint8_t _pin=pin-8;
//unsigned long width = 0; // keep initialization out of time critical area
//
//// convert the timeout from microseconds to a number of times through
//// the initial loop; it takes 16 clock cycles per iteration.
//unsigned long numloops = 0;
//unsigned long maxloops = (timeout) ; // timeout*10^-6 : (1:(16*10^6) /16;
//
//// wait for any previous pulse to end
//while ((PINB & (1<<_pin)) == (1<<_pin))
//if (numloops++ == maxloops)
//return 0;
//
//// wait for the pulse to start
//while ((PINB & (1<<_pin)) != (1<<_pin))
//if (numloops++ == maxloops)
//return 0;
//
//// wait for the pulse to stop
//while ((PINB & (1<<_pin)) == (1<<_pin)) {
//if (numloops++ == maxloops)
//return 0;
//width++;
//}
//return (width/20*16);
//}
//
//unsigned long rangeD(uint8_t pin, unsigned long timeout )
//{
//
//
//unsigned long width = 0; // keep initialization out of time critical area
//
//// convert the timeout from microseconds to a number of times through
//// the initial loop; it takes 16 clock cycles per iteration.
//unsigned long numloops = 0;
//unsigned long maxloops = (timeout) ; // timeout*10^-6 : (1:(16*10^6) /16;
//
//// wait for any previous pulse to end
//while ((PIND & (1<<pin)) == (1<<pin))
//if (--maxloops == 0)
//return 0;
//
//// wait for the pulse to start
//while ((PIND & (1<<pin)) != (1<<pin))
//if (--maxloops == 0)
//return 0;
//
//// wait for the pulse to stop
//while ((PIND & (1<<pin)) == (1<<pin)) {
//if (++width == maxloops)
//return 0;
//
//}
//return width+1;
//}
//
//unsigned long _pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
//{
//	// cache the port and bit of the pin in order to speed up the
//	// pulse width measuring loop and achieve finer resolution.  calling
//	// digitalRead() instead yields much coarser resolution.
//	uint8_t bit = digitalPinToBitMask(pin);
//	uint8_t port = digitalPinToPort(pin);
//	uint8_t stateMask = (state ? bit : 0);
//
//	// convert the timeout from microseconds to a number of times through
//	// the initial loop; it takes approximately 16 clock cycles per iteration
//	unsigned long maxloops = microsecondsToClockCycles(timeout)/16;
//
//	unsigned long width = _pulseIn1Simpl(portInputRegister(port), bit, stateMask, maxloops);
//
//	// prevent clockCyclesToMicroseconds to return bogus values if countPulseASM timed out
//	if (width)
//		return clockCyclesToMicroseconds(width * 16 + 16);
//	else
//		return 0;
//}
//
//unsigned long _pulseIn1Simpl(volatile uint8_t *port, uint8_t bit,uint8_t stateMask,unsigned long maxloops)
//{
//      unsigned long width = 0;
//     // wait for any previous pulse to end
//      while ((*port & bit) == stateMask)
//          if (--maxloops == 0)
//              return 0;
// 
//      // wait for the pulse to start
//      while ((*port & bit) != stateMask)
//          if (--maxloops == 0)
//              return 0;
// 
//      // wait for the pulse to stop
//      while ((*port & bit) == stateMask) {
//          if (++width == maxloops)
//              return 0;
//      }
//	  return width;
//}