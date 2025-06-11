#include "sensor.h"

const PROGMEM uint8_t trigger_continuous_measurement[5] = {0x00, 0x10, 0x00, 0x00, 0x81};
const PROGMEM uint8_t softreset[2] = {0xd3, 0x04};
const PROGMEM uint8_t read_data_avail[2] = {0x02, 0x02};
const PROGMEM uint8_t read_measurement[2] = {0x03, 0x00};

SENSOR_reading SENSOR_last_reading;

void SENSOR_init() {
  DDRB |= (0 << SENSOR_RDY_PIN);

  // soft reset the MCU of the sensor module
  TWI_send_data_P(softreset, 2, SENSOR_TWI_ADDR);

  // activate periodic measurements
  TWI_send_data_P(trigger_continuous_measurement, 5, SENSOR_TWI_ADDR);
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

uint8_t SENSOR_data_ready() { return PINB & (1 << SENSOR_RDY_PIN); }

SENSOR_reading SENSOR_read_data() {
  SENSOR_reading reading;

  if (!SENSOR_data_ready()) {
    // UART_println("sensor not ready, skipping reading data");
    reading.success = 0;
    return reading;
  }

  uint8_t result[18];

  TWI_send_data_P(read_measurement, 2, SENSOR_TWI_ADDR);
  _delay_ms(3);

  TWI_read_data(result, 18, SENSOR_TWI_ADDR);

  reading.co2concentration = SENSOR_data_to_float(result, 18, 0);
  reading.temperature = SENSOR_data_to_float(result, 18, 6);
  reading.humidity = SENSOR_data_to_float(result, 18, 12);
  reading.success = 1;

  return reading;
}
