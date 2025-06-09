#ifndef SENSOR_H
#define SENSOR_H

#include "config.h"
#include "twi.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define SENSOR_RDY_PIN PB0
#define SENSOR_TWI_ADDR 0x61

typedef struct {
  float temperature;
  float humidity;
  float co2concentration;
  uint8_t success;
} SENSOR_reading;

typedef struct {
  float temperature;
  float humidity;
  float co2concentration;
  uint32_t timestamp;
} SENSOR_datapoint;

extern SENSOR_reading SENSOR_last_reading;

extern void SENSOR_init();
extern SENSOR_reading SENSOR_read_data();

#endif
