#include <avr/io.h>
#include <util/delay.h>
#include "pmio.h"

int adps_below(long below_hz) {
  // 50 - 200 kHz ADC clock rate is required for maximum (10-bit) resolution
  // find the highest division factor, power of 2, below the given Hz.
  int divpower = 1;
  long rate = F_CPU;
  while (rate > below_hz) {
    rate /= 2;
    divpower++;
  }
  return divpower;
}

int main() {

  ADMUX = (0<<REFS2) | (1<<REFS1) | (0<<REFS0) | // 1.1V ref for temp sensor
          (0<<ADLAR) | // result right-adjusted
          (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0); // temperature sensor

  int adps = adps_below(200000);
  ADCSRA = (1<<ADEN) | // enable ADC
           (1<<ADSC) | // start conversion
           (0<<ADATE) | // disable auto trigger
           (0<<ADIE) | // disable interrupt
           //select ADC clock division factor
           ((adps>>2&1)<<ADPS2) | ((adps>>1&1)<<ADPS1) | ((adps&1)<<ADPS0);

  DDRB |= 1; // LED on PB0
  while(1) {
    if (ADCSRA & (1<<ADIF)) {
      // Reading ADCL blocks ADC access to ADCL and ADCH.
      // Reading ADCH releases it. ADCL should always be read before ADCH.
      int temperature = ADCL;
      temperature |= ADCH << 8;

      blink_radix(temperature, 10);

      ADCSRA |= 1<<ADIF; // clear interrupt flag
      ADCSRA |= 1<<ADSC; // start next conversion
    }
  }

  return 1;
}
