#include "display.h"

uint8_t frameBuffer[DISPLAY_ROW_SIZE][DISPLAY_NUMBER_OF_ROWS] = {0};

void DISPLAY_byte(uint8_t A0, uint8_t byte) {
  // enable chipselect
  PORTB &= ~(1 << DISPLAY_CS_PIN);

  // set A0 state (0 = display control, 1 = display memory data)
  if (A0) {
    PORTC |= (1 << DISPLAY_A0_PIN);
  } else {

    PORTC &= ~(1 << DISPLAY_A0_PIN);
  }

  // send byte
  SPI_transmit(byte);

  // disable chipselect
  PORTB |= (1 << DISPLAY_CS_PIN);
}

void DISPLAY_control_byte(uint8_t uint8_t) { DISPLAY_byte(0, uint8_t); }

void DISPLAY_data_byte(uint8_t uint8_t) { DISPLAY_byte(1, uint8_t); }

void DISPLAY_init() {
  DDRB |= (1 << DISPLAY_CS_PIN);
  DDRC |= (1 << DISPLAY_A0_PIN) | (1 << DISPLAY_RESET_PIN);

  PORTB |= (1 << DISPLAY_CS_PIN);

  PORTC &= ~(1 << DISPLAY_RESET_PIN);
  PORTC |= (1 << DISPLAY_RESET_PIN);

  DISPLAY_control_byte(DISPLAY_INTERNAL_RESET);

  DISPLAY_control_byte(DISPLAY_START_LINE_0);
  DISPLAY_control_byte(DISPLAY_ADC_REVERSE);
  // DISPLAY_control_byte(DISPLAY_ADC_NORMAL);
  // changing ADC to normal requires DISPLAY_COL_START = 4
  DISPLAY_control_byte(DISPLAY_COM_NORMAL);
  // DISPLAY_control_byte(DISPLAY_COM_REVERSE);
  DISPLAY_control_byte(DISPLAY_DISPLAY_NORMAL);
  // DISPLAY_control_byte(DISPLAY_DISPLAY_REVERSE);

  // single supply 3.3V
  DISPLAY_control_byte(DISPLAY_BIAS_RATIO_ONE_NINETH);
  DISPLAY_control_byte(DISPLAY_POWER_CONTROL_BOOSTER_REGULATOR_FOLLOWER);

  DISPLAY_control_byte(DISPLAY_SELECT_BOOSTER_RATIO);
  DISPLAY_control_byte(DISPLAY_SELECT_BOOSTER_RATIO_2x3x4x);

  DISPLAY_control_byte(DISPLAY_VOLTAGE_REGULATOR_SET);

  // contrast
  DISPLAY_control_byte(DISPLAY_ELECTRONIC_VOLUME_SET);
  DISPLAY_control_byte(0x0b);

  DISPLAY_control_byte(DISPLAY_STATIC_INDICATOR_OFF);
  DISPLAY_control_byte(DISPLAY_STATIC_INDICATOR_FLASHING_OFF);

  DISPLAY_control_byte(DISPLAY_ON);
}

void DISPLAY_set_pos(uint8_t row, uint8_t col) {
  col += DISPLAY_COL_START;
  uint8_t least_significant_col = col & 0b00001111;
  uint8_t most_significant_col = (col & 0b11110000) >> 4;

  DISPLAY_control_byte(row | DISPLAY_SET_PAGE);
  DISPLAY_control_byte(most_significant_col | DISPLAY_SET_COLUMN_MOST_SIG);
  DISPLAY_control_byte(least_significant_col | DISPLAY_SET_COLUMN_LEAST_SIG);
}

// set the Value of a pixel with coordinates (x, y)
void DISPLAY_frame_buffer_set(uint8_t x, uint8_t y, uint8_t state) {
  uint8_t b = 0x01 << (y % 8);

  if (state) {
    frameBuffer[x][(int)(y / 8)] |= b;
  } else {
    frameBuffer[x][(int)(y / 8)] &= ~b;
  }
}

// change the Value of a pixel with coordinates (x, y)
void DISPLAY_frame_buffer_switch(uint8_t x, uint8_t y) {
  uint8_t b = 0x01 << (y % 8);
  frameBuffer[x][(int)(y / 8)] ^= b;
}

void DISPLAY_clear_frame_buffer() {
  for (int j = 0; j < DISPLAY_NUMBER_OF_ROWS; j++) {
    for (int i = 0; i < DISPLAY_ROW_SIZE; i++) {
      frameBuffer[i][j] = 0x00;
    }
  }
}

