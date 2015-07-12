
MCU_GCC     = atmega8
MCU_AVRDUDE = m8
OUT_DIR     = out
OBJECTS     = main.o usart_serial.o cli_parser.o i2c_master.o adc.o pwm_osc1.o state_machine.o

CC          = avr-gcc
OBJCOPY     = avr-objcopy
CFLAGS      = -g -std=c99 -mmcu=$(MCU_GCC) -Wall -Wstrict-prototypes -Os -mcall-prologues
MKDIR_P     = mkdir -p
OBJS        = $(patsubst %,$(OUT_DIR)/%,$(OBJECTS))

all:folders program.hex 

folders:; @ ${MKDIR_P} ${OUT_DIR}

program.hex: program.out ; @echo 'hex'; $(OBJCOPY) -R .eeprom -O ihex $(OUT_DIR)/program.out $(OUT_DIR)/program.hex; echo 'done!'

program.out: $(OBJS) ; @echo 'link' ; $(CC) $(CFLAGS) -o $(OUT_DIR)/program.out $(OBJS) -Wl,-Map=$(OUT_DIR)/program.map 

$(OUT_DIR)/%.o: %.c;@echo 'cc' $^;$(CC) $(CFLAGS) -o $@ -c $^; 

clean: ;rm -f $(OUT_DIR)/*.o $(OUT_DIR)/*.map $(OUT_DIR)/*.out $(OUT_DIR)/*.hex

load: program.hex; avrdude -c usbasp -p $(MCU_AVRDUDE) -u -U flash:w:program.hex
