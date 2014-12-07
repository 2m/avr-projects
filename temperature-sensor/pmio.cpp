#include <avr/io.h>
#include <util/delay.h>

// only PORTB[0] output bit is used
#define PORT PORTB

#define BLINK_MS 150
#define BLINK_SEPARATOR_MS BLINK_MS*2
#define BLINK_LONG_MS BLINK_MS*4
#define BLINK_SHORT_MS BLINK_MS/3

#define PWM_CHIP_US 1
#define PWM_CHIPS_IN_CYCLE 1000
// PWM_CHIPS_IN_CYCLE <= 1000 makes all hold_ms > 0 values valid.

void blink(int n) {
  for(int i = 0; i < n; i++)
  {
    PORT |= 1;
    _delay_ms(BLINK_MS);
    PORT &= ~1;
    _delay_ms(BLINK_MS);
  }
  _delay_ms(BLINK_SEPARATOR_MS);
}

void blink_radix(int val, int radix) {
  PORT |= 1;
  _delay_ms(BLINK_LONG_MS);
  PORT &= ~1;
  _delay_ms(BLINK_LONG_MS);

  int n = 1;
  while(val > 0)
  {
    n = val % radix;
    val /= radix;
    blink(n);

    PORT |= 1;
    _delay_ms(BLINK_SHORT_MS);
    PORT &= ~1;
    _delay_ms(BLINK_SHORT_MS);
    PORT |= 1;
    _delay_ms(BLINK_SHORT_MS);
    PORT &= ~1;
    _delay_ms(BLINK_SHORT_MS + BLINK_SEPARATOR_MS + BLINK_MS);
  }
}

void light(float val, long hold_ms) {
  long cycles = hold_ms * 1000 / (PWM_CHIPS_IN_CYCLE * PWM_CHIP_US);
  int on_chips = int(val * PWM_CHIPS_IN_CYCLE);
  if (on_chips < 0) on_chips = 0;
  else if (on_chips > PWM_CHIPS_IN_CYCLE) on_chips = PWM_CHIPS_IN_CYCLE;
  int off_chips = PWM_CHIPS_IN_CYCLE - on_chips;

  PORT &= ~1;
  for(long i = 0; i < cycles; i++)
  {
    // _delay_us only takes compile-time constants as arguments
    PORT |= 1;
    for (int j = 0; j < on_chips; j++)
      _delay_us(PWM_CHIP_US);
    PORT &= ~1;
    for (int j = 0; j < off_chips; j++)
      _delay_us(PWM_CHIP_US);
  }
}
