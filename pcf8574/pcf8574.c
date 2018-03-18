#include "pcf8574.h"

static uint8_t addr;

// Initialize Atmega328 registers for using TWI with PCF8574 and set display's initial state
// _addr - address of i2c device
void pcf8574_init(uint8_t _addr)
{
	addr = _addr;
	twi_init(PCF8574_TWCR_INIT, PCF8574_BITRATE_PRESCALER);
}

// Write start condition to i2c bus
// _status - pointer to variable, where actual status of operation will be stored
// Return 0 on success, otherwise 1
uint8_t pcf8574_write_start_cnd(uint8_t* _status)
{	
	twi_send_start_condition();
	
	*_status = TWI_GET_STATUS();
	
	if(*_status != TWI_START_CONDITION && *_status != TWI_RSTART_CONDITION)
		return 1;
	
	return 0;
}

// Send device address to i2c bus
// _status - pointer to variable, where actual status of operation will be stored
// Return 0 on success, otherwise 1
uint8_t pcf8574_write_addr(uint8_t* _status)
{
	twi_send_byte(addr);

	*_status = TWI_GET_STATUS();
		
	if(*_status != TWI_MT_SLA_W_ACK)
		return 1;
	
	return 0;
}

// Send data to i2c bus
// _status - pointer to variable, where actual status of operation will be stored
// Return 0 on success, otherwise 1
uint8_t pcf8574_write_data(uint8_t _data, uint8_t* _status)
{
	twi_send_byte(_data);

	*_status = TWI_GET_STATUS();
		
	if(*_status != TWI_MT_DATA_W_ACK)
		return 1;
	
	return 0;
}