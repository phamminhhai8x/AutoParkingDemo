/*
Name:		myOS.ino
Created:	3/3/2016 8:50:44 AM
Author:	Fresher104
*/
// Include Fuzzy
#include "fis_header.h"


// Include Dictionary
#include <mcp_can_dfs.h>
#include <Wire.h>
#include "Dictionary\Header\header.h"
#include "Dictionary\Flag\flag.h"
#include "Dictionary\Predefine\Predefine.h"

// Include OS
#include "myOS.h"
#include "myOS_hard_init.h"


#if CANMODE != 0
#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
#endif

#include <Servo.h>

Servo myservo;

#define _stop 0
#define forward 1
#define backward 2


volatile long encoder0Pos = 0;
long newPosition;
long oldPosition;


double vel;
long i, j;
long preflag, cflag;
long distance;

void doEncoder();
unsigned long cTime, cTime2, cTime3;

// Number of inputs to the fuzzy inference system
const int fis_gcI = 2;
// Number of outputs to the fuzzy inference system
const int fis_gcO = 1;
// Number of rules to the fuzzy inference system
const int fis_gcR = 9;
// States

const int WALL_ALIGNMENT	= 0;
const int SCAN_SLOT			= 1;
const int START_MEASURE		= 2;
const int REVERSE_POINT		= 3;
const int PARKING_1			= 4;
const int PARKING_2			= 5;
const int PARKING_3			= 6;
const int PARKING_4			= 7;
const int PARKING_5			= 8;
const int PARKING_6			= 9;
const int PARKING_7			= 10;
const int PARKING__1		= 11;
const int PARKING__2		= 12;
const int PARKING__3		= 13;
const int PARKING__4		= 14;
const int PARKING__5		= 15;
const int PARKING__6		= 16;
const int PARKING__7		= 17;
const int ESCAPE_CAR_1		= 20;
const int ESCAPE_CAR_2		= 21;
const int ESCAPE_CAR_3		= 22;
const int ESCAPE_CAR_4		= 23;
const int ESCAPE_CAR_5		= 24;
const int ESCAPE_CAR_6		= 30;
const int ESCAPE_CAR__1		= 25;
const int ESCAPE_CAR__2		= 26;
const int ESCAPE_CAR__3		= 27;
const int ESCAPE_CAR__4		= 28;
const int ESCAPE_CAR__5		= 29;
const int FINISH			= 99;


//int STATE = WALL_ALIGNMENT;
int STATE = WALL_ALIGNMENT;
int NEXT_STATE = STATE;
int carStatus = 0;
int maxAlign = 4;
char PARKING_MODE = 0; // 1 for parallel parking // 2 for garage parking
char GETCAR_MODE = 0;
FIS_TYPE g_fisInput[fis_gcI];
FIS_TYPE g_fisOutput[fis_gcO];

