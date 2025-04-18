#include "twi.h"
#include "uart.h"
uint8_t TWI_last_error;

void TWI_init() {
  // set error LED to output
  DDRD |= (1 << LED_TWI_ERR);

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
  TWBR = 6; // 20 kHz
}

void TWI_error(uint8_t code) {
  TWI_last_error = code;
  // switch on TWI error LED
  PORTD |= (1 << LED_TWI_ERR);

  UART_println("encountered a TWI error:");

  switch (code) {
  // handle different error situations
  case 0x08:
    UART_println("START condition has been transmitted");
    break;
  case 0x10:
    UART_println("repeated START condition has been transmitted");
  case 0x18:
    UART_println("SLA+W has been transmitted, ACK received");
    break;
  case 0x20:
    UART_println("SLA+W has been transmitted, NOT ACK received");
    break;
  case 0x28:
    UART_println("data byte has been transmitted, ACK received");
    break;
  case 0x30:
    UART_println("data byte has been transmitted, NOT ACK received");
    break;
  case 0x38:
    UART_println("arbitration lost / not ACK bit");
    break;
  case 0x40:
    UART_println("SLA+R has been transmitted, ACK received");
    break;
  case 0x48:
    UART_println("SLA+R has been transmitted, NOT ACK received");
    break;
  case 0x50:
    UART_println("data byte has been received, ACK returned");
    break;
  case 0x58:
    UART_println("data byte has been received, NOT ACK returned");
    break;
  case 0xf8:
    UART_println("no relevant information available");
    break;
  }
}

void TWI_success() {
  // switch off TWI error LED
  PORTD &= ~(1 << LED_TWI_ERR);
}

uint8_t TWI_status_code() {
  // TWI status code register with the prescaler bits masked out
  return TWSR & 0xf8;
}

void TWI_start_condition() {
  // Send START condition
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

  // Wait for TWINT Flag set. This indicates
  // that the START condition has been
  // transmitted.
  while (!(TWCR & (1 << TWINT)))
    ;
}

void TWI_stop_condition() { TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO); }

void TWI_send_address(uint8_t addr, uint8_t rw) {
  uint8_t SLA_X = (addr << 1) | rw;

  // Load SLA_W into TWDR Register. Clear
  // TWINT bit in TWCR to start transmission of
  // address.
  TWDR = SLA_X;
  TWCR = (1 << TWINT) | (1 << TWEN);

  // Wait for TWINT Flag set. This indicates
  // that the SLA+W has been transmitted, and
  // ACK/NACK has been received.
  while (!(TWCR & (1 << TWINT)))
    ;

  // Check value of TWI Status Register. Mask
  // prescaler bits. If status different from MT_SLA_ACK, return.
  if (TWI_status_code() != TWI_STATUS_WRITE_SLA_ACK && TWI_status_code() != TWI_STATUS_READ_SLA_ACK) {

    TWI_error(TWI_status_code());
    return;
  }
}

// transmit an amount of data to a TWI slave without sending repeated start condition.
// START -> SLA+W -> data .... data -> STOP
void TWI_send_data(uint8_t *data_buffer, uint8_t len, uint8_t addr) {
  // slave address + write bit

  TWI_start_condition();
  TWI_send_address(addr, 0);

  // // fake clock stretching for SCD30
  // _delay_ms(40);

  for (uint8_t i = 0; i < len; i++) {
    // Load DATA into TWDR Register. Clear
    // TWINT bit in TWCR to start transmission of
    // data.
    TWDR = data_buffer[i];
    TWCR = (1 << TWINT) | (1 << TWEN);

    // Wait for TWINT Flag set. This indicates
    // that the DATA has been transmitted, and
    // ACK/NACK has been received.
    while (!(TWCR & (1 << TWINT)))
      ;

    // Check value of TWI Status Register. Mask
    // prescaler bits. If status different from MT_DATA_ACK go to ERROR.
    if (TWI_status_code() != TWI_STATUS_WRITE_DATA_ACK) {
      TWI_error(TWI_status_code());
    } else {
      TWI_success();
    }
  }

  // Transmit STOP condition.
  TWI_stop_condition();
}

void TWI_send_byte(uint8_t data, uint8_t addr) { return TWI_send_data(&data, 1, addr); }

void TWI_read_data(uint8_t *receive_buffer, uint8_t len, uint8_t addr) {
  // Send START condition
  TWI_start_condition();

  // send SLA+R
  TWI_send_address(addr, 1);

  // receive data bytes and transmit ACK / NACK
  for (uint8_t i = 0; i < len; i++) {
    if (i == len - 1) {
      // last data byte: return NACK
      TWCR = (1 << TWINT) | (0 << TWEA) | (1 << TWEN);

      // Received data can be read from the TWDR Register
      // when the TWINT Flag is set high by hardware
      // wait until TWINT is high.
      while (!(TWCR & (1 << TWINT))) {
      }

      if (TWI_status_code() != TWI_STATUS_READ_RECEIVED_NOT_ACK) {
        UART_println("error while receiving+NACK");
        TWI_error(TWI_status_code());
      }
    } else {
      // not last data byte: return ACK
      TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);

      // Received data can be read from the TWDR Register
      // when the TWINT Flag is set high by hardware
      // wait until TWINT is high.
      while (!(TWCR & (1 << TWINT))) {
      }

      if (TWI_status_code() != TWI_STATUS_READ_RECEIVED_ACK) {
        UART_println("error while receiving+ACK");
        TWI_error(TWI_status_code());
      }
    }

    // read data from TWI data register
    receive_buffer[i] = TWDR;
  }

  // Transmit STOP condition.
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

  TWI_success();
}
