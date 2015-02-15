#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT PORTB3

int main() {
  DDRB |= 1 << LED_PORT; // data direction to output

  while(1) {
    PORTB |= 1 << LED_PORT; // Turn LED on
    _delay_ms(500);
    PORTB &= ~(1 << LED_PORT); // Turn LED off
    _delay_ms(500);
  }

  return 1;
}
