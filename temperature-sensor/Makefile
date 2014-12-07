avrType=attiny85
avrFreq=8000000
programmerDev=/dev/spidev0.0
programmerType=linuxspi

cflags=-g -DF_CPU=$(avrFreq) -Wall -Os -Werror -Wextra
dudeflags=-p $(avrType) -c $(programmerType) -P $(programmerDev) -b 10000

sshdude=ssh root@alarmpi /usr/local/bin/avrdude

name=main
srcs=main.cpp pmio.cpp
objs=$(subst .cpp,.o,$(srcs))
lsts=$(subst .cpp,.lst,$(srcs))

.PHONY: clean elf hex flash flash-remote readfuse ext-osc-no-divide int-osc-no-divide

clean:
	rm -f $(objs) $(lsts) $(name).elf $(name).eeprom.hex $(name).fuses.hex $(name).lfuse.hex $(name).hfuse.hex $(name).efuse.hex $(name).flash.hex

%.o: %.cpp
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(subst .o,.lst,$@) -c -o $@ $<

elf: $(objs)
	avr-gcc $(cflags) -mmcu=$(avrType) -o $(name).elf $(objs)
	chmod a-x $(name).elf 2>&1

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(name).elf $(name).flash.hex

flash: hex
	avrdude $(dudeflags) -v -U flash:w:$(name).flash.hex

flash-remote: hex
	cat $(name).flash.hex $(name).flash.hex | $(sshdude) $(dudeflags) -v -U flash:w:-:i

readfuse:
	avrdude $(dudeflags) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h

# External oscillator, no division of clock
ext-osc-no-divide:
	avrdude $(dudeflags) -U lfuse:w:0xef:m

# Internal oscillator, no division of clock
int-osc-no-divide:
	avrdude $(dudeflags) -U lfuse:w:0xe2:m
