// put LED on PC5
#include <avr/io.h>       	// this is always included in AVR programs
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../twi/twi.h"
#include "../timer1/timer1.h"
#include "../pcf8574_display/pcf8574_display.h"

//#define debug

#ifdef debug
#include "../usart/usart.h"
#endif

int main(void) 
{	
	uint8_t err;
	uint8_t buf_len = 32;
	char tmp_data[buf_len];
	uint8_t recieved_len;
	
	init();
	
	while (1) 
	{
		_delay_ms(200);		// wait while all 32 possible characters will be recieved
		if(usart_is_buffer_not_empty())
		{
			pcf8574_display_clear();
			pcf8574_display_return();
			recieved_len = usart_recieve(&tmp_data, buf_len);
			pcf8574_display_write_string(&tmp_data, recieved_len);
		}
/*#ifdef debug
		usart_transmit_hex(err);	
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
	usart_init(1, 1);
#endif

	//timer1_init();
	
	// initialize display with 2 lines and 16 columns
	pcf8574_display_init(16, 0, 2);
	
	sei();
	
/*#ifdef debug
		usart_transmit_hex(UCSR0A);	
		usart_transmit_hex(UCSR0B);	
		usart_transmit_hex(SREG);
#endif*/
}

ISR(TIMER1_OVF_vect)
{
	/* this ISR purpose is wake up MCU and indicate of it with LED */
	PORTB ^= 0x20;
}

ISR(BADISR_vect) 
{
}