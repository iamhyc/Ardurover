#include "chasis.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int init_location[] = {0, 0};//Latitude, Longtitude

extern int rc[16];

void Auto_Initialization() {
	LED_RED_ON();
	delay(500);
	LED_RED_OFF();
	delay(500);
	LED_RED_ON();
	

	init_location[0] = GPS_get("LAT");
	init_location[1] = GPS_get("LNT");
}

void AutoMove(char sw) {
	switch(sw) {
		case 0:
			Transmove(rc[1], rc[0], rc[3]);
		break;
		case 1:
			GPS_Locate_pc();//for loop until
		break;
		case -1:
			Naviback();
		break;
	}
}

void Naviback() {
	return;
}