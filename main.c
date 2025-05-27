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

  DDRD |= (1 << PD7);
  PORTD |= (1 << PD7);

  // set date & time
#ifdef DO_SET_DATETIME
  DATETIME time = {.seconds = 0, .minutes = 7, .hours = 13, .weekday = 6, .day = 10, .month = 5, .year = 2025};
  CLOCK_write_time(time);
#endif

  while (1) {

    DISPLAY_clear_frame_buffer();
    // DISPLAY_clear();

    SENSOR_reading reading = SENSOR_read_data();
    if (reading.success) {
      SENSOR_last_reading = reading;
    }

    DATETIME time = CLOCK_read_time();
    char time_str[9] = {0};
    char date_str[15] = {0};

    CLOCK_tostring(time, time_str, 9);
    CLOCK_date_tostring(time, date_str, 15);

    DISPLAY_write_to_framebuffer(date_str, 0, 0);
    DISPLAY_write_to_framebuffer(time_str, 0, 88);

    char sensor_str[6] = {0};
    dtostrf(SENSOR_last_reading.temperature, 4, 1, sensor_str);
    DISPLAY_write_to_framebuffer("temperature: ", 3, 0);
    DISPLAY_write_to_framebuffer(sensor_str, 3, 64);
    DISPLAY_write_to_framebuffer(" C", 3, 88);

    dtostrf(SENSOR_last_reading.humidity, 4, 1, sensor_str);
    DISPLAY_write_to_framebuffer("humidity:      ", 5, 0);
    DISPLAY_write_to_framebuffer(sensor_str, 5, 64);
    DISPLAY_write_to_framebuffer(" %", 5, 88);

    dtostrf(SENSOR_last_reading.co2concentration, 4, 0, sensor_str);
    DISPLAY_write_to_framebuffer("co2:           ", 7, 0);
    DISPLAY_write_to_framebuffer(sensor_str, 7, 64);
    DISPLAY_write_to_framebuffer(" ppm", 7, 88);

    DISPLAY_show_frame();
    _delay_ms(200);
  }

  return 0;
}
