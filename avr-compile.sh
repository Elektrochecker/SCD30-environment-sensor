#!/bin/bash -e

# packages for fedora 40
# sudo dnf install avr* clang-tools-extra

PART=atmega328p

echo $PART | toilet --metal

FNAME=$1
INCLUDES="uart.h uart.c twi.h twi.c"

find $FNAME > /dev/null || exit
find build/ > /dev/null || mkdir build

# remove .c / .cpp file extension
NAME=$(echo $FNAME | sed "s/.c\b\|.cpp\b//")

echo "compiling $NAME..."

#compile with avr-gcc, all Warnings enabled
avr-gcc $FNAME $INCLUDES -mmcu=$PART -Wall -Os -o build/$NAME.elf

# link to binary and intel hex format
avr-objcopy -j .text -j .data -O ihex build/$NAME.elf build/$NAME.hex
# avr-objcopy -j .text -j .data build/$NAME.elf build/$NAME.bin

echo "done compiling."
echo
echo
