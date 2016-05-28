
#include <Wire.h>
#include <JY901.h>
#include <HMC5983.h>
#include "Ublox.h"
#include "PinDef.h"
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
static float HMC_alpha = 0.95;//0.15


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
			HMC_alpha = 0.15;
		else
			HMC_alpha = 0.95;

		HMC_angle = HMC_alpha * HMC_angle 
				+ (1- HMC_alpha) * rs;
	}
}

float HMC_getAngle() {
	return HMC_angle;
}

void HMC_print() {
  float c = HMC_getAngle();
  Serial.print(c);Serial.print('\n');
  Serial2.print(c);Serial2.print('\n');
}

/***************Ublox GPS*************************/
#define GPS_BAUD 9600
#define EARTH_RADIUS 6378.137;
static float GPS_latitude;
static float GPS_Longitude;

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

float GPS_get(String ins) {
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
	int sats = (int)GPS_get("SU");
	if((sats == NULL) || (sats < 6)){
		return false;
	}
	//with a filter

	return true;
}

float GPS_calc(float lat1, float lng1, float lat2, float lng2) {
	lat1 = lat1 * DEG_TO_RAD;
   	lat2 = lat2 * DEG_TO_RAD;

   	float lat_del = sin((lat1 - lat2)/2);
   	float lng_del = sin((lng1 - lng2) * DEG_TO_RAD/2);

   	float s = 2 * asin( sqrt( lat_del*lat_del + cos(lat1)*cos(lat2)*lng_del*lng_del ) );

   	s = s * EARTH_RADIUS;
   	s = round(s * 10000) / 10000;
   	return s;
}

void GPS_print() {
	Serial.print("ALT:");Serial.println(M8_Gps.altitude, 6);
	Serial.print("LAT:");Serial.println(M8_Gps.latitude, 6);
	Serial.print("LNT:");Serial.println(M8_Gps.longitude, 6);
	Serial.print("SU");Serial.println(M8_Gps.sats_in_use, 6);
	Serial.print("SV");Serial.println(M8_Gps.sats_in_view, 6);
	Serial.println();
}

