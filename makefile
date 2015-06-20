MCU=atmega8
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -std=c99 -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

all: program.hex

program.hex : program.out 
	$(OBJCOPY) -R .eeprom -O ihex program.out program.hex
program.out : objects
	$(CC) $(CFLAGS) -o program.out -Wl,-Map,program.map main.o usart_serial.o cli_parser.o i2c_master.o
objects : main.c usart_serial.c cli_parser.c i2c_master.c
	$(CC) $(CFLAGS) -Os -c main.c usart_serial.c cli_parser.c i2c_master.c
load: program.hex
	avrdude -c usbasp -p m8 -u -U flash:w:program.hex

clean:
	rm -f *.o *.map *.out *.hex

