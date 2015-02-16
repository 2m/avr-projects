#ifndef __HARDWARE_H
#define __HARDWARE_H

#include <avr/io.h>

#define LED_DDR DDRB // led data direction register
#define LED_PORT PORTB
#define LED_PIN PORTB3

/**
 * This overrides definitions in hardware.h of the tinytemplate library.
 */

#define UART_ENABLED
#define BAUD_RATE 115200

// single pin UART
#define UART_TX PINB4
#define UART_RX PINB4

#endif /* __HARDWARE_H */
