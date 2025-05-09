#include "clock.h"

void CLOCK_init() {}

DATETIME CLOCK_read_time() {
  uint8_t read_buf[7];
  DATETIME result;

  // set read address to 0x00
  TWI_send_byte(0x00, CLOCK_TWI_ADDR);

  TWI_read_data(read_buf, 7, CLOCK_TWI_ADDR);

  result.seconds = (read_buf[0] & 0x0f) + 10 * (read_buf[0] >> 4);
  result.minutes = (read_buf[1] & 0x0f) + 10 * (read_buf[1] >> 4);

  return result;
}

void CLOCK_tostring(DATETIME time, char *result, uint8_t len) {
  // parse the time to string and store it in result.
  // if len is 9 the format will be hh:mm:ss
  // if len is 6 the format will be hh:mm
  // buffer for result must be at least of size len

  if(len != 6 && len != 9) {
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
