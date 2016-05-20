
#include <SBUS.h>

SBUS sbus(Serial3);

extern int rc[16];

ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

void SBUS_Initilization() {
	sbus.begin();
}

void SBUS_update(int num) {
	for (int i = 0; i < 16; i++)
		rc[i] = sbus.getNormalizedChannel(i+1);
}