int dR, dF, dBack, dTop, dMid, gap, v, angle;
// the setup function runs once when you press reset or power the board
void setup() {
	
	// Setup role
	digitalWrite(7, HIGH);
	delay(500);
	
	//Setup PID_ESC
	newPosition = 0;
	oldPosition = 0;
	cTime = 0;
	
	//Setup servo
	myservo.attach(5);
	delay(200);
	myservo.writeMicroseconds(1500);
	//setup_stepper();
	//Setup hall sensor

	//digitalWrite(3, HIGH);       // turn on pullup resistor
	attachInterrupt(1, doEncoder, RISING);  // encoDER ON PIN 2


#if DEBUGMODE != 0
	Serial.begin(9600);
	Serial.println("Start");
#endif
//
//
#if CANMODE != 0
	START_INIT:

			  if (CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
			  {
				  Serial.println("CAN BUS Shield init ok!");
			  }
			  else
			  {
				  Serial.println("CAN BUS Shield init fail");
				  Serial.println("Init CAN BUS Shield again");
				  delay(100);
				  goto START_INIT;
			  }
#endif
			  delay(2000);
			  // Init OS, IO, Timer, Variable
			  hardware_init();
			  variable_init();
			  //PARKING_MODE = '1';
			  //sei();					// Enable Interrupts
}



// the loop function runs over and over again until power down or reset
void loop()
{
	if (millis() - cTime2 >= 1000) {
		Serial.println(carStatus);
		cTime2 = millis();
	}
	if (Serial.available() > 0)
	{
		char bt = Serial.read();
		if (bt == '3') {
			if (PARKING_MODE == '1') {
				GETCAR_MODE = '1';
				STATE = ESCAPE_CAR_1;
			}
				
			else if (PARKING_MODE == '2') {
				GETCAR_MODE = '2';
				STATE = ESCAPE_CAR__1;
			}
			NEXT_STATE = STATE;
		}
		else {
			PARKING_MODE = bt;
			GETCAR_MODE = '0';
			STATE = WALL_ALIGNMENT;
			NEXT_STATE = STATE;
		}
	}
	if (d[1] > 0 && d[0] > 0 && d[2] > 0 && d[3] > 0 && (PARKING_MODE == '1' || PARKING_MODE == '2'))
	//if (d[1] > 0 && d[0] > 0 && d[2] > 0 && d[3] > 0 && (PARKING_MODE == '1' && PARKING_MODE == '2'))
	{
		//Serial.println(PARKING_MODE);
		//analogWrite(A5, 128);
		//stepper_controller(1);

		if (millis() - cTime >= 100) {
			
			newPosition = encoder0Pos;
			dF = d[1];
			dR = d[2];
			dTop = d[0];
			dBack = d[3];
			dMid = d[4];

			switch (STATE) {
			case WALL_ALIGNMENT:
				carStatus = 1;
				//goForward();
				mode = 8;
				if (dF > 30) {
					NEXT_STATE = SCAN_SLOT;
					myservo.write(150);
					delay(50);
				}
				else {
					g_fisInput[0] = dR;
					g_fisInput[1] = dF;
					g_fisOutput[0] = 0;
					fis_evaluate();
					angle = map(g_fisOutput[0], -33.5, 33.5, 30, 150);
					myservo.write((int)angle);
					//Serial.print('2');
					//Serial.println(1, DEC);
					//Serial.println("STATE = WALL_ALIGNMENT");
					//Serial.print("phi = ");
					//Serial.println(g_fisOutput[0]);
					//Serial.print("angle = ");
					//Serial.println((int)angle);
				}
				break;
			case SCAN_SLOT:
				//Serial.println("STATE = SCAN SLOT");
				myservo.write(90);
				if (dR > 30)
				{
					gap = 0;
					NEXT_STATE = START_MEASURE;
					myservo.detach();
				}

				break;
			case START_MEASURE:
				//Serial.println("START_MEASURE");

				//myservo.detach();
				gap += (newPosition - oldPosition);
				if (PARKING_MODE == '1')
				{
					if (gap < 15 && dR < 30) {
						myservo.attach(5);
						NEXT_STATE = WALL_ALIGNMENT;
					}
					else if ((gap >= 15 && dR < 30) || gap >= 15) {
						NEXT_STATE = REVERSE_POINT;
						gap = 0;
					}
				}
				else if (PARKING_MODE == '2') {
					if (gap < 8 && dR < 30) {
						myservo.attach(5);
						NEXT_STATE = WALL_ALIGNMENT;
					}
					else if (gap >= 9) {
						if (dR < 30) {
							NEXT_STATE = REVERSE_POINT;
							mode = 3;
							gap = 0;
						}
					}
				}
				break;
			case REVERSE_POINT:
				gap += (newPosition - oldPosition);
				//Serial.println("REVERSE_POINT");
				if (PARKING_MODE == '1') {
					if (gap >= 4)
					{
						myservo.attach(5);
						mode = 4;
						gap = 0;
						NEXT_STATE = PARKING_1;
					}
				}
				else if (PARKING_MODE == '2') {
					if (gap >= 7 && dMid >= 30)
					{
						mode = 4;
						gap = 0;
						NEXT_STATE = PARKING__1;
					}
				}

				break;
				/* PARALLEL PARKING */
			case PARKING_1:
				
				//Serial.println("PARKING_1");
				myservo.write(0);
				mode = 2;
				if (dBack < 45)
					NEXT_STATE = PARKING_2;
				break;
			case PARKING_2:
				//Serial.println("PARKING_2");
				myservo.write(90);
				//goBackward();
				mode = 2;
				if (dBack < 35)
					NEXT_STATE = PARKING_3;
				break;
			case PARKING_3:
				//Serial.println("PARKING_3");
				myservo.write(180);
				//goBackward();
				mode = 2;
				if (dBack < 15)
					NEXT_STATE = PARKING_4;
				break;
			case PARKING_4:
				myservo.write(0);
				mode = 6;
				if (dTop <= 15 || dBack >= 30)
					NEXT_STATE = PARKING_5;
				if (dR >= 8 && dR <= 18 && abs(dR - dF) <= 1 && abs(dTop - dBack) <= 1)
					NEXT_STATE = FINISH;
				break;
			case PARKING_5:
				//Serial.println("PARKING_5");
				g_fisInput[0] = dR;
				g_fisInput[1] = dF;
				g_fisOutput[0] = 0;
				fis_evaluate();
				angle = map(g_fisOutput[0], -33.5, 33.5, 150, 30);
				myservo.write((int)angle);
				mode = 2;
				if (maxAlign >= 5 && abs(dTop - dBack) <= 1) {
					NEXT_STATE = FINISH;
					maxAlign = 0;
				}
				else if (dBack <= 10) {
					NEXT_STATE = PARKING_6;
					maxAlign++;
				}
				if (dR >= 8 && dR <= 18 && abs(dR - dF) <= 1 && abs(dTop - dBack) <= 1)
					NEXT_STATE = FINISH;
				break;
			case PARKING_6:
				//Serial.println("PARKING_6");
				g_fisInput[0] = dR;
				g_fisInput[1] = dF;
				g_fisOutput[0] = 0;
				fis_evaluate();
				angle = map(g_fisOutput[0], -33.5, 33.5, 30, 150);
				myservo.write((int)angle);
				mode = 6;
				if (maxAlign >= 5 && abs(dTop - dBack) <= 1) {
					NEXT_STATE = FINISH;
					maxAlign = 0;
				}
				else if (dTop <= 10) {
					NEXT_STATE = PARKING_5;
					maxAlign++;
				}
				if (dR >= 8 && dR <= 18 && abs(dR - dF) <= 1 && abs(dTop - dBack) <= 1)
					NEXT_STATE = FINISH;
				break;

				/* GARAGE PARKING */
			case PARKING__1:
				myservo.attach(5);
				//Serial.println("PARKING__1");
				myservo.write(180);
				delay(500);
				myservo.detach();
				delay(100);
				//if (dR > 60)
					NEXT_STATE = PARKING__2;
				break;
			case PARKING__2:
				//Serial.println("PARKING__2");
				gap += (newPosition - oldPosition);
				mode = 6;					//forward						
				if (gap >= 8) {
					gap = 0;
					NEXT_STATE = PARKING__3;
				}
				break;
			case PARKING__3:
				//Serial.println("PARKING__3");
				myservo.attach(5);
				myservo.write(90);
				//goBackward();
				mode = 4;					//stop
				if (dF > dR)
					NEXT_STATE = PARKING__4;
				break;
			case PARKING__4:
				//Serial.println("PARKING__4");
				myservo.write(0);
				delay(500);
				myservo.detach();
				delay(500);
				mode = 2;
				gap += (newPosition - oldPosition);
				if (gap>=7){
					gap = 0;
					NEXT_STATE = PARKING__5;
					}
				break;
			case PARKING__5:
				//Serial.println("PARKING__4");
				delay(500);
				myservo.attach(5);
				myservo.write(0);
				mode = 3;
				if (abs(dF - dR) <= 5 && abs(dMid - dR) <=2)
					NEXT_STATE = PARKING__6;
				break;
			case PARKING__6:
				//Serial.println("PARKING__5");
				myservo.write(90);
				mode = 2;
				if (dBack <= 15)
					NEXT_STATE = FINISH;
				break;

			case FINISH:
				//Serial.println("FINISH");
				//Serial.println(2, DEC);
				carStatus = 2;
				myservo.write(90);
				mode = 4;
				break;
			default:
				//Serial.println("NO CASE");
				break;
			}
			//Serial.print("gap = ");
			//Serial.println(gap);
			//Serial.print("d1 = ");
			//Serial.println(d[0]);
			//Serial.print("d2 = ");
			//Serial.println(d[1]);
			//Serial.print("d3 = ");
			//Serial.println(d[2]);
			//Serial.print("d4 = ");
			//Serial.println(d[3]);

			STATE = NEXT_STATE;

			oldPosition = newPosition;
			cTime = millis();
		}
	}
	if (d[1] > 0 && d[0] > 0 && d[2] > 0 && d[3] > 0 && (GETCAR_MODE == '1' || GETCAR_MODE == '2')) {
		if (millis() - cTime3 >= 100) {
			
			newPosition = encoder0Pos;
			dF = d[1];
			dR = d[2];
			dTop = d[0];
			dBack = d[3];
			switch (STATE) {
			case ESCAPE_CAR_1:
				mode = 2;
				
				if (dBack <= 8) {
					NEXT_STATE = ESCAPE_CAR_2;
				}				
			break;
			case ESCAPE_CAR_2:
				mode = 5;
				myservo.write(180);
				if (dTop >= 70 || dTop <= 10)
					NEXT_STATE = ESCAPE_CAR_3;
				break;
			case ESCAPE_CAR_3:
				mode = 2;
				myservo.write(0);
				if (dBack <= 15)
					NEXT_STATE = ESCAPE_CAR_4;
				break;
			case ESCAPE_CAR_4:
				myservo.write(110);
				mode = 6;
				if (dBack >= 35 && dF >= 35)
					NEXT_STATE = ESCAPE_CAR_5;
				break;
			case ESCAPE_CAR_5:
				myservo.write(0);
				if (dR <= 25 && dF <= 25)
					NEXT_STATE = ESCAPE_CAR_6;
				else if (abs(dR - dF) <= 1 && dBack >= 90)
					NEXT_STATE = FINISH;
				
				break;
			case ESCAPE_CAR_6:
				if (!myservo.attached())
					myservo.attach(5);
				g_fisInput[0] = dR;
				g_fisInput[1] = dF;
				g_fisOutput[0] = 0;
				fis_evaluate();
				angle = map(g_fisOutput[0], -33.5, 33.5, 30, 150);
				myservo.write((int)angle);
				if (abs(dF - dR) <= 1 && dBack >= 90)
					NEXT_STATE = FINISH;
				break;




				case ESCAPE_CAR__1:
					//Serial.println("ESCAPE_CAR__1");
					mode = 8;
					if (dBack >= 25)
						NEXT_STATE = ESCAPE_CAR__2;
				break;
				case ESCAPE_CAR__2:
					//Serial.println("ESCAPE_CAR__2");
					if (!myservo.attached())
						myservo.attach(5);
					myservo.write(0);
					if (dF <= 15 && dBack >= 80) 
						NEXT_STATE = ESCAPE_CAR__3;
				break;
				case ESCAPE_CAR__3:
					//Serial.println("ESCAPE_CAR__3");
					g_fisInput[0] = dR;
					g_fisInput[1] = dF;
					g_fisOutput[0] = 0;
					fis_evaluate();
					angle = map(g_fisOutput[0], -33.5, 33.5, 30, 150);
					myservo.write((int)angle);
					if (abs(dF-dR) <=1)
						NEXT_STATE = FINISH;
					break;
				case FINISH:
					//Serial.println("FINISH");
					//Serial.println(2, DEC);
					carStatus = 2;
					myservo.write(90);
					mode = 4;
					break;
				default:
					//Serial.println("NO CASE");
					break;
			}
			STATE = NEXT_STATE;
			oldPosition = newPosition;
			cTime3 = millis();
		}

	}
}


void stop()
{
	stepper_controller(0);
}


void goForward()
{
	stepper_controller(4);
}

void goBackward()
{
	stepper_controller(-4);
}

void doEncoder()
{
	encoder0Pos++;
	//Serial.println(encoder0Pos);
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

//void stepper_controller(int _stepper_mode)
//{
//	static unsigned long lastTime = 0;
//	static int tick = 0;
//	if (micros() - lastTime >= 100)
//	{
//		lastTime = micros();
//		if (_stepper_mode == _stop)
//		{
//			digitalWrite(6, LOW);
//		}
//		else
//		{
//			if (_stepper_mode == forward) digitalWrite(8, LOW);
//			if (_stepper_mode == backward) digitalWrite(8, HIGH);
//			if (tick == 0)
//			{
//				tick = 1;
//				digitalWrite(6, LOW);
//			}
//			else
//			{
//				tick = 0;
//				digitalWrite(6, HIGH);
//			}
//		}
//	}
//
//}

void stepper_controller(float vel)  // vel (revolutions / second)
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
		digitalWrite(8, LOW);
		vel = vel * (-1);
		OCR1A = (unsigned int)625 / vel;
	}
	else if (vel == 0)
	{
		OCR1A = 0;
	}
	else if (vel > 0)
	{
		digitalWrite(8, HIGH);
		OCR1A = (unsigned int)625 / vel;
	}

}

