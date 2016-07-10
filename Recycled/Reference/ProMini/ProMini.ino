// The Arduino Pro Mini is a simple & cheap board, ideal for leaving inside of
// your model. It only has a single Serial port, and the default SBUS code
// doesn't run on it. You can get it to work by not using the timer, but that
// means you can't use delay in the loop() function anymore.

#include <SBUS.h>
#include <Wire.h>
#include <JY901.h>
#include <HMC5983.h>
//#include <Servo.h> 
#include "Ublox.h"
#define SERIAL_BAUD 115200
#define GPS_BAUD 9600
#define N_FLOATS 4
#define BS_BAUD 9600
HMC5983 compass;
Ublox M8_Gps;
SBUS sbus(Serial3);
int motordirection[]={47,48,49,50,10,11,12,13};
float gpsArray[N_FLOATS] = {0, 0, 0, 0};
int speed[]={2,3,8,9};
int speedcontrol[]={0,0,0,0};
int i=0;
int rc[]={0,0,0,0,0};
float pidY[]={-20,0,0};
float error=0;
float errorold=0;
float errorsum=0;
float errordif=0;
float mag=0;
float magold=0;
float target=0;
int count1=0;
void setup()
{
   Serial2.begin(BS_BAUD);
   Serial.begin(SERIAL_BAUD);
   Serial1.begin(GPS_BAUD);
   compass.begin(); 
 // JY901.StartIIC();
  sbus.begin(false);
  for(i=0;i<8;i++)
  {
  pinMode(motordirection[i],OUTPUT);
  digitalWrite(motordirection[i],LOW);
  }

      for(i=0;i<4;i++)
  {
  pinMode(speed[i],OUTPUT);
  digitalWrite(speed[i],LOW);
  }
  getmag();
  target=mag;
}

void loop()
{
  getmag();
  Serial.println(mag, 4);
 //getrc();
 //control();
 /*if(count1 == 4){
  Serial2.print(pidY[0]);
  Serial2.write('     ');
 Serial2.print(target);
 Serial2.write('     ');
 Serial2.println(mag);
 delay(50)*/;
 }
// Serial2.println('mag:'+mag);
 
//motorcontrol(3,230);
//delay(4000); 
 //JY901.GetAngle();
  //Serial.print("Angle:");Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
  //delay(500);

int fix(int speedoutput)
{
   if(speedoutput>255)
   speedoutput=255;
   if(speedoutput<-255)
   speedoutput=-255;
   return speedoutput;   
}
void getmag()
{
  int count=0;
  float sum=0;
  while(1)
  {
    mag=-999;
    mag=compass.read();
    if(mag != -999 )
    {
      count ++;
      sum+=mag;
      if(count>=9)
     {
      mag=sum/10;
      break;
      }
    }
  }
}
void getrc()
{
  sbus.process();
  for(i=0;i<5;i++)
  {
    rc[i]= sbus.getChannel(i+1);
    
    if(rc[i]>=2000||rc[i]<=300)
    rc[i]=1000;
    if(i==0)
    rc[i]=(int)map(rc[i],330,1680,-255,255)+1;
    if(i==1)
    rc[i]=(int)map(rc[i],330,2000,-255,255)+34;
    if(i==2)
    rc[i]=(int)map(rc[i],330,1680,0,20);
    if(i==3)
    rc[i]=(int)map(rc[i],330,1680,-255,255)-7;
   // Serial.println(rc[i]);
  }
  
}
void motorcontrol(int motor,int speedcontrol)
{

  if (speedcontrol>0){
  digitalWrite(motordirection[motor*2+1],LOW);
  digitalWrite(motordirection[motor*2],HIGH);
  }
  else
  {
   digitalWrite(motordirection[motor*2+1],HIGH);
   digitalWrite(motordirection[motor*2],LOW);
  }
  analogWrite(speed[motor],abs(speedcontrol));
}

void control()
{
  pidY[0]=rc[2];
  if(abs(rc[3])>40)
  {
    
      speedcontrol[0]=fix(-rc[3]);
      speedcontrol[1]=rc[3];
      speedcontrol[2]=-rc[3];
      speedcontrol[3]=rc[3];
      for(i=0;i<4;i++)
     motorcontrol(i,speedcontrol[i]);

       
    }
    
  else
  {
  if(abs(rc[0])<=40)
  {
    count1++;
  //magold=mag;
  if(count1 == 5)
  {
  errorold=error;
  getmag();
  error=mag-target;
  errorsum+=error;
  errordif +=(error-errorold);
  count1=0;
  }
    speedcontrol[0]=fix(rc[1]-((int)(rc[0]/1.2))+pidY[0]*error+pidY[1]*errorsum+pidY[2]*errordif);
  speedcontrol[1]=speedcontrol[0];
  speedcontrol[2]=fix(rc[1]+ ((int)(rc[0]/1.2))-pidY[0]*error-pidY[1]*errorsum-pidY[2]*errordif);
  speedcontrol[3]=speedcontrol[2];
  }
  else
  {
  speedcontrol[0]=fix(rc[1]-((int)(rc[0]/1.2)));
  speedcontrol[1]=speedcontrol[0];
  speedcontrol[2]=fix(rc[1]+ ((int)(rc[0]/1.2)));
  speedcontrol[3]=speedcontrol[2];
  getrc();
  if(rc[0]>=40)
  {
  error=0;
  errorold=0;
  getmag();
  target=mag;
  errorsum=0;
  errordif=0;
  }
  }for(i=0;i<4;i++)
motorcontrol(i,speedcontrol[i]);
  }
}

