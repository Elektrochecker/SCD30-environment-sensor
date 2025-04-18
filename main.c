#include "config.h"
#define CPU_CLOCK_FREQUENCY F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "sensor.h"
#include "twi.h"
#include "uart.h"

// CURRENT CONNECTIVITY
// PD4 -> TWI error LED
// PC4 -> TWI SDA
// PC5 -> TWI SCL

int main(void) {

  TWI_init();
  UART_init();

  _delay_ms(100);

  SENSOR_init();

  while (1) {
    UART_send_string("\n\r\n\r");
    UART_send_string("requesting sensor data...\n\r");
    SENSOR_read_data();

    _delay_ms(2000);
  }

  return 0;
}