uchar8 tim_min(uchar8 in[], uchar8 n)
{
	uchar8 i = 0;
	for (uchar8 j = 1;j < n; j++)
		if (in[j] < in[i]) i = j;
	return i;
}




uchar8* set_Distance_Flag(uchar8 di[8], uchar8 d) {
	uchar8 * re;
	for (int i = 0; i<sizeof(di); i++)
	{
		if (di[i] > d)
			re[i] = 0;
		else
			re[i] = 1;
	}
	return re;
}

bool checkOBS(uchar8 di[8]) {
	bool r = 0;
	for (int i = 0; i<sizeof(di); i++)
	{
		if (di[i] == 1) {
			r = 1;
			break;
		}
	}
	return r;
}
/* Interrupt Vector for OS */

ISR(TIMER2_COMPA_vect)
{
	OS_control();
}











/* Fuzzy */
//***********************************************************************
// Support functions for Fuzzy Inference System                          
//***********************************************************************
// Trapezoidal Member Function
FIS_TYPE fis_trapmf(FIS_TYPE x, FIS_TYPE* p)
{
	FIS_TYPE a = p[0], b = p[1], c = p[2], d = p[3];
	FIS_TYPE t1 = ((x <= c) ? 1 : ((d < x) ? 0 : ((c != d) ? ((d - x) / (d - c)) : 0)));
	FIS_TYPE t2 = ((b <= x) ? 1 : ((x < a) ? 0 : ((a != b) ? ((x - a) / (b - a)) : 0)));
	return (FIS_TYPE)min(t1, t2);
}

