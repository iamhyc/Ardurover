
#include "Chasis_RM.h"
#include "sonar_trigger.h"
#include "SBUS_Ctrl.h"
#include "function.h"
#include "PinDef.h"

int rc[16] = {0};
bool RM_DRV = true;

void setup()
{
  Serial.begin(115200);//Serial Debug
  Serial2.begin(9600); //Serial2 BT
  
  SBUS_Initilization();//Serial3, 100000, Interrupt
  
  Sonar_Initialization(22);//trigger
  
  Sonar_Bind(24);//0, forward
  Sonar_Bind(25);//1, left
  Sonar_Bind(26);//2, backward
  Sonar_Bind(27);//3, right

  //Soanr_Setup();
  
  CAN_DRV_Initialization();//CAN_1000KBPS
  
  //loop0();
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
  //TransMove_RM(rc[1], rc[0], rc[3]);
  AutoMove(rc[4]);
  //AutoMove(1);
 }