void DISPLAY_show_frame() {
  for (int j = 0; j < DISPLAY_NUMBER_OF_ROWS; j++) {
    DISPLAY_set_pos(j, 0);
    for (int i = 0; i < DISPLAY_ROW_SIZE; i++) {
      DISPLAY_data_byte(frameBuffer[i][j]);
    }
  }
}

void DISPLAY_clear() {
  for (int i = 0; i < DISPLAY_NUMBER_OF_ROWS; i++) {
    DISPLAY_set_pos(i, 0);
    for (int j = 0; j < DISPLAY_ROW_SIZE; j++) {
      DISPLAY_data_byte(0x00);
    }
  }
}

// lines and rectangles
void DISPLAY_hline(uint16_t x, uint16_t y, uint16_t w) {
  for (uint8_t ix = x; ix < w + x; ix++) {
    DISPLAY_frame_buffer_set(ix, y, 1);
  }
}

void DISPLAY_vline(uint16_t x, uint16_t y, uint16_t h) {
  for (uint8_t iy = y; iy < h + y; iy++) {
    DISPLAY_frame_buffer_set(x, iy, 1);
  }
}

void DISPLAY_rect_filled(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  for (uint8_t iy = y; iy < h + y; iy++) {
    for (uint8_t ix = x; ix < w + x; ix++) {
      DISPLAY_frame_buffer_set(ix, iy, 1);
    }
  }
}

void DISPLAY_rect_hollow(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  for (uint8_t iy = y; iy < h + y; iy++) {
    for (uint8_t ix = x; ix < w + x; ix++) {
      if (ix == x || ix == x + w - 1 || iy == y || iy == y + h - 1) {
        DISPLAY_frame_buffer_set(ix, iy, 1);
      } else {
        DISPLAY_frame_buffer_set(ix, iy, 0);
      }
    }
  }
}

void DISPLAY_rect_dashed_intensity(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t intensity) {
  // best values for intensity are 2, 3, 4
  for (uint8_t iy = y; iy < h + y; iy++) {
    for (uint8_t ix = x; ix < w + x; ix++) {
      if (ix == x || ix == x + w - 1 || iy == y || iy == y + h - 1) {
        DISPLAY_frame_buffer_set(ix, iy, 1);
      } else {
        DISPLAY_frame_buffer_set(ix, iy, (ix - iy) % intensity == 0);
      }
    }
  }
}

void DISPLAY_rect_dashed_intensity_angle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t intensity, int16_t angle) {
  // best values for intensity are 2, 3, 4
  // angle 1 normal, angle -1 flipped direction
  // angles with absolute values greater than 1 have interesting dotted effects
  for (uint8_t iy = y; iy < h + y; iy++) {
    for (uint8_t ix = x; ix < w + x; ix++) {
      if (ix == x || ix == x + w - 1 || iy == y || iy == y + h - 1) {
        DISPLAY_frame_buffer_set(ix, iy, 1);
      } else {
        DISPLAY_frame_buffer_set(ix, iy, (ix + angle * iy) % intensity == 0);
      }
    }
  }
}

void DISPLAY_rect_dashed_noborder(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t intensity, int16_t angle) {
  // best values for intensity are 2, 3, 4
  // angle 1 normal, angle -1 flipped direction
  // angles with absolute values greater than 1 have interesting dotted effects
  for (uint8_t iy = y; iy < h + y; iy++) {
    for (uint8_t ix = x; ix < w + x; ix++) {
      if (ix == x || ix == x + w - 1 || iy == y || iy == y + h - 1) {
        // no border
      } else {
        DISPLAY_frame_buffer_set(ix, iy, (ix + angle * iy) % intensity == 0);
      }
    }
  }
}

void DISPLAY_rect_dashed_0(uint16_t x, uint16_t y, uint16_t w, uint16_t h) { DISPLAY_rect_dashed_intensity(x, y, w, h, 2); }

// drawing text
void DISPLAY_write(char *str) {
  for (uint32_t j = 0; j < strlen(str); j++) {
    for (uint8_t i = 0; i < 7; i++) {
      uint8_t b = pgm_read_byte(font_standard[str[j] - 0x20] + i);
      if (b == 0xaa) {
        DISPLAY_data_byte(0x00);
        break;
      } else {
        DISPLAY_data_byte(b);
      }
    }
  }
}
