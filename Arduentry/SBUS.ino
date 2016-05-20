
#include <SBUS.h>

#define S3H  160
#define S3M  100
#define S3L  60
#define S2H  160
#define S2L  60
#define RSH  160
#define RSM  100
#define RSL  60


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

void SBUS_Normlize() {
	//ch0, ch1, ch2...
}

void norm_sgn(int ls, int ms, int hs, int val) {
	
}