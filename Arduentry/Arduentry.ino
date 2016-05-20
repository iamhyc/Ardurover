
int rc[16] = {0};

void setup()
{
  PinMode_Initilization();

  HMC_Initialization();//I2C
  GPS_Initialization();//Serial1

  SBUS_Initilization();//Serial3

  Serial.begin(9600);//Serial0 Debug
}

void loop()
{
 //getrc();
 //control();
 }