#include "twi.h"

static uint8_t twcr_init_value;
/* Initialize TWI Control and Data registers 
_init_value - init value from TWCR register
_prescaler - is a prescaler to set in TWBR register */
void twi_init(uint8_t _init_value, uint8_t _prescaler)
{
	// clear TWI data register
	TWDR = 0xFF;
	
	twcr_init_value = _init_value;
	// set initial value for WTI Control register
	TWCR = twcr_init_value;
	
	// Clear prescaler bit on TWI Status Register
	TWSR &= 0xFC; 	// 1111 1100b
	
	TWI_SET_PULLUP_REGISTERS();
	
	// Set a prescaler to TWBR register
	TWBR = _prescaler;
}

// send start condition to the device
void twi_send_start_condition(void)
{
	// set START condition bit
	TWCR = twcr_init_value | 0xA4;	// 1010 0100b (to generate START and RSTART conditions all 3 bits must be written simultaneously)
	
	while(!(TWCR & 1 << TWINT_BIT));	// waiting for TWINT bit will be set in TWCR register
}

void twi_send_stop_condition(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1<<TWSTO);
	
	while((TWCR & 1 << TWSTO_BIT));				// waiting for TWSTO bit will be cleared in TWCR register
}

// function sends 1 byte to the device
// _data - data to send
void twi_send_byte(uint8_t _data)
{
	TWDR = _data;					// set data to TWDR register

	TWI_CLEAR_START_CONDITION();
	
	while(!(TWCR & 1 << TWINT_BIT));	// waiting for TWINT bit will be set in TWCR register	
}

// function read byte from TWDR register and returns it
uint8_t twi_read_data(void)
{
	while(!(TWCR & 1 << TWINT_BIT));	// waiting for TWINT bit will be set in TWCR register
	
	return TWDR;
}

