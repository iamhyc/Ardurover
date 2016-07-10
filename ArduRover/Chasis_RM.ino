#include <mcp_can.h>
#include <SPI.h>
#include "Chasis_RM.h"

MCP_CAN CAN(10);

unsigned char RM_Data[8];

void CAN_DRV_Initialization() {
  CAN.begin(CAN_1000KBPS);
  delay(200);
  CAN_RoboModule_DRV_Reset(0);
  delay(200);
  CAN_RoboModule_DRV_Mode_Choice(0,PWM_VELOCITY_MODE);
}

/****************************************************************************************

*****************************************************************************************/
void CAN_RoboModule_DRV_Reset(unsigned char Number)
{
    unsigned short can_id = 0x00;
    
    can_id |= Number<<4;
    
    RM_Data[0] = 0x55;
    RM_Data[1] = 0x55;
    RM_Data[2] = 0x55;
    RM_Data[3] = 0x55;
    RM_Data[4] = 0x55;
    RM_Data[5] = 0x55;
    RM_Data[6] = 0x55;
    RM_Data[7] = 0x55;
    
    CAN.sendMsgBuf(can_id, 0, 8, RM_Data);
}

/****************************************************************************************
PWM_MODE
PWM_CURRENT_MODE
PWM_VELOCITY_MODE
PWM_POSITION_MODE
*****************************************************************************************/
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Number,unsigned char Mode)
{
    unsigned short can_id = 0x01;
    
    can_id |= Number<<4;
    
    RM_Data[0] = Mode;
    RM_Data[1] = 0x55;
    RM_Data[2] = 0x55;
    RM_Data[3] = 0x55;
    RM_Data[4] = 0x55;
    RM_Data[5] = 0x55;
    RM_Data[6] = 0x55;
    RM_Data[7] = 0x55;
    
    CAN.sendMsgBuf(can_id, 0, 8, RM_Data);
}

/****************************************************************************************
temp_pwm
0 ~ +5000

temp_current
-1600 ~ +1600
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Current_Mode(unsigned char Number,short Temp_PWM,short Temp_Current)
{
    unsigned short can_id = 0x03;
    
    can_id |= Number<<4;

    Temp_PWM = abs(Temp_PWM);
    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    
    if(Temp_Current > 2000)
    {
        Temp_Current = 2000;
    }
    if(Temp_Current < -2000)
    {
        Temp_Current = -2000;
    }
    
    RM_Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    RM_Data[1] = (unsigned char)(Temp_PWM&0xff);
    RM_Data[2] = (unsigned char)((Temp_Current>>8)&0xff);
    RM_Data[3] = (unsigned char)(Temp_Current&0xff);
    RM_Data[4] = 0x55;
    RM_Data[5] = 0x55;
    RM_Data[6] = 0x55;
    RM_Data[7] = 0x55;
    
    CAN.sendMsgBuf(can_id, 0, 8, RM_Data);
}

/****************************************************************************************
temp_pwm
0 ~ +5000

temp_velocity
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Number,short Temp_PWM,short Temp_Velocity)
{
    unsigned short can_id = 0x04;
    
    can_id |= Number<<4;

    Temp_PWM = abs(Temp_PWM);
    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    
    RM_Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    RM_Data[1] = (unsigned char)(Temp_PWM&0xff);
    RM_Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    RM_Data[3] = (unsigned char)(Temp_Velocity&0xff);
    RM_Data[4] = 0x55;
    RM_Data[5] = 0x55;
    RM_Data[6] = 0x55;
    RM_Data[7] = 0x55;
    
    CAN.sendMsgBuf(can_id, 0, 8, RM_Data);
}

/****************************************************************************************
temp_pwm
0 ~ +5000

temp_position
32
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Position_Mode(unsigned char Number,short Temp_PWM,long Temp_Position)
{
    unsigned short can_id = 0x05;
    
    can_id |= Number<<4;

    Temp_PWM = abs(Temp_PWM);
    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    
    RM_Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    RM_Data[1] = (unsigned char)(Temp_PWM&0xff);
    RM_Data[2] = 0x55;
    RM_Data[3] = 0x55;
    RM_Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    RM_Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    RM_Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    RM_Data[7] = (unsigned char)(Temp_Position&0xff);
    
    CAN.sendMsgBuf(can_id, 0, 8, RM_Data);
}

void TransMove_RM(int vf, int vtr, int vrr)
{
    int top_speed=maxs(abs(vf),maxs(abs(vtr),abs(vrr)));
    int vt=abs(vf)+abs(vtr)+abs(vrr);
    if (vt!=0)
    {
        vf = (vf*top_speed)/vt;
        vtr = (vtr*top_speed)/vt;
        vrr = (vrr*top_speed)/vt;
    }
  
  CAN_RoboModule_DRV_PWM_Velocity_Mode(1, 5000, vf+vtr+vrr);
  CAN_RoboModule_DRV_PWM_Velocity_Mode(2, 5000, vf-vtr-vrr);
  CAN_RoboModule_DRV_PWM_Velocity_Mode(3, 5000, vf-vtr+vrr);
  CAN_RoboModule_DRV_PWM_Velocity_Mode(4, 5000, vf+vtr-vrr);
}