// Triangular Member Function
FIS_TYPE fis_trimf(FIS_TYPE x, FIS_TYPE* p)
{
	FIS_TYPE a = p[0], b = p[1], c = p[2];
	FIS_TYPE t1 = (x - a) / (b - a);
	FIS_TYPE t2 = (c - x) / (c - b);
	if ((a == b) && (b == c)) return (FIS_TYPE)(x == a);
	if (a == b) return (FIS_TYPE)(t2*(b <= x)*(x <= c));
	if (b == c) return (FIS_TYPE)(t1*(a <= x)*(x <= b));
	t1 = min(t1, t2);
	return (FIS_TYPE)max(t1, 0);
}

FIS_TYPE fis_min(FIS_TYPE a, FIS_TYPE b)
{
	return min(a, b);
}

FIS_TYPE fis_max(FIS_TYPE a, FIS_TYPE b)
{
	return max(a, b);
}

FIS_TYPE fis_array_operation(FIS_TYPE *array, int size, _FIS_ARR_OP pfnOp)
{
	int i;
	FIS_TYPE ret = 0;

	if (size == 0) return ret;
	if (size == 1) return array[0];

	ret = array[0];
	for (i = 1; i < size; i++)
	{
		ret = (*pfnOp)(ret, array[i]);
	}

	return ret;
}


