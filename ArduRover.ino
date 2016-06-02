#include "chasis.h"
#include "Chasis_RM.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int rc[16] = {0};
bool RM_DRV = true;

void setup()
{
  PinMode_Initilization();

  HMC_Initialization();//I2C

  GPS_Initialization();//Serial1, 9600,   Interrupt
  SBUS_Initilization();//Serial3, 100000, Interrupt
  Interrupt_Initialization();

  Serial.begin(115200);//Serial Debug
  Serial2.begin(9600);//Serial BT Debug
  
  CAN_DRV_Initialization();//CAN_1000KBPS
}

void loop()
{
  SBUS_update();
  //HMC_update();
  //SBUS_Normlize();
  SBUS_print();
  //HMC_print();
  //TransMove(200, 0, 0);
  //AutoMove(rc[6]);
  AutoMove(-1);
  //wheel_calib(rc[2]);
  //wheel_test();
 }
