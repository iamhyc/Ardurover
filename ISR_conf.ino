#include <FlexiTimer2.h>
#include <SBUS.h>

extern SBUS sbus;
/*ISR(TIMER2_COMPA_vect)
{
  
}*/

void Interrupt_Initialization() {
	FlexiTimer2::set(10, 1.0/1000, Timer2Service);//per 10*1ms
	FlexiTimer2::start();
}

void Timer2Service() {
	sbus.process();
	GPS_update();
	HMC_update();
}
