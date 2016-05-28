#ifndef _PinDef_H_
#define _PinDef_H_

#define w1n  52	
#define w1p  53
#define w1e  2

#define w2n  30	
#define w2p  31
#define w2e  7

#define w3n  32	
#define w3p  33
#define w3e  8

#define w4n  47	
#define w4p  48
#define w4e  3

#define LED_RED 28

void LED_RED_ON(void);
void LED_RED_OFF(void);
void LED_RED_BLINK(int);

#endif
