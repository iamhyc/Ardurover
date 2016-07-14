#include "Chasis_RM.h"
#include "sonar_trigger.h"

#define MAX_SPEED 90
#define MAX_LOOP 100
#define BND_LOOP 25
#define DETECT_BOUND_HEAD 300
#define DETECT_BOUND_SIDE 320

extern int SonarNorm[];

extern int rc[16];

//int width = SonarVal[0] + SonarVal[2];
//int length = SonarVal[1] + SonarVal[3];
static int ci = 0, cj = 0, cs = 0;
static int ck = MAX_LOOP;
int rnd_dir;
//long tim;

bool Bound_Detect() {
  bool f1 = 0, f2 = 0;
	if (SonarNorm[0] < DETECT_BOUND_HEAD)	{
	  ci = -1;cs = 0;
    f1 = 1;
	}
  else
	  if (SonarNorm[2] < DETECT_BOUND_HEAD)	{
	    ci = +1;cs = 0;
      f1 = 1;
	  }
	
	if (SonarNorm[1] < DETECT_BOUND_SIDE)	{
	  cj = +1;cs = 0;
    f2 = 1;
	}
	else
	  if (SonarNorm[3] < DETECT_BOUND_SIDE)	{
	    cj = -1;cs = 0;
      f2 = 1;
	  }
   return (f1||f2)&&(cs==0);
}

void RandomMove() {
  if(Bound_Detect() && (ck < BND_LOOP)){
    ck = BND_LOOP;
    //Serial.println(ck);
  }
  else if(ck <= MAX_LOOP){
    ck++;
  }   
  else{
    ck = 1;
    //Serial.println(millis() - tim);tim = millis(); 
    randomSeed(analogRead(A0)); 
    int rnd_val = random(2);
    rnd_dir = (2 * rnd_val + rnd_dir + 1) % 4;
    rnd_dir = ((int)(random(3)) + rnd_dir + 1) % 4;
    randomSeed(analogRead(A0));
    rnd_val = random(6);
    rnd_dir = (rnd_val==0)?(4):(rnd_dir);
    //Serial.println(rnd_dir);
    switch(rnd_dir) {
      case 0:ci=+1;cj=0;cs=0;break;//forward
      case 1:ci=0;cj=+1;cs=0;break;//right
      case 2:ci=-1;cj=0;cs=0;break;//backward
      case 3:ci=0;cj=-1;cs=0;break;//left
      case 4:ci=0;cj=0;cs=+1;break;//rotate
    }

  }
  //Serial.println(ck);
  //Serial.print(ci);Serial.print(' ');Serial.print(cj);Serial.print(' ');Serial.println(cs);
  TransMove_RM(ci * MAX_SPEED, cj * MAX_SPEED, cs * MAX_SPEED);
}

void AutoMove(char sw) {
	switch(sw) {
		case 1:
      RandomMove();
    break;
    case 0:
		default:
      //ci = (rc[1]>0?1:-1);cj = (rc[0]>0?1:-1);BoundDetect();
			TransMove_RM(rc[1], rc[0], rc[3]);
		break;
	}
}
