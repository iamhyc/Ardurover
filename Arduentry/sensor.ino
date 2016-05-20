
#include <JY901.h>
#include <HMC5983.h>
//#include <Servo.h> 
#include "Ublox.h"

/****************HMC5983****************/
HMC5983 compass;


/*************Ublox GPS*****************/
Ublox M8_Gps;
#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define N_FLOATS 4
#define BS_BAUD 9600
