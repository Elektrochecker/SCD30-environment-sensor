#ifndef CLOCK_H
#define CLOCK_H

#include "config.h"
#include "twi.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#define CLOCK_TWI_ADDR 0x68

typedef struct {
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t weekday;
  uint8_t day;
  uint8_t month;
  uint16_t year;
} DATETIME;

typedef struct {
  uint32_t data;
} DATETIME_reduced;

extern void CLOCK_init();
extern DATETIME CLOCK_read_time();
extern void CLOCK_write_time(DATETIME time);
extern void CLOCK_tostring(DATETIME time, char *result, uint8_t len);
extern void CLOCK_date_tostring(DATETIME time, char *result, uint8_t len);
extern DATETIME_reduced CLOCK_datetime_to_reduced(DATETIME time);
extern DATETIME CLOCK_datetime_from_reduced(DATETIME_reduced time);

#endif
