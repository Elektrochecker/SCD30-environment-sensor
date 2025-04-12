#ifndef TWI_H
#define TWI_H

#include "config.h"

#include <avr/io.h>
// #include <stdlib.h>
// #include <util/delay.h>

// TWI status codes
#define TWI_STATUS_START 0x08                 // START condition has been transmitted
#define TWI_STATUS_REPEATED_START 0x10        // repeated START condition has been transmitted

#define TWI_STATUS_WRITE_SLA_ACK 0x18         // SLA+W has been transmitted, ACK received
#define TWI_STATUS_WRITE_SLA_NOT_ACK 0x20     // SLA+W has been transmitted, NOT ACK received
#define TWI_STATUS_WRITE_DATA_ACK 0x28        // data byte has been transmitted, ACK received
#define TWI_STATUS_WRITE_DATA_NOT_ACK 0x30    // data byte has been transmitted, NOT ACK received
#define TWI_STATUS_WRITE_FAILED 0x38          // arbitration lost / not ACK bit

#define TWI_STATUS_READ_FAILED 0x38           // arbitration lost / not ACK bit
#define TWI_STATUS_READ_SLA_ACK 0x40          // SLA+R has been transmitted, ACK received
#define TWI_STATUS_READ_SLA_NOT_ACK 0x48      // SLA+R has been transmitted, NOT ACK received
#define TWI_STATUS_READ_RECEIVED_ACK 0x50     // data byte has been received, ACK returned
#define TWI_STATUS_READ_RECEIVED_NOT_ACK 0x58 // data byte has been received, NOT ACK returned

extern void TWI_init();
extern uint8_t TWI_status_code();
extern void TWI_send_data(uint8_t data, uint8_t addr);

#endif
