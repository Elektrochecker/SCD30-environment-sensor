#ifndef DISPLAY_FONT_H
#define DISPLAY_FONT_H

// ORIGINAL SOURCE
// https://www.mikrocontroller.net/topic/75589
// 14.01.2008 00:15 - graphics_dogm128.zip

#include <avr/pgmspace.h>

// Font for printing ASCII characters on a graphics display

// The first character has the ASCII code 0x20, the last one 0x7F.
// Adresses can be calculated like this:
// (where &data is the beginning adress of the data and asc the ascii code)
// addr = &data + 7 * (asc - 0x20);

// Each byte of a character is a column of it.
// The least significant bit is the top most pixel, the most sig. the bottom.
// I.e. 0x01 is a pixel at the top most row, where 0x80 is one at the bottom.

// An empty column is indicated by 0xAA and is ignored while drawing text.
// If no space indicater is set, or if all characters have the same count
// of space indicators, the font is monospace.

#define CHAR_UNDEFINED {0xFF, 0x81, 0x81, 0x81, 0xFF, 0xaa, 0xaa}

extern const PROGMEM uint8_t font_standard[96][7];

#endif
