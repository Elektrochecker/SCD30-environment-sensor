#ifndef UART_H
#define UART_H

#include "config.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// UBRR = UART baud rate register
// UBRR =  F_CPU / (baud rate * 16) - 1
// F = 1 MHz baud = 9600 => UBRR = 5.5
// F = 8 MHz baud = 9600 => UBRR = 51
// F = 16 MHz baud = 9600 => UBRR = 103
// F = 16 MHz baud = 115200 => UBRR = 8
// F = 20 MHz baud = 9600 => UBRR = 129

// #define BAUD 115200L
// #define temp_UBRR (F_CPU / (BAUD * 16) - 1)
#define UART_UBRR 8
#define UART_STRING_BUFFER_SIZE 12

extern void UART_init();
extern void UART_send_char(uint8_t data);
extern void UART_send_string(char *s);
extern void UART_send_string_P(const char *s);
extern void UART_println(char *s);
extern void UART_send_number(int16_t n);
extern void UART_send_number_hex(uint16_t n);
extern void UART_send_number_hex_32(uint32_t n);
extern void UART_send_float(float f, uint8_t width, uint8_t precision);
extern void UART_erase_line();

#endif
