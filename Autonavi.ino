#include "chasis.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int init_location[] = {0, 0};//Latitude, Longtitude

extern int rc[16];

void Auto_Initialization() {
	while(!GPS_Locate_pc());
	init_location[0] = GPS_Locate("LAT");
	init_location[1] = GPS_Locate("LNT");
}
