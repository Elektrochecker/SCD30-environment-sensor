#include "storage.h"

void STORAGE_init() {
  DDRB |= (1 << STORAGE_CS_PIN);
  PORTB |= (1 << STORAGE_CS_PIN);

  void STORAGE_write_disable();
}

void STORAGE_write_enable() {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_WRITE_ENABLE);
  PORTB |= (1 << STORAGE_CS_PIN);
}

void STORAGE_write_disable() {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_WRITE_DISABLE);
  PORTB |= (1 << STORAGE_CS_PIN);
}

void STORAGE_write_data(uint32_t addr, uint8_t *write_buffer, uint32_t size) {
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_PAGE_PROGRAM);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  SPI_transmit_n(write_buffer, size);
  PORTB |= (1 << STORAGE_CS_PIN);

  STORAGE_write_disable();
}

void STORAGE_read_data(uint32_t addr, uint8_t *read_buffer, uint32_t size) {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_DATA);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  SPI_receive_n(read_buffer, size);
  PORTB |= (1 << STORAGE_CS_PIN);
}

// allows reading at the flash chips maximum clock frequency by adding 8 dummy clocks before receiving data
void STORAGE_read_data_fast(uint32_t addr, uint8_t *read_buffer, uint32_t size) {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_DATA);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  SPI_transmit(0xff);
  SPI_receive_n(read_buffer, size);
  PORTB |= (1 << STORAGE_CS_PIN);
}

uint16_t STORAGE_read_device_id() {
  uint16_t result;

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_DEVICE_ID);
  SPI_transmit(0x00);
  SPI_transmit(0x00);
  SPI_transmit(0x00);
  result |= SPI_receive() << 8;
  result |= SPI_receive();
  PORTB |= (1 << STORAGE_CS_PIN);

  return result;
}

uint8_t STORAGE_read_status_1() {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_STATUS_1);
  uint8_t result = SPI_receive();
  PORTB |= (1 << STORAGE_CS_PIN);

  return result;
}

uint8_t STORAGE_read_status_2() {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_STATUS_2);
  uint8_t result = SPI_receive();
  PORTB |= (1 << STORAGE_CS_PIN);

  return result;
}

uint8_t STORAGE_read_status_3() {
  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_STATUS_3);
  uint8_t result = SPI_receive();
  PORTB |= (1 << STORAGE_CS_PIN);

  return result;
}

void STORAGE_print_page_data(uint32_t adddr) {
  UART_send_string("\r\n");

  uint16_t flash_read_sample;

  for (uint32_t i = 0; i < 128; i++) {
    STORAGE_read_data(i * 2, (uint8_t *)&flash_read_sample, 2);
    UART_send_number_hex(flash_read_sample);
    UART_send_string(" ");
  }

  UART_send_string("\r\n");
}

void STORAGE_print_debug_information() {
  UART_send_string("Flash chip device ID: ");
  UART_send_number_hex(STORAGE_read_device_id());
  UART_send_string("\r\n");
  UART_send_string("Flash chip status register 1: ");
  UART_send_number_hex(STORAGE_read_status_1());
  UART_send_string("\r\n");
  UART_send_string("Flash chip status register 2: ");
  UART_send_number_hex(STORAGE_read_status_2());
  UART_send_string("\r\n");
  UART_send_string("Flash chip status register 3: ");
  UART_send_number_hex(STORAGE_read_status_3());
  UART_send_string("\r\n");
}
