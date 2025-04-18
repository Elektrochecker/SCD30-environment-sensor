#include "uart.h"

char uart_string_buffer[UART_STRING_BUFFER_SIZE];

void UART_init() {
  // UART baud rate
  UBRR0H = (uint8_t)(temp_UBRR >> 8); // baud rate upper byte
  UBRR0L = (uint8_t)temp_UBRR;        // baud rate lower byte

  // enable RX and TX
  UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
  // set the frame format to 8 data bits and 1 stop bit (8N1)
  UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_send_char(uint8_t data) {
  // Wait for empty transmit buffer
  while (!(UCSR0A & (1 << UDRE0))) {
  };

  // Put data into buffer, sends the data
  UDR0 = data;
}

void UART_send_string(char *s) {
  while (*s) {
    UART_send_char(*s);
    s++;
  }
}

void UART_println(char *s) {
  UART_send_string(s);
  UART_send_string("\n\r");
}

void UART_send_number(int16_t n) {
  itoa(n, uart_string_buffer, 10);
  UART_send_string(uart_string_buffer);
}

void UART_send_number_hex(int16_t n) {
  itoa(n, uart_string_buffer, 16);
  UART_send_string("0x");
  UART_send_string(uart_string_buffer);
}

void UART_send_float(float f, uint8_t width, uint8_t precision) {
  dtostrf(f, width, precision, uart_string_buffer);
  UART_send_string(uart_string_buffer);
}

void UART_erase_line() {
  // VT100 escape code + CR
  // erases the current line and returns to start of line
  UART_send_string("\33[2K\r");
}
