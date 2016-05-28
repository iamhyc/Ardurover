#include "PinDef.h"
#include "chasis.h"
#include "SBUS_Ctrl.h"
#include "function.h"

#define MLOW 0.0
#define ROTP 1.00//6.45

AWheel w1 = {w1n, w1p, w1e, 1.0000};//1.0000
AWheel w2 = {w2n, w2p, w2e, 1.6891};//1.6891
AWheel w3 = {w3n, w3p, w3e, 1.6891};//1.6891
AWheel w4 = {w4n, w4p, w4e, 1.7837};//1.7837

static float current_angle;
static float target_angle;

void Wheel_Ctrl(AWheel wl, float val) {
	char dir = -1;
	if (val > MLOW)	dir = 1;
	val = abs(__fix(val));
  
	switch(dir) {
		case 0:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, 0);
			break;
		case 1:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, HIGH);
			analogWrite(wl.ENPin, val*wl.para);
      break;
		case -1:
			digitalWrite(wl.nPin, HIGH);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, val*wl.para);
      break;
	}
}

void TransMove(float vfr, float vtr, float vrr) {

	int top_speed=maxs(abs(vfr),maxs(abs(vtr),abs(vrr)));
	int vt=abs(vfr)+abs(vtr)+abs(vrr);
	if (vt!=0)
	{
		vfr = (vfr*top_speed)/vt;
		vtr = (vtr*top_speed)/vt;
		vrr = (vrr*top_speed)/vt;
	}
	if (vrr == 0) {
		current_angle = HMC_getAngle();
		vrr = TransMove_Control(current_angle, target_angle);
	}
	else{
		target_angle = HMC_getAngle();
	}
	
	Wheel_Ctrl(w1, vfr - vtr - ROTP * vrr);
	Wheel_Ctrl(w2, vfr + vtr + ROTP * vrr);
	Wheel_Ctrl(w3, vfr - vtr + ROTP * vrr);
	Wheel_Ctrl(w4, vfr + vtr - ROTP * vrr);
	
}

float TransMove_Control(float current_angle, float target_angle)
{
    const float g_p = 20.0;
    const float g_i = 0.0;
    const float g_d = 0.0;
    
    static float error_g[3] = {0.0, 0.0, 0.0};
    static float vrr_out = 0;
    
    error_g[0] = error_g[1];
    error_g[1] = error_g[2];
    error_g[2] = target_angle - current_angle;
    
    vrr_out = error_g[2] * g_p 
			+ error_g[2] * g_i 
			+ (error_g[2] - error_g[1]) * g_d;

    return -__fix(vrr_out);
}

void Wheel_Stop() {
	Wheel_Ctrl(w1, 0);
	Wheel_Ctrl(w2, 0);
	Wheel_Ctrl(w3, 0);
	Wheel_Ctrl(w4, 0);
}

void wheel_test() {
  Serial.println("run");
  float num = 255;
  Wheel_Ctrl(w1, num);
  //delay(2000);
  //Wheel_Ctrl(w1, 0);
  
  Wheel_Ctrl(w2, num);
  //delay(2000);
  //Wheel_Ctrl(w2, 0);
  
  Wheel_Ctrl(w3, num);
  //delay(2000);
  //Wheel_Ctrl(w3, 0);
  
  Wheel_Ctrl(w4, num); 
  //delay(2000);
  //Wheel_Ctrl(w4, 0); 

  //delay(4000);
}

float this_val = 90.0;
void wheel_calib(float v) {
  Wheel_Ctrl(w2, +255);
  Wheel_Ctrl(w3, -255);
  
  this_val = this_val + 0.1 * norm_sgn3(rc[4]);
  float angle = HMC_getAngle();
  w3.para = this_val / 100 * w2.para;
  Serial2.println(angle, 4);delay(50);
  Serial2.println(this_val, 4);delay(50);
  delay(100);
}



