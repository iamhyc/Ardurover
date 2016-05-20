#include "PinDef.h"
#include "chasis.h"
#define MLOW 0.0

AWheel w1 = {w1n, w1p, w1e, 1.00};
AWheel w2 = {w2n, w2p, w2e, 1.00};
AWheel w3 = {w3n, w3p, w3e, 1.00};
AWheel w4 = {w4n, w4p, w4e, 1.00};

void Wheel_Ctrl(AWheel wl, float val) {
	char dir = -1;
	if (val > MLOW)	dir = 1;

	switch(dir) {
		case 0:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, 0);
			break;
		case 1:
			digitalWrite(wl.nPin, HIGH);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, val*wl.para);
		case -1:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, HIGH);
			analogWrite(wl.ENPin, val*wl.para);
	}
}

void TransMove(float vf, float vtr, float vr) {
  
	Wheel_Ctrl(w1, vf - vtr - vr);
	Wheel_Ctrl(w2, vf + vtr + vr);
	Wheel_Ctrl(w3, vf - vtr + vr);
	Wheel_Ctrl(w4, vf + vtr - vr);
}
