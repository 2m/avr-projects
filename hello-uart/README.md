This template demonstrates how to print logging information via UART using [tinytemplate](https://github.com/thegaragelab/tinytemplate) library.

## Building

First make sure that `tinytemplate` symlink is pointing to a directory that contains the library.

For 8MHz clock write fuses and flash by:

```bash
make avrType=attiny85 dudeHost=alarmpi hw=8mhz-int writefuse
make avrType=attiny85 dudeHost=alarmpi hw=8mhz-int flash
```

For 16MHz use:

```bash
make avrType=attiny85 dudeHost=alarmpi hw=16mhz-int writefuse
make avrType=attiny85 dudeHost=alarmpi hw=16mhz-int flash
```

## Getting the output

Connect `RX` pin of UART receiver to `PINB4` of ATtiny85. Then use a terminal emulator to see the output:

``` bash
sudo picocom -b 115200 /dev/ttyUSB0
```

Quit picocom by pressing `CTRL + A + Q`.
