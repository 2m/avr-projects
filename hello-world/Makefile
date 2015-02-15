programmerDev=/dev/spidev0.0
programmerType=linuxspi

cflags=-g -DF_CPU=$(avrFreq) -Wall -Os -Werror -Wextra
dudeflags=-p $(avrType) -c $(programmerType) -P $(programmerDev)
dudeMoreFuse=

ifeq ($(avrType), attiny85)
	avrFreq=8000000
	dudeflags+= -b 10000
else ifeq ($(avrType), attiny2313)
	avrFreq=12000000
	dudeMoreFuse=-U lock:r:-:h
endif

ifdef dudeHost
	# remote avrdude
	sshdude=ssh root@$(dudeHost) /usr/local/bin/avrdude
	dudeFlash=cat $(name).flash.hex $(name).flash.hex | $(sshdude)
	dudeFlashFile=-
	dudeFuse=$(sshdude)
else
	# local avrdude
	dudeFlash=avrdude
	dudeFlashFile=$(name).flash.hex
	dudeFuse=avrdude
endif

name=main
srcs=main.c
objs=$(subst .c,.o,$(srcs))
lsts=$(subst .c,.lst,$(srcs))

.PHONY: clean object elf hex flash readfuse ext-osc-no-divide int-osc-no-divide

clean:
	rm -f $(objs) $(lsts) $(name).elf $(name).eeprom.hex $(name).fuses.hex $(name).lfuse.hex $(name).hfuse.hex $(name).efuse.hex $(name).flash.hex

%.o: %.c
	avr-gcc $(cflags) -mmcu=$(avrType) -Wa,-ahlmns=$(subst .o,.lst,$@) -c -o $@ $<

elf: $(objs)
	avr-gcc $(cflags) -mmcu=$(avrType) -o $(name).elf $(objs)
	chmod a-x $(name).elf 2>&1

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(name).elf $(name).flash.hex

flash: hex
	$(dudeFlash) $(dudeflags) -v -U flash:w:$(dudeFlashFile):i

readfuse:
	$(dudeFuse) $(dudeflags) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h $(dudeMoreFuse)

# External oscillator, no division of clock
ext-osc-no-divide:
	$(dudeFuse) $(dudeflags) -U lfuse:w:0xef:m

# Internal oscillator, no division of clock
int-osc-no-divide:
	$(dudeFuse) $(dudeflags) -U lfuse:w:0xe2:m
