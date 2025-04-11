TARGET=firmware
MCU=atmega328p # model of your microcontroller
F_CPU=16000000UL

BAUD=115200 # baud rate of arduino bootloader, for old models use 57600
PORT=COM3 # serial port that connected to your arduino

CC=avr-gcc
CFLAGS=-Wall -mmcu=$(MCU) -Os -DF_CPU=$(F_CPU) -I include -I lib/include -I images

SRC=src/main.c src/port.c src/uart.c src/game.c # you can add your files in src dir
SRC += lib/spi.c lib/st7735.c lib/st7735_font.c lib/st7735_gfx.c

BUILD_DIR=build
OBJ=$(foreach file,$(SRC),$(BUILD_DIR)/$(file:.c=.o))

all: info $(BUILD_DIR)/$(TARGET).hex
	@echo Build done!
	
info:
	@echo ===================
	@echo github.com/ifelsik
	@echo Build started
	@echo Controller: $(MCU)
	@echo Quartz F_CPU: $(F_CPU)
	@echo PORT: $(PORT)
	@echo Bootloader baudrate: $(BAUD)
	@echo ===================

$(BUILD_DIR)/%.o: %.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	avr-objcopy -O ihex $< $@


flash: $(BUILD_DIR)/$(TARGET).hex
	avrdude -p m328p -c arduino \
		-P $(PORT) -b $(BAUD) \
		-U flash:w:$(BUILD_DIR)/$(TARGET).hex:i

clean:
	if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)