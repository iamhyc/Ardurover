#include "chasis.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int init_location[] = {0, 0, 0};//Latitude, Longtitude
//static bool locate_flag = false;
//static bool calib_flag = false;

extern int rc[16];

void Auto_Initialization() {
	GPS_update();

	if(!GPS_Locate_pc()) {
		LED_RED_BLINK(500);
	}
	else {
		init_location[0] = GPS_get("LAT");
		init_location[1] = GPS_get("LNT");
		init_location[2] = HMC_getAngle();
		LED_RED_BLINK(100);
		LED_RED_ON();
	}
}

void AutoMove(char sw) {
	switch(sw) {
		case 0:
			//delay(50);
			TransMove(rc[1], rc[0], rc[3]);
		break;
		case -1:
			Wheel_Stop();
			Auto_Initialization();//for loop with FSM
		break;
		case 1:
			Wheel_Stop();
			Naviback();
		break;
	}
}

void Naviback() {
	GPS_update();
	if(!GPS_Locate_pc()) return;
		
	return;
}