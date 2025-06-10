#include "storage.h"

// location of the last written datapoint in units of sizeof SENSOR_datapoint
uint32_t STORAGE_current_location;

// The WEL bit must be set prior to every Page Program, Quad Page Program, Sector Erase, Block
// Erase, Chip Erase
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

void STORAGE_init() {
  DDRB |= (1 << STORAGE_CS_PIN);
  PORTB |= (1 << STORAGE_CS_PIN);

  STORAGE_write_disable();
}

// The Page Program instruction allows from one byte to 256 bytes (a page) of data to be programmed at
// previously erased (FFh) memory locations. A Write Enable instruction must be executed before the device
// will accept the Page Program Instruction
void STORAGE_write_data(uint32_t addr, uint8_t *write_buffer, uint16_t size) {
  addr = addr & STORAGE_MAX_ADDR;

  PORTD |= (1 << LED_STORAGE_WRITE);
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_PAGE_PROGRAM);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  SPI_transmit_n(write_buffer, size);
  PORTB |= (1 << STORAGE_CS_PIN);

  _delay_ms(4); // t_pp <= 3ms

  STORAGE_write_disable();
  PORTD &= ~(1 << LED_STORAGE_WRITE);
}

// The Read Data instruction allows one or more data bytes to be sequentially read from the memory
void STORAGE_read_data(uint32_t addr, uint8_t *read_buffer, uint32_t size) {
  addr = addr & STORAGE_MAX_ADDR;

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
  addr = addr & STORAGE_MAX_ADDR;

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_DATA);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  SPI_transmit(0xff);
  SPI_receive_n(read_buffer, size);
  PORTB |= (1 << STORAGE_CS_PIN);
}

// The Sector Erase instruction sets all memory within a specified sector (4K-bytes) to the erased state of all
// 1s (FFh). A Write Enable instruction must be executed before the device will accept the Sector Erase
// Instruction
void STORAGE_sector_erase(uint32_t addr) {
  addr = addr & STORAGE_MAX_ADDR;

  PORTD |= (1 << LED_STORAGE_WRITE);
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_SECTOR_ERASE);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  PORTB |= (1 << STORAGE_CS_PIN);

  _delay_ms(500); // t_se <= 400ms

  STORAGE_write_disable();
  PORTD &= ~(1 << LED_STORAGE_WRITE);
}

// The Block Erase instruction sets all memory within a specified block (32K-bytes) to the erased state of all
// 1s (FFh). A Write Enable instruction must be executed before the device will accept the Block Erase
// Instruction
void STORAGE_block_erase_32k(uint32_t addr) {
  addr = addr & STORAGE_MAX_ADDR;

  PORTD |= (1 << LED_STORAGE_WRITE);
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_BLOCK_ERASE_32K);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  PORTB |= (1 << STORAGE_CS_PIN);

  _delay_ms(1700); // t_be1 <= 1600ms

  STORAGE_write_disable();
  PORTD &= ~(1 << LED_STORAGE_WRITE);
}

// The Block Erase instruction sets all memory within a specified block (64K-bytes) to the erased state of all
// 1s (FFh). A Write Enable instruction must be executed before the device will accept the Block Erase
// Instruction
void STORAGE_block_erase_64k(uint32_t addr) {
  addr = addr & STORAGE_MAX_ADDR;

  PORTD |= (1 << LED_STORAGE_WRITE);
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_BLOCK_ERASE_64K);
  SPI_transmit((addr >> 16) & 0xff);
  SPI_transmit((addr >> 8) & 0xff);
  SPI_transmit((addr >> 0) & 0xff);
  PORTB |= (1 << STORAGE_CS_PIN);

  _delay_ms(2100); // t_be1 <= 2000ms

  STORAGE_write_disable();
  PORTD &= ~(1 << LED_STORAGE_WRITE);
}

// The Chip Erase instruction sets all memory within the device to the erased state of all 1s (FFh). A Write
// Enable instruction must be executed before the device will accept the Chip Erase Instruction
void STORAGE_chip_erase() {
  PORTD |= (1 << LED_STORAGE_WRITE);
  STORAGE_write_enable();

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_CHIP_ERASE);
  PORTB |= (1 << STORAGE_CS_PIN);

  // t_ce <= 50s
  for (uint8_t i = 0; i < 52; i++) {
    _delay_ms(1000);
  }

  STORAGE_write_disable();
  PORTD &= ~(1 << LED_STORAGE_WRITE);
}

