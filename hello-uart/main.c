#include <util/delay.h>

#include "softuart.h"

// Forward declaration with 'noreturn' attribute
void main() __attribute__ ((noreturn));

void ledInit()
{
  LED_DDR |= _BV(LED_PIN); // data direction to output
}

void ledOn()
{
  LED_PORT |= _BV(LED_PIN); // Turn LED on
}

void ledOf()
{
  LED_PORT &= ~_BV(LED_PIN); // Turn LED off
}

void main() {

  uartInit();
  ledInit();

  while(1) {
    uartSend('O');
    uartSend('H');
    uartSend('I');
    uartSend('!');
    uartSend('\n');
    uartSend('\r');

    ledOn();
    _delay_ms(500);
    ledOf();
    _delay_ms(500);
  }
}
