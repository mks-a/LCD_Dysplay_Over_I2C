#include "timer1.h"

void timer1_init(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x05;
	
	// Clear pending interrupts
	TIFR1 |= 0x01;			// Timer / Counter1 Interrupt flag register
	
	// Set timer/counter
	TCNT1 = 0xFFFF;
	
	TIMSK1 = 0x01;			// overflow interrupt enable
}