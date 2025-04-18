compile for AVR
```
./avr-compile.sh main.c
```

flash with avrdude
```
avrdude -v -c stk500 -p m328p -P /dev/ttyACM0 -U flash:w:build/main.hex:i
```
