#include "sensor.h"
#include "uart.h"

uint8_t trigger_continuous_measurement[] = {0x00, 0x10, 0x00, 0x00, 0x81};
uint8_t softreset[] = {0xd3, 0x04};
uint8_t read_data_avail[] = {0x02, 0x02};
uint8_t read_measurement[] = {0x03, 0x00};

void SENSOR_init() {
  DDRD |= (0 << SENSOR_RDY_PIN);

  // soft reset the MCU of the sensor module
  UART_send_string("TWI: soft reset\n\r");
  TWI_send_data(softreset, 2, SENSOR_TWI_ADDR);
  // activate periodic measurements

  UART_send_string("TWI: trigger continuous measurement\n\r");
  TWI_send_data(trigger_continuous_measurement, 5, SENSOR_TWI_ADDR);
}

uint8_t SENSOR_data_ready() { return PIND & (1 << SENSOR_RDY_PIN); }

void SENSOR_read_data() {
  if (!SENSOR_data_ready()) {
    UART_println("sensor not ready, skipping reading data");
    return;
  }

  uint8_t result[18];

  UART_println("sending read address");
  TWI_send_data(read_measurement, 2, SENSOR_TWI_ADDR);
  _delay_ms(3);
  UART_println("trying to read data");
  TWI_read_data(result, 18, SENSOR_TWI_ADDR);

  for (uint8_t i = 0; i < 18; i++) {
    UART_send_number(result[i]);
    UART_send_string("\n\r");
  }
}
