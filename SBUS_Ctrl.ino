
#include <SBUS.h>
#include "SBUS_Ctrl.h"

extern int rc[16];

SBUS sbus(Serial3);
ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

void SBUS_Initilization() {
	sbus.begin();
}

void SBUS_update() {
	//if(num < 1 || num > 16) num = 16;
	for (int i = 0; i < 16; i++)
		rc[i] = sbus.getNormalizedChannel(i+1);
}

char norm_sgn2(int val) {
	switch(val) {
		case S2L:return -1;
		case S2H:return +1;
		default:return 0;
	}
}

char norm_sgn3(int val) {
	switch(val) {
		case S3M:return 0;
		case S3H:return +1;
		case S3L:return -1;
		default:return 0;
	}
}

int norm_sgn255(int ls, int ms, int hs, int val) {
	val = (val - ms) ;
	if (val > 0)
		val = val / (hs-ms) * 255;
	else
		val = val / (ms-ls) * 255;
	return __fix(val);
}

void SBUS_Normlize() {
	//ch1, ch2, ch3, ch4...
	rc[0] = norm_sgn255(R3L_CH1, R3M_CH1, R3H_CH1, rc[0]);
	rc[1] = norm_sgn255(R3L_CH2, R3M_CH2, R3H_CH2, rc[1]);
	rc[2] = norm_sgn255(R3L_CH3, R3M_CH3, R3H_CH3, rc[2]);
	rc[3] = norm_sgn255(R3L_CH4, R3M_CH4, R3H_CH4, rc[3]);
	//ch5, ch7...
	rc[4] = norm_sgn3(rc[4]);
	rc[6] = norm_sgn3(rc[6]);
}

void SBUS_print() {
	for (int i = 0; i < 16; i++) {
    char* str;
    sprintf(str, "Ch%d ", i+1);
		Serial.print(str);
		Serial.println(rc[i]);
	}

	Serial.print("Failsafe: ");
	if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_ACTIVE) {
		Serial.println("Active");
	}
	if (sbus.getFailsafeStatus() == SBUS_FAILSAFE_INACTIVE) {
		Serial.println("Not Active");
	}
	
	Serial.print("Time diff: ");
	Serial.println(millis() - sbus.getLastTime());
  Serial.print("\n\n\n");
}

