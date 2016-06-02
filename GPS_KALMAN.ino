#include "Ublox.h"
#include "GPS_KALMAN.h"

bool GPS_Init_Flag = false;
float cosLat = 0.0;  
extern Ublox M8_Gps;
/****************************************/
extern unsigned long LOC_TIME;
float delta_T = 0; 
//((float) (GPS_data[GPS_INDEX][2] - GPS_data[GPS_INDEX-1][2])) / 1000; 
//LOC_TIME elapsed in seconds

float LEN_DATA[2] = {0,0}; 
//{(float) (GPS_data[GPS_INDEX][0]-GPS_data[0][0])*PI/180*Rearth/10000000,
//(float) (GPS_data[GPS_INDEX][1]-GPS_data[0][1])*PI/180*Rearth*cosLat/10000000};

float Matrix_P[4][4] = {
  {0.001, 0,     0,    0   },
  {0,     0.001, 0,    0   },
  {0,     0,     0.02, 0   },
  {0,     0,     0,    0.02},
};

float Xstate[4][1] = {
        {LEN_DATA[0]},
        {LEN_DATA[1]},
        {0.0    },
        {0.0    },
};

/****************************************/
int64_t GPS_data[1][3] = {
        {0,0,0},
};
int64_t prevGPS_data[1][3] = {
        {0,0,0},
};
int64_t firstGPS_data[1][3] = {
        {0,0,0},
};

void GPS_update_kalman()
{
  //if(Serial1.available())
  if(millis() < LOC_TIME + 1500)
  {
    prevGPS_data[0][0] = GPS_data[0][0];
    prevGPS_data[0][1] = GPS_data[0][1];
    prevGPS_data[0][2] = GPS_data[0][2];
    
    GPS_data[0][0] = GPS_get("LAT");
    GPS_data[0][1] = GPS_get("LNG");
    GPS_data[0][2] = (uint64_t)LOC_TIME;
    //Test for valid GPS data in Continental U.S. 
    KalmanData();
  }
  else if (GPS_Init_Flag){
    Serial.println("NO DATA.");
    //If system is initialized & GPS data not available for more than 1 second
    KalmanNoData();
  }
}

