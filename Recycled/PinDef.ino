
/***********Chasis Wheel Pin Define************/
#include "PinDef.h"


/**************Other Pin Define****************/


void PinMode_Initilization() {
	/*pinMode(w1n, OUTPUT);	pinMode(w1p, OUTPUT);	pinMode(w1e, OUTPUT);
	pinMode(w2n, OUTPUT);	pinMode(w2p, OUTPUT);	pinMode(w2e, OUTPUT);
	pinMode(w3n, OUTPUT);	pinMode(w3p, OUTPUT);	pinMode(w3e, OUTPUT);
	pinMode(w4n, OUTPUT);	pinMode(w4p, OUTPUT);	pinMode(w4e, OUTPUT);*/

	pinMode(LED_RED, OUTPUT);
}

void LED_RED_ON() {
	digitalWrite(LED_RED, HIGH);
}

void LED_RED_OFF() {
	digitalWrite(LED_RED, LOW);
}

void LED_RED_BLINK(int interval) {
	LED_RED_ON();
	delay(interval);
	LED_RED_OFF();
	delay(interval);
}
