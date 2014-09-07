# AVR cross-compiler toolchain is used here
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

src=main
AVR_TYPE=attiny2313
AVR_FREQ=12000000
PROGRAMMER_DEV=/dev/spidev0.0
PROGRAMMER_TYPE=linuxspi

CFLAGS=-g -DF_CPU=$(AVR_FREQ) -Wall -Os -Werror -Wextra -mmcu=$(AVR_TYPE) -Iusbdrv
OBJFLAGS=-j .text -j .data -O ihex
DUDEFLAGS=-p $(AVR_TYPE) -c $(PROGRAMMER_TYPE) -P $(PROGRAMMER_DEV) -v

# Object files for the firmware (usbdrv/oddebug.o not strictly needed I think)
OBJECTS = usbdrv/usbdrv.o usbdrv/oddebug.o usbdrv/usbdrvasm.o main.o

# Command-line client
CMDLINE = usbtest

# By default, build the firmware and command-line client, but do not flash
all: main.hex

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: main.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

# One-liner to compile the command-line client from usbtest.c
$(CMDLINE): usbtest.c
	gcc -O -Wall usbtest.c -o $@ -lusb

# Housekeeping
clean:
	rm -f *.o *.hex *.elf *.lst usbdrv/*.o $(CMDLINE)

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
main.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change
# the config! I spent a few hours debugging because of this...
$(OBJECTS): usbdrv/usbconfig.h

# From C source to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@

readfuse:
	avrdude -p $(AVR_TYPE) -c $(programmerType) -P $(programmerDev) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h

# External oscilator and no divition of clock
ext-osc-no-divide:
	avrdude -p $(AVR_TYPE) -c $(programmerType) -P $(programmerDev) -U lfuse:w:0xef:m
