
#include "chasis.h"
#include "Chasis_RM.h"
#include "SBUS_Ctrl.h"
#include "GPS_KALMAN.h"
#include "function.h"

int init_location[] = {0, 0, 0};//Latitude, Longtitude, Yaw
//static bool locate_flag = false;
//static bool calib_flag = false;

extern int rc[16];
extern float Xstate[4][1];

void Auto_Initialization() {
  //GPS_print();
	if(!GPS_Locate()) {
		LED_RED_BLINK(500);
	}
	else {
		init_location[0] = Xstate[0][1];//GPS_get("LAT");
		init_location[1] = Xstate[1][1];//GPS_get("LNG");
		init_location[2] = HMC_getAngle();

    Serial.print(init_location[0]);Serial.print(" ");
		Serial.print(init_location[1]);Serial.print(" ");
		Serial.println(init_location[2]);
		
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
	if(!GPS_Locate()) return;
  	float current_lat = Xstate[0][1];
  	float current_lng = Xstate[1][1];
		
	return;
}
