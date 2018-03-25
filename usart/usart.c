#include "usart.h"

static uint8_t recieved_data[BUFFER_MAX_SIZE];
static volatile uint8_t buffer_counter;

// Initialize USART for using
// _tx_en - Indicate that USART data transmittion is enabled
//		0 - transmission not enabled
//		1 - transmission enabled
// _rx_en - Indicate that USART data recieving is enabled
//		0 - recieving not enabled
//		1 - recieving enabled
void usart_init(uint8_t _tx_en, uint8_t _rx_en)
{
	uint8_t uscr0b_status = 0;
	uint8_t ddrd_status = 0;
	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	if(_tx_en)
	{
		ddrd_status = 0x02;					// set DDRD pin1 as output (transmit pin)
		uscr0b_status |= (1<<TXEN0);
	}
	
	if(_rx_en)
	{
		ddrd_status &= 0xFE;				// set DDRD pon0 as input (recieve pin)
		uscr0b_status |= (1<<RXEN0);
		usart_enable_recieve_irq();
	}
	
	DDRD = ddrd_status;
	UCSR0B |= uscr0b_status;						// enable usart transmitter
	
	clear_buffer();
}

// Transmit data to USART bus
// data - data to transmit
void usart_transmit(uint8_t data)
{
	while(!(UCSR0A & (1 << UDRE0)));		// loop until USART data register empty bit in USART Control and Statis register 0A not set
	
	UDR0 = data;							// when UDRE0 bit is set put new data to data register
}

// Recieve data from USART bus
// _buffer - buffer where data will be writen
// _len - length of buffer
// returns length in byte of recieved data
uint8_t usart_recieve(char* _buffer, uint8_t _len)
{
	uint8_t i;
	uint8_t max_pos = _len > BUFFER_MAX_SIZE - 1 ? BUFFER_MAX_SIZE - 1 : _len;
	uint8_t str_len = buffer_counter > max_pos ? max_pos : buffer_counter;
	
	for(i = 0; i <= str_len; i++)
	{
		_buffer[i] = recieved_data[i];
	}
	
	clear_buffer();
	buffer_counter = 0;
	
	return i - 1;
}

// Check for USART recieved buffer
// returns 0 if buffer is empty, 1 otherwise
uint8_t usart_is_buffer_not_empty(void)
{
	return recieved_data[0] == 0 ? 0 : 1;
}

// Transmit byte in hexadecimal format
// NOTE: Atmega328 is Little endian, so be aware of this
void usart_transmit_hex(uint8_t data)
{
	uint8_t tmp;
	
	usart_transmit(0x30);				// ASCII value for 0
	usart_transmit(0x78);				// ASCII value for x
	
	// transmit upper half of the byte
	tmp = data >> 4;
	tmp = num2hex(tmp);
	usart_transmit(tmp);

	// transmit lower half of the byte
	tmp = data & 0x0F;
	tmp = num2hex(tmp);
	usart_transmit(tmp);
	
	usart_transmit(0x20);				// ASCII value for space
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

// Enables USART recieve interrupt
static void usart_enable_recieve_irq(void)
{
	UCSR0B |= (1<<RXCIE0);
}

static void clear_buffer(void)
{
	uint8_t i;
	
	for(i = 0; i < BUFFER_MAX_SIZE; i++)
		recieved_data[i] = 0;
	
	buffer_counter = 0;
}

ISR(USART_RX_vect)
{
	if(buffer_counter < BUFFER_MAX_SIZE - 1)
	{
		recieved_data[buffer_counter] = UDR0;
		buffer_counter++;
	}
}