//***********************************************************************
// Data for Fuzzy Inference System                                       
//***********************************************************************
// Pointers to the implementations of member functions
_FIS_MF fis_gMF[] =
{
	fis_trapmf, fis_trimf
};

// Count of member function for each Input
int fis_gIMFCount[] = { 3, 3 };

// Count of member function for each Output 
int fis_gOMFCount[] = { 5 };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFI0Coeff1[] = { 0, 0, 6.5, 9.5 };
FIS_TYPE fis_gMFI0Coeff2[] = { 6.5, 9.5, 12.5, 17.5 };
FIS_TYPE fis_gMFI0Coeff3[] = { 12.5, 14.5, 28, 28 };
FIS_TYPE* fis_gMFI0Coeff[] = { fis_gMFI0Coeff1, fis_gMFI0Coeff2, fis_gMFI0Coeff3 };
FIS_TYPE fis_gMFI1Coeff1[] = { 0, 0, 6.5, 9.5 };
FIS_TYPE fis_gMFI1Coeff2[] = { 6.5, 9.5, 12.5, 17.5 };
FIS_TYPE fis_gMFI1Coeff3[] = { 12.5, 14.5, 28, 28 };
FIS_TYPE* fis_gMFI1Coeff[] = { fis_gMFI1Coeff1, fis_gMFI1Coeff2, fis_gMFI1Coeff3 };
FIS_TYPE** fis_gMFICoeff[] = { fis_gMFI0Coeff, fis_gMFI1Coeff };

