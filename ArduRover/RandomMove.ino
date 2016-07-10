#include "Chasis_RM.h"
#include "sonar_trigger.h"

#define MAX_SPEED 120
#define MAX_LOOP 100
#define BND_LOOP 80
#define DETECT_BOUND 250

extern int SonarNorm[];

extern int rc[16];

//int width = SonarVal[0] + SonarVal[2];
//int length = SonarVal[1] + SonarVal[3];
static int ci = 0, cj = 0;
static int ck = MAX_LOOP;
int rnd_dir;

bool Bound_Detect() {
  bool f1 = 0, f2 = 0;
	if (SonarNorm[0] < DETECT_BOUND)	{
	  ci = -1;
    f1 = 1;
	}
  else
	  if (SonarNorm[2] < DETECT_BOUND)	{
	    ci = +1;
      f1 = 1;
	  }
	
	if (SonarNorm[1] < DETECT_BOUND)	{
	  cj = +1;
    f2 = 1;
	}
	else
	  if (SonarNorm[3] < DETECT_BOUND)	{
	    cj = -1;
      f2 = 1;
	  }

   return (f1||f2);
}

void RandomMove() {
  if(Bound_Detect()){
    TransMove_RM(ci * MAX_SPEED, cj * MAX_SPEED, 0);
  }
  else{
    randomSeed(analogRead(A0)); 
    rnd_dir = (2 * (int)(random(2)) + rnd_dir + 1) % 4;

    switch(rnd_dir) {
      case 0:ci=+1;cj=0;break;//forward
      case 1:ci=0;cj=+1;break;//right
      case 2:ci=-1;cj=0;break;//backward
      case 3:ci=0;cj=-1;break;//left
    }

    TransMove_RM(ci * MAX_SPEED, cj * MAX_SPEED, 0);
    delay(8E2);
  }
  
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
