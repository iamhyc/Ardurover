/***
This is the chasis control part. Based on Four Macanum Wheel
***/

#include "chasis.h"

/********************VARIABLE AREA**********************/
typedef struct WHEEL_STRUCT
{
	short pPin;
	short nPin;
	short ENPin;
	float para;
}AWheel;

static AWheel w1 = {w1n, w1p, w1e, 1.00};
static AWheel w2 = {w2n, w2p, w2e, 1.00};
static AWheel w3 = {w3n, w3p, w3e, 1.00};
static AWheel w4 = {w4n, w4p, w4e, 1.00};

/********************VARIABLE AREA**********************/


/*	Basic Control part.
 * 	Including the logic of H-Bridge;
 *	And wheel control with AWheel
 */
void Wheel_Ctrl(AWheel wl, direction dir, float val) {
	switch(dir) {
		case stay:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, LOW);
			break;
		case forward:
			digitalWrite(wl.nPin, HIGH);
			digitalWrite(wl.pPin, LOW);
			analogWrite(wl.ENPin, 255*val*wl.para);
		case backward:
			digitalWrite(wl.nPin, LOW);
			digitalWrite(wl.pPin, HIGH);
			analogWrite(wl.ENPin, 255*val*wl.para);
	}
}

/*	Wheel control function part.
 * 	Including the logic of H-Bridge;
 *	And wheel control with AWheel
 */
void Mov_Clear() {
	Wheel_Ctrl(w1, 0, 0);
	Wheel_Ctrl(w2, 0, 0);
	Wheel_Ctrl(w3, 0, 0);
	Wheel_Ctrl(w4, 0, 0);
}

void Mov_For(float val) {
	Wheel_Ctrl(w1, 1, val);
	Wheel_Ctrl(w2, 1, val);
	Wheel_Ctrl(w3, 1, val);
	Wheel_Ctrl(w4, 1, val);
}

void Mov_Back(float val) {
	Wheel_Ctrl(w1, -1, val);
	Wheel_Ctrl(w2, -1, val);
	Wheel_Ctrl(w3, -1, val);
	Wheel_Ctrl(w4, -1, val);
}

void Mov_Left(float val) {
	Wheel_Ctrl(w1, -1, val);
	Wheel_Ctrl(w2, 1, val);
	Wheel_Ctrl(w3, 1, val);
	Wheel_Ctrl(w4, -1, val);
}

void Mov_Right(float val) {
	Wheel_Ctrl(w1, 1, val);
	Wheel_Ctrl(w2, -1, val);
	Wheel_Ctrl(w3, -1, val);
	Wheel_Ctrl(w4, 1, val);
}

void Rot_Left() {
	Wheel_Ctrl(w1, 1, val);
	Wheel_Ctrl(w2, -1, val);
	Wheel_Ctrl(w3, 1, val);
	Wheel_Ctrl(w4, -1, val);
}

void Rot_Right() {
	Wheel_Ctrl(w1, 1, val);
	Wheel_Ctrl(w2, -1, val);
	Wheel_Ctrl(w3, 1, val);
	Wheel_Ctrl(w4, -1, val);
}
