#ifndef TWI_H
#define TWI_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
// #include <stdlib.h>
// #include <util/delay.h>

extern void TWI_init();
extern void TWI_send_data(uint8_t data, uint8_t addr);

#endif
