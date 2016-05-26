#include "chasis.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int rc[16] = {0};

void setup()
{
  PinMode_Initilization();

  HMC_Initialization();//I2C
  GPS_Initialization();//Serial1,9600

  SBUS_Initilization();//Serial3,100000

  Serial2.begin(9600);//Serial BT Debug
  Serial.begin(115200);
  
  //Auto_Initialization();
}

void loop()
{
  SBUS_update();
  HMC_update();
  SBUS_Normlize();

  HMC_print();
  delay(50);
  TransMove(200, 0, 0);
  //TransMove(rc[1], rc[0], rc[3]);
  /*if(rc[7] > 0)
    Transmove(rc[1], rc[0], rc[3]);
  else
    automove();*/

  //wheel_calib(rc[2]);
  //wheel_test();
  //HMC_print();
  //control();
 }
