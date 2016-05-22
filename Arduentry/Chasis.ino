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

void TransMove(float vf, float vtr, float vrr) {

	int top_speed=maxs(abs(vf),maxs(abs(vtr),abs(vrr)));
	int vt=abs(vf)+abs(vtr)+abs(vrr);
	if (vt!=0)
	{
		vf = (vf*top_speed)/vt;
		vtr = (vtr*top_speed)/vt;
		vrr = (vrr*top_speed)/vt;
	}
	Wheel_Ctrl(w1, vf - vtr - ROTP * vrr);
	Wheel_Ctrl(w2, vf + vtr + ROTP * vrr);
	Wheel_Ctrl(w3, vf - vtr + ROTP * vrr);
	Wheel_Ctrl(w4, vf + vtr - ROTP * vrr);
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
  delay(2000);
  Wheel_Ctrl(w1, 0);
  
  Wheel_Ctrl(w2, num);
  delay(2000);
  Wheel_Ctrl(w2, 0);
  
  Wheel_Ctrl(w3, num);
  delay(2000);
  Wheel_Ctrl(w3, 0);
  
  Wheel_Ctrl(w4, num); 
  delay(2000);
  Wheel_Ctrl(w4, 0); 

  delay(4000);
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


