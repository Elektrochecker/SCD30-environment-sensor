#include "config.h"
#define CPU_CLOCK_FREQUENCY F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "clock.h"
#include "display.h"
#include "scene.h"
#include "sensor.h"
#include "spi.h"
#include "storage.h"
#include "twi.h"
#include "uart.h"

// IO CONNECTIVITY

// PB0 -> sensor RDY
// PB1 -> CS display
// PB2 -> CS flash chip
// PB3 -> MOSI
// PB4 -> MISO
// PB5 -> CLK
// PC0 -> display reset
// PC1 -> display A0
// PC4 -> TWI SDA
// PC5 -> TWI SCL
// PD2 .. PD5 -> user buttons (4)
// PD6 -> TWI error LED (red)
// PD7 -> program LED (blue)

// uncomment to set the datetime when flashing.
// must flash a second time with this option turned off again
// #define DO_SET_DATETIME

int main(void) {

  SPI_init();
  TWI_init();
  UART_init();

  _delay_ms(100);

  SENSOR_init();
  CLOCK_init();
  DISPLAY_init();
  DISPLAY_clear();
  STORAGE_init();

  DDRD |= (1 << PD7);
  PORTD |= (1 << PD7);

  _delay_ms(500);

  STORAGE_print_debug_information();

  // set date & time
#ifdef DO_SET_DATETIME
  DATETIME time = {.seconds = 0, .minutes = 32, .hours = 12, .weekday = 1, .day = 9, .month = 6, .year = 2025};
  CLOCK_write_time(time);
#endif

  DATETIME_reduced time_r = CLOCK_datetime_to_reduced(CLOCK_read_time());
  DATETIME time = CLOCK_datetime_from_reduced(time_r);
  UART_send_number_hex(time_r.data & 0xff);
  UART_send_number_hex((time_r.data >> 16) & 0xff);
  char str[15];
  CLOCK_tostring(time, str, 9);
  UART_send_string("\r\n");
  UART_send_string(str);
  CLOCK_date_tostring(time, str, 15);
  UART_send_string("\r\n");
  UART_send_string(str);

  while (1) {
    SENSOR_reading reading = SENSOR_read_data();
    if (reading.success) {
      SENSOR_last_reading = reading;
    }

    SCENE_display_current_scene();
    SCNENE_advance();

    _delay_ms(200);
  }

  return 0;
}
