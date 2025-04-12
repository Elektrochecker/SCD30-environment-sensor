#ifndef UART_H
#define UART_H

#include "config.h"

#include <avr/io.h>
#include <stdlib.h>
// #include <util/delay.h>

// UBRR = UART baud rate register
// UBRR =  F_CPU / (baud rate * 16) - 1
// F = 1 MHz baud = 9600 => UBRR = 5.5
// F = 8 MHz baud = 9600 => UBRR = 51
// F = 16 MHz baud = 9600 => UBRR = 103
// F = 20 MHz baud = 9600 => UBRR = 129

#define BAUD 9600L
#define temp_UBRR (F_CPU / (BAUD * 16) - 1)
// #define temp_UBRR 103

extern void UART_init();
extern void UART_send_char(uint8_t data);
extern void UART_send_string(char *s);
extern void UART_send_number(int16_t n);
extern void UART_erase_line();

#endif
