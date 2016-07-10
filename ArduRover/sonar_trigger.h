#ifndef _SONAR_TRI_H_
#define _SONAR_TRI_H_

#define MAX_DEV 10

void Sonar_Initialization(int);//trigger-pin
int Sonar_Bind(int);
void Sonar_update(void);
void Sonar_print(void);

#endif
