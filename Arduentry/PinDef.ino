
/***********Chasis Wheel Pin Define************/
#define w1n  22	
#define w1p  23
#define w1e  3

#define w2n  24	
#define w2p  25
#define w2e  4

#define w3n  26	
#define w3p  27
#define w3e  5

#define w4n  28	
#define w4p  29
#define w4e  6

/**************Other Pin Define****************/


void PinMode_Initilization() {
	pinMode(w1n, OUTPUT);	pinMode(w1p, OUTPUT);	pinMode(w1e, OUTPUT);
	pinMode(w2n, OUTPUT);	pinMode(w2p, OUTPUT);	pinMode(w2e, OUTPUT);
	pinMode(w3n, OUTPUT);	pinMode(w3p, OUTPUT);	pinMode(w3e, OUTPUT);
	pinMode(w4n, OUTPUT);	pinMode(w4p, OUTPUT);	pinMode(w4e, OUTPUT);
}