uint16_t STORAGE_read_device_id() {
  uint16_t result;

  PORTB &= ~(1 << STORAGE_CS_PIN);
  SPI_transmit(STORAGE_CMD_READ_DEVICE_ID);
  SPI_transmit(0x00);
  SPI_transmit(0x00);
  SPI_transmit(0x00);
  result = SPI_receive() << 8;
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

void STORAGE_save_datapoint(SENSOR_reading sensor_reading, DATETIME time) {
  uint32_t location = ++STORAGE_current_location;

  if (location * sizeof(SENSOR_datapoint) > STORAGE_MAX_ADDR)
    return;

  SENSOR_datapoint datapoint = {.temperature = sensor_reading.temperature,
                                .humidity = sensor_reading.humidity,
                                .co2concentration = sensor_reading.co2concentration,
                                .timestamp = CLOCK_datetime_to_reduced(time)};

  uint32_t addr = location * sizeof(SENSOR_datapoint);
  STORAGE_write_data(addr, (uint8_t *)&datapoint, sizeof(SENSOR_datapoint));
}

SENSOR_datapoint STORAGE_recall_datapoint(uint32_t location) {
  SENSOR_datapoint result = {0};

  if (location * sizeof(SENSOR_datapoint) > STORAGE_MAX_ADDR) {
    return result;
  }

  uint32_t addr = location * sizeof(SENSOR_datapoint);

  STORAGE_read_data(addr, (uint8_t *)&result, sizeof(SENSOR_datapoint));

  return result;
}

// scans the memory of the flash chip to find the location to resume writing
uint32_t STORAGE_scan_location() {
  PORTD |= (1 << LED_STORAGE_WRITE);
  uint32_t current = 0;

  // find the page with end of data
  for (uint32_t i = 1; i <= STORAGE_MAX_ADDR / sizeof(SENSOR_datapoint); i += 16) {
    uint32_t addr = i * sizeof(SENSOR_datapoint) - 1;
    uint8_t reading = 0;
    STORAGE_read_data(addr, &reading, 1);

    // the last byte of a SENSOR_datapoint is always zero.
    // the first reading with a 1 is therefore the end of written data
    if (reading & 0x80) {
      current = i;
      break;
    }
  }

  // edge case: no datapoints in flash
  if (current == 1) {
    uint32_t addr = sizeof(SENSOR_datapoint) - 1;
    uint8_t reading = 0;
    STORAGE_read_data(addr, &reading, 1);

    if (reading & 0x80) {
      PORTD &= ~(1 << LED_STORAGE_WRITE);
      return -1;
    }
  }

  // find the actual end of data
  for (uint32_t i = current; i < STORAGE_MAX_ADDR / sizeof(SENSOR_datapoint); i--) {
    uint32_t addr = i * sizeof(SENSOR_datapoint) - 1;
    uint8_t reading = 0;
    STORAGE_read_data(addr, &reading, 1);

    // the last byte of a SENSOR_datapoint is always zero.
    // the first reading with a 1 is therefore the end of written data
    if (!(reading & 0x80)) {
      PORTD &= ~(1 << LED_STORAGE_WRITE);
      return i;
    }
  }

  PORTD &= ~(1 << LED_STORAGE_WRITE);
  return STORAGE_MAX_ADDR / sizeof(SENSOR_datapoint);
}

// print the entire flash database in CSV-like format
const PROGMEM char STORAGE_csv_header[] = "\r\nmin\t;hour\t;day\t;month\t;year\t;temperature[C]\t;humidity[%]\t;co2concentration[ppm]\r\n";
void STORAGE_dump_datapoints_to_uart() {
  if (STORAGE_current_location > STORAGE_MAX_ADDR / sizeof(SENSOR_datapoint)) {
    return;
  }

  UART_send_string_P(STORAGE_csv_header);

  for (uint32_t i = 0; i < STORAGE_current_location; i++) {
    SENSOR_datapoint datapoint = STORAGE_recall_datapoint(i);
    DATETIME time = CLOCK_datetime_from_reduced(datapoint.timestamp);

    UART_send_number(time.minutes);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_number(time.hours);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_number(time.day);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_number(time.month);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_number(time.year);
    UART_send_char('\t');
    UART_send_char(';');

    UART_send_float(datapoint.temperature, 4, 1);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_float(datapoint.humidity, 4, 1);
    UART_send_char('\t');
    UART_send_char(';');
    UART_send_float(datapoint.co2concentration, 4, 1);
    UART_send_char('\r');
    UART_send_char('\n');
  }
}
