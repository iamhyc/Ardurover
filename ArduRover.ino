
/*	Rover Arduino Version
 *	This is the initialization part.
 *	Teamwork: 
 */

/* Libarary Include */
#include <SBUS.h>
#include <Wire.h>
#include <JY901.h>

/* User src Include */
#include "chasis.h"
#include "rc.h"

/* Pre-setup part */
SBUS sbus(Serial3);
// Use Timer2, Triggers ever 1ms
ISR(TIMER2_COMPA_vect)
{
  sbus.process();
}

void setup() {
  
  /*Chasis Pin Setup*/
  pinMode(w1n, OUTPUT); pinMode(w1p, OUTPUT); pinMode(w1e, OUTPUT);
  pinMode(w2n, OUTPUT); pinMode(w2p, OUTPUT); pinMode(w2e, OUTPUT);
  pinMode(w3n, OUTPUT); pinMode(w3p, OUTPUT); pinMode(w3e, OUTPUT);
  pinMode(w4n, OUTPUT); pinMode(w4p, OUTPUT); pinMode(w4e, OUTPUT);

  /*Remote Control Setup*/
  sbus.begin();
}

void loop() {

  
}
