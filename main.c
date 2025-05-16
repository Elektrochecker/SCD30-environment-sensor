#include "config.h"
#define CPU_CLOCK_FREQUENCY F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "clock.h"
#include "display.h"
#include "sensor.h"
#include "spi.h"
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
// PD2 .. PD5 user buttons (4)
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

  DDRD |= (1 << PD7);

  // set date & time
#ifdef DO_SET_DATETIME
  DATETIME time = {.seconds = 0, .minutes = 7, .hours = 13, .weekday = 6, .day = 10, .month = 5, .year = 2025};
  CLOCK_write_time(time);
#endif

  while (1) {

    DISPLAY_frame_buffer_set(16, 16, 1);
    DISPLAY_show_frame();

    UART_send_string("\n\r\n\r");
    UART_send_string("requesting sensor data...\n\r");
    SENSOR_read_data();

    DATETIME time = CLOCK_read_time();
    char time_str[9] = {0};
    char date_str[15] = {0};
    CLOCK_tostring(time, time_str, 9);
    CLOCK_date_tostring(time, date_str, 15);

    UART_send_string(time_str);
    UART_send_string("\n\r");
    UART_send_string(date_str);
    UART_send_string("\n\r");

    _delay_ms(1000);

    PORTD ^= (1 << PD7);
  }

  return 0;
}
