// put LED on PC5
#include <avr/io.h>       	// this is always included in AVR programs
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../twi/twi.h"
#include "../timer1/timer1.h"
#include "../pcf8574_display/pcf8574_display.h"

//#define debug

#ifdef debug
#include "../usart_dbg/usart_dbg.h"
#endif

int main(void) 
{	
	uint8_t err;
	
	init();
	
	while (1) 
	{
		pcf8574_display_write_string("Hello world, I'm here!!!", 24);
/*#ifdef debug
		usart_dbg_transmit_hex(err);	
#endif*/
		
		sleep_enable();
		sleep_cpu();		
		sleep_disable();
	}
  
	return(0);          	// should never get here, this is to prevent a compiler warning
}

void init(void)
{	
	DDRB = 0x20;     		// set pin 13 as output, others as input

#ifdef debug	
	usart_dbg_init();
#endif

	//timer1_init();
	
	// initialize display with 2 lines and 16 columns
	pcf8574_display_init(16, 0, 2);
	
	sei();
}

ISR(TIMER1_OVF_vect)
{
	/* this ISR purpose is wake up MCU and indicate of it with LED */
	PORTB ^= 0x20;
}

ISR(BADISR_vect) 
{
}