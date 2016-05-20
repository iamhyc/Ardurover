#define MLOW 1.0

extern int rc[16];
typedef struct
{
  short pPin;
  short nPin;
  short ENPin;
  float para;
}AWheel;

const AWheel[5] wl;
wl[1] = {w1n, w1p, w1e, 1.00};
wl[2] = {w2n, w2p, w2e, 1.00};
wl[3] = {w3n, w3p, w3e, 1.00};
wl[4] = {w4n, w4p, w4e, 1.00};


void Wheel_Ctrl(AWheel wl, float val) {
	char dir;
	if (val > MLOW)
		dir = 1;
	else
		dir = -1;

	if (abs(val) < MLOW)
		dir = 0;

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
	Wheel_Ctrl(wl[1], vf - vtr - vr);
	Wheel_Ctrl(wl[2], vf + vtr + vr);
	Wheel_Ctrl(wl[3], vf - vtr + vr);
	Wheel_Ctrl(wl[4], vf + vtr - vr);
}