#ifndef twi_h
#define twi_h

#ifndef F_CPU          		// if F_CPU was not defined in Project -> Properties
#define F_CPU 16000000UL    // define it now as 16 MHz unsigned long
#endif

#include <avr/io.h> 		// here Arduino registers are defined
#include <inttypes.h>

#define debug

#ifdef debug
#include "../usart_dbg/usart_dbg.h"
#endif

#define TWI_TWINT_CLEAR() TWCR = TWCR & 0x7F 				//clears the TWINT flag
#define TWI_CLEAR_START_CONDITION() TWCR &= 0xDF		// 1101 1111b
#define TWI_GET_STATUS() TWSR & 0xF8						// mask 2:0bits of WTSR
#define TWI_TWINT_CLEAR() TWCR & 0x7F						// ands TWCR value with 0111 1111b
#define TWI_SET_PULLUP_REGISTERS() PORTC = PINC | 0x30				// 0011 0000b 
#define TWI_CLEAR_TWSR_PRESCALER() TWSR = TWSR & 0xFC		// 1111 1100b
#define TWI_ACK_DISABLE() TWCR = TWCR & 0xBF				// 1011 1111b

// TWI Status codes
#define TWI_NO_STATE_INFO 0xF8								// no relevant state information
#define TWI_BUS_ERR 0x00									// bus error
#define TWI_START_CONDITION 0x08
#define TWI_RSTART_CONDITION 0x10							// repeated start
#define TWI_MT_SLA_W_ACK 0x18								// slave address write acknowledgment received
#define TWI_MT_SLA_W_NACK 0x20								// not acknowledgment received
#define TWI_MT_DATA_W_ACK 0x28
#define TWI_MT_DATA_W_NACK 0x30
#define TWI_M_ARB_LOST 0x38									// master arbitration lost
#define TWI_MR_SLA_R_ACK 0x40								// master received slave address read acknowledgment
#define TWI_MR_SLA_R_NACK 0x48								// master received slave address read not acknowledgment received
#define TWI_MR_DATA_R_ACK 0x50								// master received data received acknowledgment
#define TWI_MR_DATA_R_NACK 0x58								// not acknowledgment

#define TWINT_BIT 0x07
#define TWSTO_BIT 0x04

void twi_init(uint8_t _init_value, uint8_t _prescaler);
void twi_send_start_condition(void);
void twi_send_stop_condition(void);
static void twi_clear_start_condition(void);
void twi_send_byte(uint8_t _data);
uint8_t twi_read_data(void);

#endif