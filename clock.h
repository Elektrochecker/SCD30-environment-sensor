#ifndef CLOCK_H
#define CLOCK_H

#include "config.h"
#include "twi.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

#define CLOCK_TWI_ADDR 0x68

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
} DATETIME;

extern void CLOCK_init();
extern DATETIME CLOCK_read_time();
extern void CLOCK_tostring(DATETIME time, char* result, uint8_t len);

#endif
