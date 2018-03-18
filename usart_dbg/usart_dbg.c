#include "usart_dbg.h"

void usart_dbg_init(void)
{	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	UCSR0B |= (1<<TXEN0);						// enable usart transmitter
}

void usart_dbg_transmit(uint8_t data)
{
	while(!(UCSR0A & (1 << UDRE0)));		// loop until USART data register empty bit in USART Control and Statis register 0A not set
	
	UDR0 = data;							// when UDRE0 bit is set put new data to data register
}

// NOTE: Atmega328 is Little endian, so be aware of this
void usart_dbg_transmit_hex(uint8_t data)
{
	uint8_t tmp;
	
	usart_dbg_transmit(0x30);				// ASCII value for 0
	usart_dbg_transmit(0x78);				// ASCII value for x
	
	// transmit upper half of the byte
	tmp = data >> 4;
	tmp = num2hex(tmp);
	usart_dbg_transmit(tmp);

	// transmit lower half of the byte
	tmp = data & 0x0F;
	tmp = num2hex(tmp);
	usart_dbg_transmit(tmp);
	
	usart_dbg_transmit(0x20);				// ASCII value for space
}

static uint8_t num2hex(uint8_t num)
{
	uint8_t tmp;
	
	// set initial value of return character	
	switch(num < 10)
	{
		case 0:
			tmp = 0x37;
			break;
		case 1:
			tmp = 0x30;
			break;
	}
	
	return tmp + num;
}