#include "uart.h"

void UART_init() {
    // UART
    UBRR0H = (uint8_t)(temp_UBRR >> 8); // baud rate oberes byte
    UBRR0L = (uint8_t)temp_UBRR;        // baud rate unteres byte

    // enable RX and TX
    UCSR0B = (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
    // set the frame format to 8 data bits and 1 stop bit (8N1)
    UCSR0C = (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_send_char(uint8_t data) {
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0))) {
    };

    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void UART_send_string(char *s) {
    while (*s) {
        UART_send_char(*s);
        s++;
    }
}

void UART_send_number(int16_t n) {
    char str[8];
    itoa(n, str, 10);
    UART_send_string(str);
}

void UART_erase_line() {
    // VT100 escape code + CR
    UART_send_string("\33[2K\r");
}
