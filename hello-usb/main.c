#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#include <util/delay.h>

#define LED_PIN PORTB0

#define abs(x) ((x) > 0 ? (x) : (-x))

// Called by V-USB after device reset
void hadUsbReset() {
  int frameLength, targetLength = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
  int bestDeviation = 9999;
  uchar trialCal, bestCal, step, region;

  DDRB |= _BV(LED_PIN); // LED is on PB0
  PORTB &= ~_BV(LED_PIN); // turn LED off

  // do a binary search in regions 0-127 and 128-255 to get optimum OSCCAL
  for(region = 0; region <= 1; region++) {
    frameLength = 0;
    trialCal = (region == 0) ? 0 : 128;

    for(step = 64; step > 0; step >>= 1) {
      if(frameLength < targetLength) // true for initial iteration
        trialCal += step; // frequency too low
      else
        trialCal -= step; // frequency too high

      OSCCAL = trialCal;
      frameLength = usbMeasureFrameLength();

      if(abs(frameLength-targetLength) < bestDeviation) {
        bestCal = trialCal; // new optimum found
        bestDeviation = abs(frameLength -targetLength);
      }
    }
  }

  OSCCAL = bestCal;
}

#define USB_LED_OFF 0
#define USB_LED_ON  1

// this gets called when custom control message is received
USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (void *)data; // cast data to correct

  switch(rq->bRequest) { // custom command is in the bRequest field
  case USB_LED_ON:
    PORTB |= _BV(LED_PIN); // turn LED on
    return 0;
  case USB_LED_OFF:
    PORTB &= ~_BV(LED_PIN); // turn LED off
    return 0;
  }

  return 0; // should not get here
}



int main() {
  uchar i;

  DDRB |= _BV(LED_PIN); // LED is on PB0
  PORTB |= _BV(LED_PIN);

  // set up a 1 second watchdog timer that resets the microcontroller
  // if 1000 milliseconds pass without a call to wdt_reset()
  wdt_enable(WDTO_1S);

  // init v-usb
  usbInit();

  usbDeviceDisconnect(); // enforce re-enumeration
  for(i = 0; i < 250; i++) { // wait 500 ms
    wdt_reset(); // keep the watchdog happy
    _delay_ms(2);
  }
  usbDeviceConnect();

  sei(); // Enable interrupts after re-enumeration

  while(1) {
    wdt_reset(); // keep the watchdog happy
    usbPoll();
  }

  return 0;
}
