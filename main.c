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
// PD6 -> TWI error / Flash storage write LED (red)
// PD7 -> program LED (blue)

// uncomment to set the datetime when flashing.
// must flash a second time with this option turned off again
// #define DO_SET_DATETIME

#define MAIN_LOOP_DELAY 200U
#define DATA_SAVE_INTERVAL 60U // seconds
#define DATA_SAVE_LOOP_COUNT DATA_SAVE_INTERVAL * 1000U / MAIN_LOOP_DELAY
uint16_t data_save_counter = 0;

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

  // STORAGE_print_debug_information();

  // STORAGE_block_erase_32k(0);

  // scan the flash chip for the last written datapoint
  STORAGE_current_location = STORAGE_scan_location();

  // dump current database on startup
  STORAGE_dump_datapoints_to_uart();

  // set date & time
#ifdef DO_SET_DATETIME
  DATETIME time = {.seconds = 0, .minutes = 32, .hours = 12, .weekday = 1, .day = 9, .month = 6, .year = 2025};
  CLOCK_write_time(time);
#endif

  while (1) {
    SENSOR_reading reading = SENSOR_read_data();
    if (reading.success) {
      SENSOR_last_reading = reading;

      if (data_save_counter >= DATA_SAVE_LOOP_COUNT) {
        STORAGE_save_datapoint(SENSOR_last_reading, CLOCK_read_time());

        data_save_counter = 0;
      }
    }

    SCENE_display_current_scene();
    SCNENE_advance();

    _delay_ms(MAIN_LOOP_DELAY);
    data_save_counter++;
  }

  return 0;
}
