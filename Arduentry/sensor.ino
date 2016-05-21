
#include <Wire.h>
#include <JY901.h>
#include <HMC5983.h>
#include "Ublox.h"
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
void HMC_Initialization() {
	//I2C communication
	compass.begin();
}

float HMC_getAngle() {//Synchronous
	float rs = -999;
	rs = compass.read();
	//with kalman filter
	if (rs != -999) return rs;
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
	if (ins=="lat"||ins=="LAT"||ins=="latitude"||ins=="Latitude")
		return M8_Gps.latitude;//
	if (ins=="lng"||ins=="LNG"||ins=="longitude"||ins=="Longitude")
		return M8_Gps.longitude;//
	if (ins=="SU"||ins=="sats_in_use")
		return M8_Gps.sats_in_use;//
	if (ins=="SV"||ins=="sats_in_view")
		return M8_Gps.sats_in_view;//
	if (ins=="alt"||ins=="ALT"||ins=="altitude"||ins=="Altitude")
		return M8_Gps.sats_in_view;//

	return -1;
}

void GPS_Locate_pc(){
	//with kalman filter
}

void GPS_print() {
	Serial.print("ALT:");Serial.println(M8_Gps.altitude, 6);
	Serial.print("LAT:");Serial.println(M8_Gps.latitude, 6);
	Serial.print("LNT:");Serial.println(M8_Gps.longitude, 6);
	Serial.print("SU");Serial.println(M8_Gps.sats_in_use, 6);
	Serial.print("SV");Serial.println(M8_Gps.sats_in_view, 6);
	Serial.println();
}
