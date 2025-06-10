#include "clock.h"

void CLOCK_init() {
  uint8_t buf[2] = {0};

  // set 24h format
  // 24h format is the default, and overwriting the hours register deletes the hours every reset
  // buf[0] = 0x02; // write address
  // buf[1] = 0x00; // write data
  // TWI_send_data(buf, 2, CLOCK_TWI_ADDR);

  // configure trickle charger
  // one diode, 250 ohm resistor
  buf[0] = 0x10; // write address
  buf[1] = 0xa9; // write data
  TWI_send_data(buf, 2, CLOCK_TWI_ADDR);
}

DATETIME CLOCK_read_time() {
  uint8_t read_buf[7];
  DATETIME result;

  // set read address to 0x00
  TWI_send_byte(0x00, CLOCK_TWI_ADDR);

  // read time data
  TWI_read_data(read_buf, 7, CLOCK_TWI_ADDR);

  result.seconds = (read_buf[0] & 0x0f) + 10 * (read_buf[0] >> 4);
  result.minutes = (read_buf[1] & 0x0f) + 10 * (read_buf[1] >> 4);
  result.hours = (read_buf[2] & 0x0f) + 10 * ((read_buf[2] >> 4) & 0x03);

  result.weekday = read_buf[3];
  result.day = (read_buf[4] & 0x0f) + 10 * (read_buf[4] >> 4);
  result.month = (read_buf[5] & 0x0f) + 10 * ((read_buf[5] >> 4) & 0x07);
  result.year = ((read_buf[6] & 0x0f) + 10 * (read_buf[6] >> 4) + 100 * (read_buf[5] >> 7)) + 1900;

  return result;
}

void CLOCK_write_time(DATETIME time) {
  uint8_t buf[8] = {0};
  // the zero-th byte of the buffer is used to set the write address

  buf[1] = ((time.seconds % 10) & 0x0f) | ((time.seconds / 10) << 4);
  buf[2] = ((time.minutes % 10) & 0x0f) | ((time.minutes / 10) << 4);
  buf[3] = ((((time.hours % 10) & 0x0f) | ((time.hours / 10) << 4)) & 0x3f);
  buf[4] = time.weekday & 0x07;
  buf[5] = ((time.day % 10) & 0x0f) | ((time.day / 10) << 4);
  buf[6] = ((time.month % 10) & 0x0f) | (((time.month / 10) << 4) & 0x10) | ((((time.year - 1800) / 100) % 2) << 7);
  buf[7] = ((time.year % 10) & 0x0f) | (((time.year - 1900) / 10) << 4);

  // set read address to 0x00 and
  // write time data
  TWI_send_data(buf, 8, CLOCK_TWI_ADDR);
}

void CLOCK_tostring(DATETIME time, char *result, uint8_t len) {
  // parse the time to string and store it in result.
  // if len is 9 the format will be hh:mm:ss
  // if len is 6 the format will be hh:mm
  // buffer for result must be at least of size len

  if (len != 6 && len != 9) {
    return;
  }

  char sec[3];
  char min[3];
  char hou[3];
  itoa(time.seconds, sec, 10);
  itoa(time.minutes, min, 10);
  itoa(time.hours, hou, 10);

  if (strlen(sec) < 2) {
    sec[1] = sec[0];
    sec[0] = '0';
  }

  if (strlen(min) < 2) {
    min[1] = min[0];
    min[0] = '0';
  }

  if (strlen(hou) < 2) {
    hou[1] = hou[0];
    hou[0] = '0';
  }

  sec[2] = 0;
  min[2] = 0;
  hou[2] = 0;

  if (len == 9) {
    snprintf(result, len, "%s:%s:%s", hou, min, sec);
  } else if (len == 6) {
    snprintf(result, len, "%s:%s", hou, min);
  }
}

void CLOCK_date_tostring(DATETIME time, char *result, uint8_t len) {
  // len must be at least 15
  // buffer for result must be at least of size len
  switch (time.weekday) {
  case 1:
    strncpy(result, "mon ", 5);
    break;
  case 2:
    strncpy(result, "tue ", 5);
    break;
  case 3:
    strncpy(result, "wed ", 5);
    break;
  case 4:
    strncpy(result, "thu ", 5);
    break;
  case 5:
    strncpy(result, "fri ", 5);
    break;
  case 6:
    strncpy(result, "sat ", 5);
    break;
  case 7:
    strncpy(result, "sun ", 5);
    break;
  default:
    strncpy(result, "ERR ", 5);
    break;
  }

  snprintf(result + 4, len - 4, "%d.%d.%d", time.day, time.month, time.year);
}

DATETIME_reduced CLOCK_datetime_to_reduced(DATETIME time) {
  // 6 bits minutes, 5b hours, 5b days, 4b month, 7b year, 5b spare
  // loses precision down to 1min and information about the weekday
  DATETIME_reduced result;

  result.data = (time.minutes & 0x3f) | ((time.hours & 0x1f) << 6) | ((time.day & 0x1f) << 11) | (((uint32_t)time.month & 0x0f) << 16) |
                (((uint32_t)(time.year) & 0x7f) << 20);

  return result;
}

DATETIME CLOCK_datetime_from_reduced(DATETIME_reduced time) {
  DATETIME result = {
      .seconds = 0,
      .minutes = time.data & 0x3f,
      .hours = (time.data >> 6) & 0x1f,
      .day = (time.data >> 11) & 0x1f,
      .month = (time.data >> 16) & 0x0f,
      .year = ((time.data >> 20) & 0x7f),
  };

  return result;
}
