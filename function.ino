
#include "function.h"

int __fix(int speedoutput)
{
   if(speedoutput>255)
   speedoutput=255;
   if(speedoutput<-255)
   speedoutput=-255;
   return speedoutput;   
}

float rot_abs(float deg) {
	float tmp = abs(deg);
	if (deg > 180)
		return (360 - deg);
	else
		return deg;
}