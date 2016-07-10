
#include "sonar_trigger.h"

static float sonar_alpha = 0;
static float sonar_thresold = 5;

static int sonar_index = -1;
static int sonar_trigger = 0;

int SonarDev[MAX_DEV] = {0};
int SonarVal[MAX_DEV] = {0};
float SonarInt[MAX_DEV] = {0};
int SonarNorm[MAX_DEV] = {0};

void Sonar_Initialization(int tri_pin) {
	pinMode(tri_pin, OUTPUT);
	sonar_trigger = tri_pin;
}

int Sonar_Bind(int echo) {
	if (sonar_index++ < MAX_DEV) {
		SonarDev[sonar_index] = echo;
		pinMode(echo, INPUT);
		return sonar_index;
	}
	return -1;
}

void Sonar_Norm(int i) {
  if (SonarVal[i] > 0) {
    int tmp = abs(SonarVal[i] - SonarInt[i]);

    if (tmp > sonar_thresold)
      sonar_alpha = 0.15;
    else
      sonar_alpha = 0.95;

    SonarInt[i] = sonar_alpha * SonarInt[i]
        + (1 - sonar_alpha) * SonarVal[i];
    SonarNorm[i] = (int)(SonarInt[i]/10);
  }
}

void Sonar_update() {
    for(int i=0;i<=sonar_index;i++){
      digitalWrite(sonar_trigger, HIGH);delay(1);
      digitalWrite(sonar_trigger, LOW);
      SonarVal[i] = pulseIn(SonarDev[i], HIGH, 10E3);
      if(SonarVal[i]==0)  SonarVal[i] = 999;
      Sonar_Norm(i);
    }
}

void Sonar_print() {
  for (int i = 0; i <= sonar_index; i++) {
    Serial.print("Sonar");Serial.print(i);Serial.print(':');Serial.println((int)SonarNorm[i]);
    //Serial2.print("Sonar");Serial2.print(i);Serial2.print(':');Serial2.println((int)SonarNorm[i]);
  }
  Serial.println();
  //Serial2.println();
}

