#include "twi.h"

void TWI_init() {
  // The Power Reduction TWI bit in the Power Reduction Register (PRRn.PRTWI)
  // must be written to '0' to enable the two-wire Serial Interface.
  PRR &= ~(1 << PRTWI);

  // TWS, prescaler value
  // 00, 1
  // 01, 4
  // 10, 16
  // 11, 64

  // set TWI prescaler
  TWSR |= (1 << TWPS0);
  TWSR |= (1 << TWPS1);

  // set TWI bit rate register
  // SCL frequency = F_CPU / (16 + 2*(TWBR)*(prescaler value))
  TWBR = 100; // 1248 Hz
}

void TWI_error() {
  // do nothing. perhaps implement an error LED at some point.
}

void TWI_start_condition() {
  // Send START condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

  // Wait for TWINT Flag set. This indicates
  // that the START condition has been
  // transmitted.
  while (!(TWCR & (1 << TWINT)))
    ;

  // Check value of TWI Status Register. Mask prescaler bits.
  if (TWI_status_code() != TWI_STATUS_START) {
    TWI_error();
  }
}

void TWI_stop_condition() {
  // Transmit STOP condition.
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

uint8_t TWI_status_code() {
  // TWI status code register with the prescaler bits masked out
  return TWSR & 0xf8;
}

// transmit an amount of data to a TWI slave without sending repeated start condition.
// START -> SLA+W -> data .... data -> STOP
void TWI_send_data(uint8_t *data, uint8_t len, uint8_t addr) {
  // slave address + write bit
  uint8_t SLA_W = (addr << 1) | 0x00;

  TWI_start_condition();

  // Load SLA_W into TWDR Register. Clear
  // TWINT bit in TWCR to start transmission of
  // address.
  TWDR = SLA_W;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Wait for TWINT Flag set. This indicates
  // that the SLA+W has been transmitted, and
  // ACK/NACK has been received.
  while (!(TWCR & (1 << TWINT)))
    ;

  // Check value of TWI Status Register. Mask
  // prescaler bits. If status different from MT_SLA_ACK, return.
  if (TWI_status_code() != TWI_STATUS_WRITE_SLA_ACK) {
    if (TWI_status_code() == TWI_STATUS_WRITE_SLA_NOT_ACK) {
      TWI_stop_condition();
    }

    TWI_error();
    return;
  }

  for (uint8_t i = 0; i < len; i++) {
    // Load DATA into TWDR Register. Clear
    // TWINT bit in TWCR to start transmission of
    // data.
    TWDR = data[i];
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT Flag set. This indicates
    // that the DATA has been transmitted, and
    // ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)))
      ;

    // Check value of TWI Status Register. Mask
    // prescaler bits. If status different from MT_DATA_ACK go to ERROR.
    if (TWI_status_code() != TWI_STATUS_WRITE_DATA_ACK) {
      TWI_error();
      TWI_stop_condition();
      return;
    }
  }

  TWI_stop_condition();
}

void TWI_send_byte(uint8_t data, uint8_t addr) { TWI_send_data(&data, 1, addr); }
