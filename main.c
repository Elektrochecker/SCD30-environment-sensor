#include "config.h"
#define CPU_CLOCK_FREQUENCY F_CPU

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "twi.h"

int main(void) {

  UART_init();
  TWI_init();

  while (1) {
    UART_send_string("sending i2c data..\n\r");
    _delay_ms(500);

    for (uint8_t i = 0; i < 100; i++) {
      TWI_send_data(i, 0xd4);
      _delay_ms(100);
    }
  }

  return 0;
}
