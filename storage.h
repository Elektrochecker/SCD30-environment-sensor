#ifndef STORAGE_H
#define STORAGE_H

#include "clock.h"
#include "config.h"
#include "sensor.h"
#include "spi.h"
#include "uart.h"

#include <avr/eeprom.h>
#include <avr/io.h>

// Flash write LED
#define LED_STORAGE_WRITE PD6

#define STORAGE_CS_PIN PB2

#define STORAGE_CMD_BLOCK_ERASE_32K 0x52
#define STORAGE_CMD_BLOCK_ERASE_64K 0xd8
#define STORAGE_CMD_CHIP_ERASE 0xc7 // 0xc7 or 0x60
#define STORAGE_CMD_PAGE_PROGRAM 0x02
#define STORAGE_CMD_READ_DATA 0x03
#define STORAGE_CMD_READ_DEVICE_ID 0x90
#define STORAGE_CMD_READ_STATUS_1 0x05
#define STORAGE_CMD_READ_STATUS_2 0x15
#define STORAGE_CMD_READ_STATUS_3 0x35
#define STORAGE_CMD_SECTOR_ERASE 0x20
#define STORAGE_CMD_WRITE_DISABLE 0x04
#define STORAGE_CMD_WRITE_ENABLE 0x06

// largest possible address in bytes (32Mbit)
#define STORAGE_MAX_ADDR 0x3fffff

// location of the last written datapoint in units of sizeof SENSOR_datapoint
extern uint32_t STORAGE_current_location;

extern void STORAGE_init();

extern void STORAGE_write_data(uint32_t addr, uint8_t *write_buffer, uint16_t size);
extern void STORAGE_read_data(uint32_t addr, uint8_t *read_buffer, uint32_t size);
extern void STORAGE_read_data_fast(uint32_t addr, uint8_t *read_buffer, uint32_t size);

extern uint16_t STORAGE_read_device_id();
extern uint8_t STORAGE_read_status_1();
extern uint8_t STORAGE_read_status_2();
extern uint8_t STORAGE_read_status_3();
extern void STORAGE_print_page_data(uint32_t adddr);
extern void STORAGE_print_debug_information();

// location = start address in flash / sizeof SENSOR_datapoint
extern void STORAGE_save_datapoint(SENSOR_reading sensor_reading, DATETIME time);
extern SENSOR_datapoint STORAGE_recall_datapoint(uint32_t location);
extern uint32_t STORAGE_scan_location();
extern void STORAGE_dump_datapoints_to_uart();

extern void STORAGE_sector_erase(uint32_t addr);
extern void STORAGE_block_erase_32k(uint32_t addr);
extern void STORAGE_block_erase_64k(uint32_t addr);
extern void STORAGE_chip_erase();

#endif
