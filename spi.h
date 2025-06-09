#ifndef SPI_H
#define SPI_H

#include "config.h"

#include <avr/io.h>

extern void SPI_init();
extern void SPI_transmit(uint8_t data);
extern uint8_t SPI_receive();
extern void SPI_transmit_receive_n(uint8_t *transmit_buffer, uint8_t *receive_buffer, uint32_t size);
extern void SPI_transmit_n(uint8_t *data, uint32_t size);
extern void SPI_receive_n(uint8_t *receive_buffer, uint32_t size);

#endif
