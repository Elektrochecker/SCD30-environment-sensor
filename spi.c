#include "spi.h"

void SPI_init() {
  /* Set MOSI, SS and SCK output, all others input */
  DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
  PORTB |= (1 << PB2); // disable SS preemptiveley

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_transmit(char data) {
  /* Start transmission */
  SPDR = data;
  while (!(SPSR & (1 << SPIF))) {
    /* Wait for transmission complete */
  };
}
