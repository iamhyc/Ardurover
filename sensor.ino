
#include <Wire.h>
#include <JY901.h>
#include <HMC5983.h>
#include "Ublox.h"
#include "function.h"
//#include <Servo.h>

/********************JY901************************/
void JY901_Initilization() {
	//I2C communication
	JY901.StartIIC();
}

void JY901_update() {
	JY901.GetAcc();
	JY901.GetGyro();
	JY901.GetAngle();
}

float JY901_getAcc(int num) {
	if (num > 2 || num < 0)	return -1;
	return (float)JY901.stcAcc.a[num]/32768*16;
	//Acceleration Kalman Filter
}

float JY901_getAngle(int num) {
	if (num > 2 || num < 0)	return -1;
	return (float)JY901.stcAngle.Angle[num]/32768*180;
	//Angle Kalman Filter
}

float JY901_getGyro(int num) {
	if (num > 2 || num < 0)	return -1;
	return (float)JY901.stcGyro.w[num]/32768*2000;
	//Gyro Kalman Filter
}

void JY901_print() {
	Serial.print("Acc:");
	Serial.print((float)JY901.stcAcc.a[0]/32768*16);Serial.print(" ");
	Serial.print((float)JY901.stcAcc.a[1]/32768*16);Serial.print(" ");
	Serial.println((float)JY901.stcAcc.a[2]/32768*16);
	Serial.print("Gyro:");
	Serial.print((float)JY901.stcGyro.w[0]/32768*2000);Serial.print(" ");
	Serial.print((float)JY901.stcGyro.w[1]/32768*2000);Serial.print(" ");
	Serial.println((float)JY901.stcGyro.w[2]/32768*2000);
	Serial.print("Angle:");
	Serial.print((float)JY901.stcAngle.Angle[0]/32768*180);Serial.print(" ");
	Serial.print((float)JY901.stcAngle.Angle[1]/32768*180);Serial.print(" ");
	Serial.println((float)JY901.stcAngle.Angle[2]/32768*180);
	Serial.println();
}


/****************HMC5983**************************/
HMC5983 compass;
static const float HMC_thresold = 1.0;
static float HMC_angle = 0.0;
static float HMC_alpha = 0.90;


void HMC_Initialization() {
	//I2C communication
	compass.begin();
  	delay(50);
  	HMC_angle = compass.read();
}

void HMC_update() {//Synchronous
	float rs = -999;
	rs = compass.read();
	//with lowpass filter
	if (rs != -999) {
		float tmp = rot_abs(rs - HMC_angle);
		if (tmp > HMC_thresold)
			HMC_alpha = 0.20;
		else
			HMC_alpha = 0.90;

		HMC_angle = HMC_alpha * HMC_angle 
				+ (1- HMC_alpha) * rs;
	}
}

float HMC_getAngle() {
	return HMC_angle;
}

void HMC_print() {
  float c = HMC_getAngle();
  Serial.println(c);
  Serial2.println(c);
}

/***************Ublox GPS*************************/
#define GPS_BAUD 9600

Ublox M8_Gps;
// Altitude - Latitude - Longitude - N Satellites

void GPS_Initialization() {
	Serial1.begin(GPS_BAUD);
}

void GPS_update() {//Synchronous
	while(Serial1.available()){
		M8_Gps.encode(Serial1.read());
	}
}

float GPS_Locate(String ins) {
	if (ins=="LAT"||ins=="latitude"||ins=="Latitude")
		return M8_Gps.latitude;//
	if (ins=="LNG"||ins=="longitude"||ins=="Longitude")
		return M8_Gps.longitude;//
	if (ins=="speed"||ins=="Speed")
		return M8_Gps.speed;//
	//if (ins=="time"||ins=="timestamp")
		//return M8_Gps.datetime.timestamp;//
	if (ins=="SU"||ins=="sats_in_use")
		return M8_Gps.sats_in_use;//
	if (ins=="SV"||ins=="sats_in_view")
		return M8_Gps.sats_in_view;//

	if (ins=="alt"||ins=="ALT"||ins=="altitude"||ins=="Altitude")
		return M8_Gps.altitude;//

	return -1;
}

bool GPS_Locate_pc(){
	while(GPS_Locate("SU") < 7){
		delay(1000);
	}
	//with kalman filter
	return true;
}

void GPS_print() {
	Serial.print("ALT:");Serial.println(M8_Gps.altitude, 6);
	Serial.print("LAT:");Serial.println(M8_Gps.latitude, 6);
	Serial.print("LNT:");Serial.println(M8_Gps.longitude, 6);
	Serial.print("SU");Serial.println(M8_Gps.sats_in_use, 6);
	Serial.print("SV");Serial.println(M8_Gps.sats_in_view, 6);
	Serial.println();
}