void KalmanData(){

          if (!GPS_Init_Flag){ //Initialize
            firstGPS_data[0][0] = GPS_data[0][0];
            firstGPS_data[0][1] = GPS_data[0][1];
            firstGPS_data[0][2] = GPS_data[0][2];

            cosLat = cos((float) firstGPS_data[0][0]/10000000*PI/180);
            GPS_Init_Flag = true;
          }


          delta_T = ((float) (GPS_data[0][2] - prevGPS_data[0][2])) / 1000; //LOC_TIME elapsed in seconds
          float Matrix_A[4][4] = {
            {1, 0,  delta_T, 0      },
            {0, 1,  0,       delta_T},
            {0, 0,  1,       0      },
            {0, 0,  0,       1      },
          };

          float Xstate_Estimate[4][1] = {0,0,0,0};
          Matrix.Multiply((float*)Matrix_A, (float*)Xstate, 4, 4, 1, (float*)Xstate_Estimate);       
          /*************Line 78 of Matlab code*************/
          /** Cal_Line78 Function
          * Matrix_A_Trans    = Transpose(Matrix_A)
          * Matrix_Ans        = Multiply(Matrix_A, Matrix_P)
          * Matrix_Ans        = Multiply(Matrix_Ans, Matrix_A_Trans)
          * Matrix_P_Estimate = ADD(Matrix_Ans, Matrix_Q)
          *
          * P[k+1] = Q + A*P[k]*A(T)
          **/
          float Matrix_P_Estimate[4][4] = {0, 0, 0, 0};
          float Matrix_A_Trans[4][4] = {0, 0, 0, 0};
          float Matrix_Tmp44_1[4][4] = {0, 0, 0, 0};
          float Matrix_Tmp44_2[4][4] = {0, 0, 0, 0};

          Matrix.Transpose((float*)Matrix_A, 4, 4, (float *)Matrix_A_Trans);
          Matrix.Multiply((float*)Matrix_A, (float*)Matrix_P, 4, 4, 4, (float*)Matrix_Tmp44_1);
          Matrix.Multiply((float*)Matrix_Tmp44_1, (float*)Matrix_A_Trans, 4, 4, 4, (float*)Matrix_Tmp44_2);
          Matrix.Add((float*)Matrix_Tmp44_2, (float*)Matrix_Q, 4, 4, (float*)Matrix_P_Estimate);
          /*************Line 81 of Matlab code*************/
          /**
          * Matrix_Ans = Multiply(Matrix_H, Matrix_P_Estimate)
          * Matrix_Ans = Multiply(Matrix_Ans, Matrix_H_Trans)
          * Matrix_Tmp = ADD(Matrix_Ans, Matrix_R)
          * {H*P[k+1]*H(T) + R}
          * Matrix_Ans = Multiply(Matrix_P_Estimate, Matrix_H_Trans)
          * KalmanGain = Multiply(Matrix_Ans, Matrix_Tmp)
          * GAIN = {P[k+1]*H(T)}*{H*P[k+1]*H(T) + R}
          **/
          float KalmanGain[4][2] = {0,0,0,0};
          float Matrix_Tmp24[2][4] = {0,0};
          float Matrix_Tmp22_1[2][2] = {0,0};
          float Matrix_Tmp22_2[2][2] = {0,0};
          float Matrix_Tmp42[4][2] = {0,0,0,0};
          
          Matrix.Multiply((float*)Matrix_H, (float*)Matrix_P_Estimate, 2, 4, 4, (float*)Matrix_Tmp24);
          Matrix.Multiply((float*)Matrix_Tmp24, (float*)Matrix_H_Trans, 2, 4, 2, (float*)Matrix_Tmp22_1);
          Matrix.Add((float*)Matrix_Tmp22_1, (float*)Matrix_R, 2, 2, (float*)Matrix_Tmp22_2);
          /*************/
          Matrix.Multiply((float*)Matrix_P_Estimate, (float*)Matrix_H_Trans, 4, 4, 2, (float*) Matrix_Tmp42);
          Matrix.Multiply((float*)Matrix_Tmp42, (float*)Matrix_Tmp22_2, 4, 2, 2, (float*)KalmanGain);


          LEN_DATA[0] = (float) (GPS_data[0][0]-firstGPS_data[0][0])*PI/180*Rearth/10000000;
          LEN_DATA[1] = (float) (GPS_data[0][1]-firstGPS_data[0][1])*PI/180*Rearth*cosLat/10000000;

          //We only need the transposed version of this, so we do it right here.
          /**
          * Matrix_Ans = Multiply(Matrix_H, Xstate_Estimate)
          * Matrix_Ans = Substract(Zk_Trans, Matrix_Ans)
          * Matrix_Ans = Multiply(KalmanGain, Matrix_Ans)
          * Xstate     = Add(Xstate_Estimate, Matrix_Ans)
          * X[k] = X[k+1] + KalmanGain*{Z[k](T)-H*X[k+1]}
          **/
          float ZkTranspose[2][1] = {
            {LEN_DATA[0]},
            {LEN_DATA[1]},
          }; 
          float Matrix_Tmp21[2][1] = {0,0};
          float Matrix_Tmp41[4][1] = {0,0,0,0};

          Matrix.Multiply((float*)Matrix_H, (float*)Xstate_Estimate, 2, 4, 1, (float*)Matrix_Tmp21);
          Matrix.Subtract((float*)ZkTranspose, (float*)Matrix_Tmp21, 2, 1, (float*) Matrix_Tmp21);
          Matrix.Multiply((float*)KalmanGain, (float*)Matrix_Tmp21, 4, 2, 1, (float*) Matrix_Tmp41); 
          //Reuse intermediate matrix because it has appropriate dimensions
          Matrix.Add((float*)Xstate_Estimate, (float*)Matrix_Tmp41, 4, 1, (float*)Xstate); 
          //NO NEED TO TRANSPOSE X STATE. WE DID THAT IN MATLAB FOR CONVENIENCE

          /*************Line 84 of Matlab code*************/
          Matrix.Multiply((float*)KalmanGain, (float*)Matrix_H, 4, 2, 4, (float*)Matrix_Tmp44_1);
          Matrix.Subtract((float*)Matrix_I, (float*)Matrix_Tmp44_1, 4,4, (float*)Matrix_Tmp44_2);
          Matrix.Multiply((float*)Matrix_Tmp44_2, (float*)Matrix_P_Estimate, 4,4,4, (float*)Matrix_P);
          GPS_DI_Print();
}

