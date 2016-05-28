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

  Serial.begin(115200);
  Serial2.begin(9600);//Serial BT Debug
  
}

void loop()
{
  SBUS_update();
  HMC_update();
  SBUS_Normlize();

  //HMC_print();
  //TransMove(200, 0, 0);
  AutoMove(rc[6]);
  //wheel_calib(rc[2]);
  //wheel_test();
 }
