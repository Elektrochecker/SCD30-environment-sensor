#ifndef SPI_H
#define SPI_H

#include "config.h"

#include <avr/io.h>

extern void SPI_init();
extern void SPI_transmit(char data);

#endif