/*void forward()
{
  digitalWrite(a[0],HIGH);
  digitalWrite(a[1],LOW);
  digitalWrite(a[3],HIGH);
  digitalWrite(a[2],LOW);
  digitalWrite(b[0],HIGH);
  digitalWrite(b[1],LOW);
  digitalWrite(b[2],HIGH);
  digitalWrite(b[3],LOW);
  speedcontrol[0]=fix(rc[1]+((int)(rc[0]/2)));
  speedcontrol[1]=fix(rc[1]+((int)(rc[0]/2)));
  speedcontrol[2]=fix(rc[1]-((int)(rc[0]/2)));
  speedcontrol[3]=fix(rc[1]-((int)(rc[0]/2)));
  for(i=0;i<4;i++)
  analogWrite(speed[i],speedcontrol[i]);
}
void backward()
{
  digitalWrite(a[1],HIGH);
  digitalWrite(a[0],LOW);
  digitalWrite(a[2],HIGH);
  digitalWrite(a[3],LOW);
  digitalWrite(b[1],HIGH);
  digitalWrite(b[0],LOW);
  digitalWrite(b[3],HIGH);
  digitalWrite(b[2],LOW);
  
  speedcontrol[0]=fix(abs(rc[1])+((int)(rc[0]/3)));
  speedcontrol[1]=fix(abs(rc[1])+((int)(rc[0]/3)));
  speedcontrol[2]=fix(abs(rc[1])-((int)(rc[0]/3)));
  speedcontrol[3]=fix(abs(rc[1])-((int)(rc[0]/3)));
  for(i=0;i<4;i++)
  analogWrite(speed[i],speedcontrol[i]);
}
void right()
{
  digitalWrite(a[1],HIGH);
  digitalWrite(a[0],LOW);
  digitalWrite(a[3],HIGH);
  digitalWrite(a[2],LOW);
  digitalWrite(b[1],HIGH);
  digitalWrite(b[0],LOW);
  digitalWrite(b[2],HIGH);
  digitalWrite(b[3],LOW);
}
void left()
{
  digitalWrite(a[0],HIGH);
  digitalWrite(a[1],LOW);
  digitalWrite(a[2],HIGH);
  digitalWrite(a[3],LOW);
  digitalWrite(b[0],HIGH);
  digitalWrite(b[1],LOW);
  digitalWrite(b[3],HIGH);
  digitalWrite(b[2],LOW);
}

void turnleft()
{
  digitalWrite(a[0],HIGH);
  digitalWrite(a[1],LOW);
  digitalWrite(a[3],HIGH);
  digitalWrite(a[2],LOW);
  digitalWrite(b[1],HIGH);
  digitalWrite(b[0],LOW);
  digitalWrite(b[3],HIGH);
  digitalWrite(b[2],LOW);
}

void turnright()
{
  digitalWrite(a[1],HIGH);
  digitalWrite(a[0],LOW);
  digitalWrite(a[2],HIGH);
  digitalWrite(a[3],LOW);
  digitalWrite(b[0],HIGH);
  digitalWrite(b[1],LOW);
  digitalWrite(b[2],HIGH);
  digitalWrite(b[3],LOW);
}*/
void right()
{
  digitalWrite(motordirection[0],LOW);
  digitalWrite(motordirection[1],HIGH);
  digitalWrite(motordirection[2],LOW);
  digitalWrite(motordirection[3],HIGH);
  digitalWrite(motordirection[4],LOW);
  digitalWrite(motordirection[5],HIGH);
  digitalWrite(motordirection[6],LOW);
  digitalWrite(motordirection[7],HIGH);
}
void left()
{
  digitalWrite(motordirection[1],LOW);
  digitalWrite(motordirection[0],HIGH);
  digitalWrite(motordirection[3],LOW);
  digitalWrite(motordirection[2],HIGH);
  digitalWrite(motordirection[5],LOW);
  digitalWrite(motordirection[4],HIGH);
  digitalWrite(motordirection[7],LOW);
  digitalWrite(motordirection[6],HIGH);
}

void stop()
{
  digitalWrite(motordirection[0],LOW);
  digitalWrite(motordirection[1],LOW);
  digitalWrite(motordirection[2],LOW);
  digitalWrite(motordirection[3],LOW);
  digitalWrite(motordirection[4],LOW);
  digitalWrite(motordirection[5],LOW);
  digitalWrite(motordirection[6],LOW);
  digitalWrite(motordirection[7],LOW);
}

void getGPS()
{
     if(!Serial1.available())
    return;

  while(Serial1.available()){
        char c = Serial1.read();
         if (M8_Gps.encode(c)) {
          gpsArray[0] = M8_Gps.altitude;
          gpsArray[1] = M8_Gps.latitude;
          gpsArray[2] = M8_Gps.longitude; 
          gpsArray[3] = M8_Gps.sats_in_use;
        }
  }
}


