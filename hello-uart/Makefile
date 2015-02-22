programmerDev=/dev/spidev0.0
programmerType=linuxspi

optimise=-ffunction-sections -fdata-sections -ffreestanding
cflags=-g -DF_CPU=$(avrFreq) -mmcu=$(avrType) -Wall -Os -Werror -Wextra -std=gnu99
dudeflags=-p $(avrType) -c $(programmerType) -P $(programmerDev)
dudeMoreFuse=

ifeq ($(avrType), attiny85)
	dudeflags+= -b 10000
else ifeq ($(avrType), attiny2313)
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

ifeq ($(hw), 16mhz-int)
	avrFreq=16500000
	fuses=-U lfuse:w:0xe1:m -U hfuse:w:0xdd:m
else ifeq ($(hw), 8mhz-int)
  avrFreq=8000000
  fuses=-U lfuse:w:0xe2:m -U hfuse:w:0xdd:m
endif

name=main
srcs=main.c
objs=$(subst .c,.o,$(srcs))
lsts=$(subst .c,.lst,$(srcs))

# definitions that override defines in various libraries
cflags+=-include definitions.h

# https://github.com/thegaragelab/tinytemplate library
tinytpl=tinytemplate
cflags+=-I $(tinytpl)/firmware/include
srcs+= \
	$(tinytpl)/firmware/shared/uart_send.c

.PHONY: clean object elf hex flash readfuse ext-osc-no-divide int-osc-no-divide

clean:
	rm -f $(objs) $(lsts) $(name).elf $(name).eeprom.hex $(name).fuses.hex $(name).lfuse.hex $(name).hfuse.hex $(name).efuse.hex $(name).flash.hex

%.o: %.c
	avr-gcc $(cflags) $(optimise) -Wa,-ahlmns=$(subst .o,.lst,$@) -c -o $@ $<

elf: $(objs)
	avr-gcc $(cflags) $(optimise) -o $(name).elf $(objs)
	chmod a-x $(name).elf 2>&1

hex: elf
	avr-objcopy -j .text -j .data -O ihex $(name).elf $(name).flash.hex

flash: hex
	$(dudeFlash) $(dudeflags) -v -U flash:w:$(dudeFlashFile):i

readfuse:
	$(dudeFuse) $(dudeflags) -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h $(dudeMoreFuse)

writefuse:
	$(dudeFuse) $(dudeflags) -e $(fuses)
