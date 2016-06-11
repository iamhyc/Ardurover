
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

bool back_flag = false;

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
			if(!back_flag)
				Wheel_Stop();
			Naviback();
		break;
	}
}

void Naviback() {
	back_flag = true;
	if(!GPS_Locate()) return;

	float current_angle = HMC_getAngle();
  	float delta_lat = Xstate[0][1] - init_location[0];
  	float delta_lng = Xstate[1][1] - init_location[1];
  	float delta_agl = atan(delta_lat / delta_lng);//delta_y/delta_x
  	//clockwise as forword
  	float target_angle = (delta_lat<0?(delta_agl - 15):(165 - delta_agl));
  	target_angle = rot_fix(target_angle);

  	float vrr_ctl = TransMove_Control(current_angle, target_angle);

  	TransMove_RM(255, 0, vrr_ctl);

	return;
}
