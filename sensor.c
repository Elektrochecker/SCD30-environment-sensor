#include "sensor.h"
#include <string.h>

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

float SENSOR_data_to_float(uint8_t *buffer, uint8_t buffer_size, uint8_t offset) {
  // TODO: check CRCs
  uint8_t tmp[4];

  tmp[3] = buffer[0 + offset];
  tmp[2] = buffer[1 + offset];
  // CRC 1
  tmp[1] = buffer[3 + offset];
  tmp[0] = buffer[4 + offset];
  // CRC 2

  return *(float *)tmp;
}

uint8_t SENSOR_data_ready() { return PIND & (1 << SENSOR_RDY_PIN); }

void SENSOR_read_data() {
  if (!SENSOR_data_ready()) {
    UART_println("sensor not ready, skipping reading data");
    return;
  }

  uint8_t result[18];

  TWI_send_data(read_measurement, 2, SENSOR_TWI_ADDR);
  _delay_ms(3);

  TWI_read_data(result, 18, SENSOR_TWI_ADDR);

  // for (uint8_t i = 0; i < 18; i++) {
  //   UART_send_number_hex(result[i]);
  //   UART_send_string("\n\r");
  // }

  float co2concentration = SENSOR_data_to_float(result, 18, 0);
  float temperature = SENSOR_data_to_float(result, 18, 6);
  float humidity = SENSOR_data_to_float(result, 18, 12);

  UART_send_string("co2 concentration ");
  UART_send_float(co2concentration, 4, 0);
  UART_send_string(" ppm\n\r");

  UART_send_string("temperature       ");
  UART_send_float(temperature, 4, 1);
  UART_send_string(" C\n\r");

  UART_send_string("humidity          ");
  UART_send_float(humidity, 4, 0);
  UART_send_string(" %\n\r");
}
