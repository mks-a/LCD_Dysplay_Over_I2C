#ifndef usrat_debug_h
#define usrat_debug_h

#include <avr/io.h> 
#include <inttypes.h>
#define BAUD 9600
#include <util/setbaud.h>

//extern void __init_usart(char high_value, char low_value);
void usart_dbg_init(void);
void usart_dbg_transmit(uint8_t data);
static uint8_t num2hex(uint8_t num);
#endif

