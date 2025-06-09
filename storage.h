#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"
#include "spi.h"
#include "uart.h"

#include <avr/io.h>

#define STORAGE_CS_PIN PB2

#define STORAGE_CMD_PAGE_PROGRAM 0x02
#define STORAGE_CMD_READ_DATA 0x03
#define STORAGE_CMD_READ_DEVICE_ID 0x90
#define STORAGE_CMD_READ_STATUS_1 0x05
#define STORAGE_CMD_READ_STATUS_2 0x15
#define STORAGE_CMD_READ_STATUS_3 0x35
#define STORAGE_CMD_WRITE_DISABLE 0x04
#define STORAGE_CMD_WRITE_ENABLE 0x06

extern void STORAGE_init();
extern void STORAGE_write_data(uint32_t addr, uint8_t *write_buffer, uint32_t size);
extern void STORAGE_read_data(uint32_t addr, uint8_t *read_buffer, uint32_t size);
extern void STORAGE_read_data_fast(uint32_t addr, uint8_t *read_buffer, uint32_t size);
extern uint16_t STORAGE_read_device_id();
extern uint8_t STORAGE_read_status_1();
extern uint8_t STORAGE_read_status_2();
extern uint8_t STORAGE_read_status_3();
extern void STORAGE_print_page_data(uint32_t adddr);
extern void STORAGE_print_debug_information();

#endif
