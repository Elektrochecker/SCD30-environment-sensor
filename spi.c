#include "spi.h"

void SPI_init() {
  /* Set MOSI, SS and SCK output, all others input */
  DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
  PORTB |= (1 << PB2); // disable SS preemptiveley

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_transmit(uint8_t data) {
  /* Start transmission */
  SPDR = data;
  while (!(SPSR & (1 << SPIF))) {
    /* Wait for transmission complete */
  };
}

uint8_t SPI_receive() {
  /* Start transmission */
  SPDR = 0xff;
  while (!(SPSR & (1 << SPIF))) {
    /* Wait for transmission complete */
  };

  return SPDR;
}

void SPI_transmit_n(uint8_t *data, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    SPI_transmit(data[i]);
  }
}

uint8_t SPI_transmit_receive(uint8_t data) {
  /* Start transmission */
  SPDR = data;
  while (!(SPSR & (1 << SPIF))) {
    /* Wait for transmission complete */
  };

  return SPDR;
}

void SPI_transmit_receive_n(uint8_t *transmit_buffer, uint8_t *receive_buffer, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    receive_buffer[i] = SPI_transmit_receive(transmit_buffer[i]);
  }
}

void SPI_receive_n(uint8_t *receive_buffer, uint32_t size) {
  for (uint32_t i = 0; i < size; i++) {
    receive_buffer[i] = SPI_transmit_receive(0xff);
  }
}
