#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"
#include "spi.h"

#define DISPLAY_CS_PIN PB1
#define DISPLAY_RESET_PIN PC0
#define DISPLAY_A0_PIN PC1

// LCD control bytes
#define DISPLAY_ON 0xaf
#define DISPLAY_OFF 0xae
#define DISPLAY_ADC_NORMAL 0xa0  // col. address 4 - 131
#define DISPLAY_ADC_REVERSE 0xa1 // col. address 0 - 127
#define DISPLAY_COL_START 0      // 0 for ADC_REVERSE, 4 for ADC_NORMAL
#define DISPLAY_DISPLAY_NORMAL 0xa6
#define DISPLAY_DISPLAY_REVERSE 0xa7
#define DISPLAY_ALL_POINTS_OFF 0xa4
#define DISPLAY_ALL_POINTS_ON 0xa5
#define DISPLAY_BIAS_RATIO_ONE_SEVENTH 0xa3
#define DISPLAY_BIAS_RATIO_ONE_NINETH 0xa2
#define DISPLAY_INTERNAL_RESET 0xe2
#define DISPLAY_COM_NORMAL 0xc0  // last 3 bits insignificant
#define DISPLAY_COM_REVERSE 0xc8 // last 3 bits insignificant
#define DISPLAY_STATIC_INDICATOR_OFF 0xac
#define DISPLAY_STATIC_INDICATOR_ON 0xad
#define DISPLAY_STATIC_INDICATOR_FLASHING_ON 0x01  // follows DISPLAY_STATIC_INDICATOR_OFF/ON
#define DISPLAY_STATIC_INDICATOR_FLASHING_OFF 0x00 // follows DISPLAY_STATIC_INDICATOR_OFF/ON
#define DISPLAY_SELECT_BOOSTER_RATIO 0xf8
#define DISPLAY_SELECT_BOOSTER_RATIO_2x3x4x 0x00
#define DISPLAY_SELECT_BOOSTER_RATIO_5x 0x01
#define DISPLAY_NO_OPERATION 0xe3
#define DISPLAY_START_LINE_0 0x40
#define DISPLAY_POWER_CONTROL_BOOSTER_REGULATOR_FOLLOWER 0x2f // 0x28 - 0x2f select internal power supply operating mode
#define DISPLAY_VOLTAGE_REGULATOR_SET 0x27                    // 0x20 - 0x27 select voltage regulator resistor ratio
#define DISPLAY_ELECTRONIC_VOLUME_SET 0x81                    // followed by 0x00 - 0x3f (contrast)

#define DISPLAY_SET_PAGE 0xb0             // 0xb0 - 0xbf (but only pages 0 - 7 used)
#define DISPLAY_SET_COLUMN_MOST_SIG 0x10  // 0x10 - 0x1f
#define DISPLAY_SET_COLUMN_LEAST_SIG 0x00 // 0x00 - 0x0f

// size of display; each row is 8 pixels tall, each column is 1 pixel wide
#define DISPLAY_ROW_SIZE 128
#define DISPLAY_NUMBER_OF_ROWS 8

extern uint8_t frameBuffer[DISPLAY_ROW_SIZE][DISPLAY_NUMBER_OF_ROWS];

extern void DISPLAY_init();
extern void DISPLAY_set_pos(uint8_t row, uint8_t col);
extern void DISPLAY_frame_buffer_set(uint8_t x, uint8_t y, uint8_t state);
extern void DISPLAY_frame_buffer_switch(uint8_t x, uint8_t y);
extern void DISPLAY_clear_frame_buffer();
extern void DISPLAY_show_frame();
extern void DISPLAY_clear();

extern void DISPLAY_hline(uint16_t x, uint16_t y, uint16_t w);
extern void DISPLAY_vline(uint16_t x, uint16_t y, uint16_t h);
extern void DISPLAY_rect_filled(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern void DISPLAY_rect_hollow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
extern void DISPLAY_rect_dashed_intensity(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t intensity);
extern void DISPLAY_rect_dashed_intensity_angle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t intensity, int16_t angle);
extern void DISPLAY_rect_dashed_0(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif
