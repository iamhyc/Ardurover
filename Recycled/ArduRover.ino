
#include "Chasis_RM.h"
#include "sonar_trigger.h"
#include "SBUS_Ctrl.h"
#include "function.h"

int rc[16] = {0};
bool RM_DRV = true;

void setup()
{
  Serial.begin(115200);//Serial Debug
  Serial2.begin(9600); //Serial2 BT
  
  SBUS_Initilization();//Serial3, 100000, Interrupt
  
  Sonar_Initialization(22);//trigger
  
  Sonar_Bind(21);//0, forward
  Sonar_Bind(20);//1, left
  Sonar_Bind(19);//2, backward
  Sonar_Bind(18);//3, right

  Soanr_Setup();
  
  CAN_DRV_Initialization();//CAN_1000KBPS
}

void loop()
{
  //Serial2.println("Hello World!");
  SBUS_update();
  SBUS_Normlize();
  //SBUS_print();
  Sonar_update();
  //Sonar_print();
  //delay(100);
  //AutoMove(rc[4]);
  AutoMove(1);
 }