void KalmanNoData(){
          uint64_t currentTime = (int32_t)millis();  
          delta_T = (float) (currentTime - LOC_TIME) / 1000; //LOC_TIME elapsed in seconds
          float Matrix_A[4][4] = {
                 {1, 0,  delta_T, 0      },
                 {0, 1,  0,       delta_T},
                 {0, 0,  1,       0      },
                 {0, 0,  0,       1      },
          };          
          float Xstate_Estimate[4][1] = {
                {0},
                {0},
                {0},
                {0},
          };    
          Matrix.Multiply((float*)Matrix_A, (float*)Xstate, 4, 4, 1, (float*)Xstate_Estimate);
          Xstate[0][0] = Xstate_Estimate[0][0];
          Xstate[1][0] = Xstate_Estimate[1][0];
          Xstate[2][0] = Xstate_Estimate[2][0];
          Xstate[3][0] = Xstate_Estimate[3][0];
                    
          /*************Line 78 of Matlab code*************/
          float Matrix_P_Estimate[4][4] = {0, 0, 0, 0};
          float Matrix_A_Trans[4][4] = {0, 0, 0, 0};
          float Matrix_Tmp44_1[4][4] = {0, 0, 0, 0};
          float Matrix_Tmp44_2[4][4] = {0, 0, 0, 0};

          Matrix.Transpose((float*)Matrix_A, 4, 4, (float *)Matrix_A_Trans);
          Matrix.Multiply((float*)Matrix_A, (float*)Matrix_P, 4, 4, 4, (float*)Matrix_Tmp44_1);
          Matrix.Multiply((float*)Matrix_Tmp44_1, (float*)Matrix_A_Trans, 4, 4, 4, (float*)Matrix_Tmp44_2);
          Matrix.Add((float*)Matrix_Tmp44_2, (float*)Matrix_Q, 4, 4, (float*)Matrix_P_Estimate);

          for (int j = 0; j < 4; j++){
            for (int k = 0; k < 4; k++){
              Matrix_P[j][k] = Matrix_P_Estimate[j][k];
            }
          }          
          GPS_DN_Print();
          LOC_TIME = millis();
}

void GPS_DI_Print() {
  Serial.print(Xstate[0][0]);Serial.print(",");
  Serial.print(Xstate[1][0]);Serial.print(",");
  Serial.print(Xstate[2][0]);Serial.print(",");
  Serial.print(Xstate[3][0]);Serial.print(",");

  //Serial.print(ZkTranspose[0][0]);Serial.print(",");
  //Serial.print(ZkTranspose[1][0]);Serial.print(",");
  Serial.print((int32_t) GPS_data[0][0]);Serial.print(",");
  Serial.print((int32_t) GPS_data[0][1]);Serial.print(",");
  Serial.println((int32_t) GPS_data[0][2]);
}

void GPS_DN_Print() {
  Serial.println("KALMAN WITH NO DATA");
  Serial.print(Xstate[0][0]);Serial.print(",");
  Serial.print(Xstate[1][0]);Serial.print(",");
  Serial.print(Xstate[2][0]);Serial.print(",");
  Serial.print(Xstate[3][0]);Serial.print(",");

  Serial.print(9999);Serial.print(",");
  Serial.print(9999);Serial.print(",");
  Serial.print(-1.0);Serial.print(",");
  Serial.print(-1.0);Serial.print(",");
  //Serial.println((uint32_t)currentTime);
}
