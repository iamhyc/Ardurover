
static float sonar_alpha = 0;
static float sonar_thresold = 5;

static int sonar_index = -1;
static int sonar_trigger = 0;

int SonarDev[MAX_DEV] = {0};
long SonarVal[MAX_DEV] = {0};
long SonarInt[MAX_DEV] = {0};
long SonarNorm[MAX_DEV] = {0};

void Sonar_Initialization(int tri_pin) {
	pinMode(tri_pin, OUTPUT);
	sonar_trigger = tri_pin;
}

void Soanr_Setup() {
  attachInterrupt(2, SonarDetect0, RISING);
  attachInterrupt(3, SonarDetect1, RISING);
  attachInterrupt(4, SonarDetect2, RISING);
  attachInterrupt(5, SonarDetect3, RISING);
}

void SonarDetect0() {
  if(SonarVal[0] == 0) {
    SonarVal[0] = micros();
    attachInterrupt(2, SonarDetect0, FALLING);
  }
  else{
    SonarVal[0] = micros() - SonarVal[0];
    Sonar_Norm(0);
    SonarVal[0] = 0;
    attachInterrupt(2, SonarDetect0, RISING);
  }
}

void SonarDetect1() {
  if(SonarVal[1] == 0) {
    SonarVal[1] = micros();
    attachInterrupt(3, SonarDetect1, FALLING);
  }
  else{
    SonarVal[1] = micros() - SonarVal[1];
    Sonar_Norm(1);
    SonarVal[1] = 0;
    attachInterrupt(3, SonarDetect1, RISING);
  }
}

void SonarDetect2() {
  if(SonarVal[2] == 0) {
    SonarVal[2] = micros();
    attachInterrupt(4, SonarDetect2, FALLING);
  }
  else{
    SonarVal[2] = micros() - SonarVal[2];
    Sonar_Norm(2);
    SonarVal[2] = 0;
    attachInterrupt(4, SonarDetect2, RISING);
  }
}

void SonarDetect3() {
  if(SonarVal[3] == 0) {
    SonarVal[3] = micros();
    attachInterrupt(5, SonarDetect3, FALLING);
  }
  else{
    SonarVal[3] = micros() - SonarVal[3];
    Sonar_Norm(3);
    SonarVal[3] = 0;
    attachInterrupt(5, SonarDetect3, RISING);
  }
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
    long tmp = abs(SonarVal[i] - SonarInt[i]);

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
		digitalWrite(sonar_trigger, HIGH);
		digitalWrite(sonar_trigger, LOW);
}

void Sonar_print() {
  for (int i = 0; i <= sonar_index; i++) {
    Serial.print("Sonar");Serial.print(i);Serial.print(':');Serial.println((int)SonarNorm[i]);
    //Serial2.print("Sonar");Serial2.print(i);Serial2.print(':');Serial2.println((int)SonarNorm[i]);
  }
  Serial.println();
  //Serial2.println();
}

