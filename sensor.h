#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "twi.h"
#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>

#define SENSOR_RDY_PIN PD5
#define SENSOR_TWI_ADDR 0x61

extern void SENSOR_init();
extern void SENSOR_read_data();

#endif
