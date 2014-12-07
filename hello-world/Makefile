src=main
avrType=attiny2313
avrFreq=12000000
programmerDev=/dev/spidev0.0
programmerType=linuxspi

cflags=-g -DF_CPU=$(avrFreq) -Wall -Os -Werror -Wextra
dudeflags=-p $(avrType) -c $(programmerType) -P $(programmerDev)

ifeq ($(avrType), attiny85)
avrFreq=8000000
dudeflags+= -b 10000
endif

.PHONY: clean object elf hex flash readfuse ext-osc-no-divide int-osc-no-divide

clean:
	rm -f $(src).lst $(src).elf $(src).eeprom.hex $(src).fuses.hex $(src).lfuse.hex $(src).hfuse.hex $(src).efuse.hex $(src).flash.hex $(src).o

object:
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(src).lst -c -o $(src).o $(src).c

elf: object
	avr-gcc $(cflags) -mmcu=$(avrType) -o $(src).elf $(src).o
	chmod a-x $(src).elf 2>&1

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(src).elf $(src).flash.hex

flash: hex
	avrdude $(dudeflags) -v -U flash:w:$(src).flash.hex

ifeq ($(avrType), attiny85)
readfuse:
	avrdude $(dudeflags) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
else
readfuse:
	avrdude $(dudeflags) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h
endif

# External oscillator, no division of clock
ext-osc-no-divide:
	avrdude $(dudeflags) -U lfuse:w:0xef:m

# Internal oscillator, no division of clock
int-osc-no-divide:
	avrdude $(dudeflags) -U lfuse:w:0xe2:m