// Coefficients for the Input Member Functions
FIS_TYPE fis_gMFO0Coeff1[] = { -45, -45, -30, -15 };
FIS_TYPE fis_gMFO0Coeff2[] = { -30, -15, 0 };
FIS_TYPE fis_gMFO0Coeff3[] = { -15, 0, 15 };
FIS_TYPE fis_gMFO0Coeff4[] = { 0, 15, 30 };
FIS_TYPE fis_gMFO0Coeff5[] = { 15, 30, 45, 45 };
FIS_TYPE* fis_gMFO0Coeff[] = { fis_gMFO0Coeff1, fis_gMFO0Coeff2, fis_gMFO0Coeff3, fis_gMFO0Coeff4, fis_gMFO0Coeff5 };
FIS_TYPE** fis_gMFOCoeff[] = { fis_gMFO0Coeff };

// Input membership function set
int fis_gMFI0[] = { 0, 0, 0 };
int fis_gMFI1[] = { 0, 0, 0 };
int* fis_gMFI[] = { fis_gMFI0, fis_gMFI1 };

// Output membership function set
int fis_gMFO0[] = { 0, 1, 1, 1, 0 };
int* fis_gMFO[] = { fis_gMFO0 };

// Rule Weights
FIS_TYPE fis_gRWeight[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Type
int fis_gRType[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// Rule Inputs
int fis_gRI0[] = { 1, 1 };
int fis_gRI1[] = { 1, 2 };
int fis_gRI2[] = { 1, 3 };
int fis_gRI3[] = { 2, 1 };
int fis_gRI4[] = { 2, 2 };
int fis_gRI5[] = { 2, 3 };
int fis_gRI6[] = { 3, 1 };
int fis_gRI7[] = { 3, 2 };
int fis_gRI8[] = { 3, 3 };
int* fis_gRI[] = { fis_gRI0, fis_gRI1, fis_gRI2, fis_gRI3, fis_gRI4, fis_gRI5, fis_gRI6, fis_gRI7, fis_gRI8 };

// Rule Outputs
int fis_gRO0[] = { 4 };
int fis_gRO1[] = { 2 };
int fis_gRO2[] = { 2 };
int fis_gRO3[] = { 4 };
int fis_gRO4[] = { 3 };
int fis_gRO5[] = { 2 };
int fis_gRO6[] = { 4 };
int fis_gRO7[] = { 4 };
int fis_gRO8[] = { 2 };
int* fis_gRO[] = { fis_gRO0, fis_gRO1, fis_gRO2, fis_gRO3, fis_gRO4, fis_gRO5, fis_gRO6, fis_gRO7, fis_gRO8 };

// Input range Min
FIS_TYPE fis_gIMin[] = { 0, 0 };

// Input range Max
FIS_TYPE fis_gIMax[] = { 28, 28 };

// Output range Min
FIS_TYPE fis_gOMin[] = { -45 };

// Output range Max
FIS_TYPE fis_gOMax[] = { 45 };

//***********************************************************************
// Data dependent support functions for Fuzzy Inference System                          
//***********************************************************************
FIS_TYPE fis_MF_out(FIS_TYPE** fuzzyRuleSet, FIS_TYPE x, int o)
{
	FIS_TYPE mfOut;
	int r;

	for (r = 0; r < fis_gcR; ++r)
	{
		int index = fis_gRO[r][o];
		if (index > 0)
		{
			index = index - 1;
			mfOut = (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
		}
		else if (index < 0)
		{
			index = -index - 1;
			mfOut = 1 - (fis_gMF[fis_gMFO[o][index]])(x, fis_gMFOCoeff[o][index]);
		}
		else
		{
			mfOut = 0;
		}

		fuzzyRuleSet[0][r] = fis_min(mfOut, fuzzyRuleSet[1][r]);
	}
	return fis_array_operation(fuzzyRuleSet[0], fis_gcR, fis_max);
}

FIS_TYPE fis_defuzz_centroid(FIS_TYPE** fuzzyRuleSet, int o)
{
	FIS_TYPE step = (fis_gOMax[o] - fis_gOMin[o]) / (FIS_RESOLUSION - 1);
	FIS_TYPE area = 0;
	FIS_TYPE momentum = 0;
	FIS_TYPE dist, slice;
	int i;

	// calculate the area under the curve formed by the MF outputs
	for (i = 0; i < FIS_RESOLUSION; ++i) {
		dist = fis_gOMin[o] + (step * i);
		slice = step * fis_MF_out(fuzzyRuleSet, dist, o);
		area += slice;
		momentum += slice*dist;
	}

	return ((area == 0) ? ((fis_gOMax[o] + fis_gOMin[o]) / 2) : (momentum / area));
}

//***********************************************************************
// Fuzzy Inference System                                                
//***********************************************************************
void fis_evaluate()
{
	FIS_TYPE fuzzyInput0[] = { 0, 0, 0 };
	FIS_TYPE fuzzyInput1[] = { 0, 0, 0 };
	FIS_TYPE* fuzzyInput[fis_gcI] = { fuzzyInput0, fuzzyInput1, };
	FIS_TYPE fuzzyOutput0[] = { 0, 0, 0, 0, 0 };
	FIS_TYPE* fuzzyOutput[fis_gcO] = { fuzzyOutput0, };
	FIS_TYPE fuzzyRules[fis_gcR] = { 0 };
	FIS_TYPE fuzzyFires[fis_gcR] = { 0 };
	FIS_TYPE* fuzzyRuleSet[] = { fuzzyRules, fuzzyFires };
	FIS_TYPE sW = 0;

	// Transforming input to fuzzy Input
	int i, j, r, o;
	for (i = 0; i < fis_gcI; ++i)
	{
		for (j = 0; j < fis_gIMFCount[i]; ++j)
		{
			fuzzyInput[i][j] =
				(fis_gMF[fis_gMFI[i][j]])(g_fisInput[i], fis_gMFICoeff[i][j]);
		}
	}

	int index = 0;
	for (r = 0; r < fis_gcR; ++r)
	{
		if (fis_gRType[r] == 1)
		{
			fuzzyFires[r] = FIS_MAX;
			for (i = 0; i < fis_gcI; ++i)
			{
				index = fis_gRI[r][i];
				if (index > 0)
					fuzzyFires[r] = fis_min(fuzzyFires[r], fuzzyInput[i][index - 1]);
				else if (index < 0)
					fuzzyFires[r] = fis_min(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
				else
					fuzzyFires[r] = fis_min(fuzzyFires[r], 1);
			}
		}
		else
		{
			fuzzyFires[r] = FIS_MIN;
			for (i = 0; i < fis_gcI; ++i)
			{
				index = fis_gRI[r][i];
				if (index > 0)
					fuzzyFires[r] = fis_max(fuzzyFires[r], fuzzyInput[i][index - 1]);
				else if (index < 0)
					fuzzyFires[r] = fis_max(fuzzyFires[r], 1 - fuzzyInput[i][-index - 1]);
				else
					fuzzyFires[r] = fis_max(fuzzyFires[r], 0);
			}
		}

		fuzzyFires[r] = fis_gRWeight[r] * fuzzyFires[r];
		sW += fuzzyFires[r];
	}

	if (sW == 0)
	{
		for (o = 0; o < fis_gcO; ++o)
		{
			g_fisOutput[o] = ((fis_gOMax[o] + fis_gOMin[o]) / 2);
		}
	}
	else
	{
		for (o = 0; o < fis_gcO; ++o)
		{
			g_fisOutput[o] = fis_defuzz_centroid(fuzzyRuleSet, o);
		}
	}
}