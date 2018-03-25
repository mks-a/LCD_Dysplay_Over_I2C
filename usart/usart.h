#ifndef usrat_h
#define usrat_h

#include <avr/io.h> 
#include <inttypes.h>
#include <avr/interrupt.h>
#define BAUD 9600
#include <util/setbaud.h>

#define BUFFER_MAX_SIZE 32

void usart_init(uint8_t _tx_en, uint8_t _rx_en);
void usart_transmit(uint8_t data);
uint8_t usart_recieve(char* _buffer, uint8_t _len);
static uint8_t num2hex(uint8_t num);
static void usart_enable_recieve_irq(void);
uint8_t usart_is_buffer_not_empty(void);
static void clear_buffer(void);
#endif