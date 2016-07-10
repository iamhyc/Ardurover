
#include <Arduino.h>

String data;
boolean valid=false;

static String  
        GGAUTCtime,
        GGAlatitude,
        GGAlongitude,
        GPStatus,
        SatelliteNum,
        HDOPfactor,
        Height,
        PositionValid,
        RMCUTCtime,
        RMClatitude,
        RMClongitude,
        Speed,
        Direction,
        Date,
        Declination,
        Mode;

'''void setup(){ 
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
}'''


  while (Serial1.available()> 0){

      data = reader_start();

      if(data.equals("GPGGA")){
        GGAUTCtime = reader();

        GGAlatitude = reader();
        GGAlatitude += reader();

        GGAlongitude =reader();
        GGAlongitude += reader();

        GPStatus = reader();
        SatelliteNum = reader();
        HDOPfactor = reader();
        Height = reader();
        valid = true;
      }
      else if(data.equals("GPRMC")){
        RMCUTCtime = reader();
        PositionValid = reader();

        RMClatitude = reader();
        RMClatitude += reader();

        RMClongitude = reader();
        RMClongitude += reader();

        Speed = reader();
        Direction = reader();
        Date = reader();

        Declination = reader();
        Declination += reader();

        Mode = reader();
        valid = true;
      }
      else if(data.equals("GPGSA")){
        valid = false;
      }
      else if(data.equals("GPGSV")){
        valid = false;
      }
      else if(data.equals("GPVTG")){
        valid = false;
      }
      else{
        valid = false;
      }
    }

    if (valid && PositionValid=="A") Serial.println("Position Valid");
  }

void reader_start(String data){
  char tmp = Serial1.read();
  while (!Serial1.find('$')) delay(1);

  Serial1.readStringUntil('$');
  Serial1.readBytes(data, 5);
}

String reader(){
  String value="";
  int temp;

    while (Serial1.available() > 0){
      delay(2);
      temp=Serial1.read();

      if((temp == ',')||(temp == '*')||(temp == '\n')) {
        return (value.length()?value:"");
      }
      else{
        value+=char(temp);
      }
    }

}