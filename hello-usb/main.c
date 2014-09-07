#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv.h"

#include <util/delay.h>

#define USB_LED_OFF 0
#define USB_LED_ON  1

// this gets called when custom control message is received
USB_PUBLIC uchar usbFunctionSetup(uchar data[8]) {
    usbRequest_t *rq = (void *)data; // cast data to correct

    switch(rq->bRequest) { // custom command is in the bRequest field
    case USB_LED_ON:
        PORTB |= 1; // turn LED on
        return 0;
    case USB_LED_OFF:
        PORTB &= ~1; // turn LED off
        return 0;
    }

    return 0; // should not get here
}

int main() {
    uchar i;

    DDRB = 1; // LED is on PB0

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