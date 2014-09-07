src=main
avrType=attiny2313
avrFreq=12000000
programmerDev=/dev/spidev0.0
programmerType=linuxspi

cflags=-g -DF_CPU=$(avrFreq) -Wall -Os -Werror -Wextra

.PHONY: clean elf flash hex object

clean:
	rm -f $(src).lst $(src).elf $(src).eeprom.hex $(src).fuses.hex $(src).lfuse.hex $(src).hfuse.hex $(src).efuse.hex $(src).flash.hex $(src).o

object:
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(src).lst -c -o $(src).o $(src).cpp

elf: object
	avr-gcc $(cflags) -mmcu=$(avrType) -o $(src).elf $(src).o
	chmod a-x $(src).elf 2>&1

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(src).elf $(src).flash.hex

flash: hex
	avrdude -p $(avrType) -c $(programmerType) -P $(programmerDev) -v -U flash:w:$(src).flash.hex

readfuse:
	avrdude -p $(avrType) -c $(programmerType) -P $(programmerDev) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h

# External oscilator and no divition of clock
ext-osc-no-divide:
	avrdude -p $(avrType) -c $(programmerType) -P $(programmerDev) -U lfuse:w:0xef:m
