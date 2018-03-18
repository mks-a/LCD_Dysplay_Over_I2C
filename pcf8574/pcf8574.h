#ifndef pcf8574_h

#define pcf8574_h

#ifndef F_CPU          		// if F_CPU was not defined in Project -> Properties
#define F_CPU 16000000UL    // define it now as 16 MHz unsigned long
#endif

#include <util/delay.h>
#include <avr/io.h>
#include "../twi/twi.h"

#define PCF8574_TWCR_INIT 0x44			// init value from TWCR register
#define PCF8574_BITRATE_PRESCALER 0x48

void pcf8574_init(uint8_t _addr);
uint8_t pcf8574_write_addr(uint8_t* _status);
uint8_t pcf8574_write_data(uint8_t _data, uint8_t* _status);
uint8_t pcf8574_write_start_cnd(uint8_t* _status);

#endif