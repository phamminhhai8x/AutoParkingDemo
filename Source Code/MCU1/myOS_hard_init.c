// 
// 
// 

#include "myOS_hard_init.h"

void hardware_init(){
	IO_init();
	timer_init();
}

void variable_init(){

}


void IO_init(){
	//DDRD |= _BV(2); 
	//DDRD |= _BV(4); 
 //   DDRD &= ~_BV(3); 
	//DDRD &= ~_BV(5); 
	pinMode(8, OUTPUT);
	pinMode(A5, INPUT);
	pinMode(A4, INPUT);
	pinMode(0, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(9, OUTPUT);
	pinMode(7, OUTPUT);

}

void timer_init(){
	
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  
  OCR2A = 249;
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 64 prescaler
  TCCR2B |= (1 << CS22);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}
