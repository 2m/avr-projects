/**
* Poor man's IO library.
* Note: set PORTB[0] to output before using (DDRB |= 1;)
**/

#ifndef _PMIO_H_
#define _PMIO_H_ 1

// blink n times
void blink(int n);
// call blink(int n) for every digit
void blink_radix(int val, int radix);
// light an LED with (0 < val < 1) brightness for hold_ms with PWM
void light(float val, long hold_ms);

#endif /* _PMIO_H_ */
