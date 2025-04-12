#ifndef CONFIG_H
#define CONFIG_H

// mcu clock frequency [Hz]
#define F_CPU 16000000

// compiling with the part flag (-mmcu) set will automatically set
// this macro, so we guard for redefinition
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#endif
