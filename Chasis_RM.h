#ifndef __CHASIS_RM_H__
#define __CHASIS_RM_H__

#define PWM_MODE                                0x11
#define PWM_CURRENT_MODE                        0x22
#define PWM_VELOCITY_MODE                       0x33
#define PWM_POSITION_MODE                       0x44


void CAN_RoboModule_DRV_Reset(unsigned char Number);
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Number,unsigned char Mode);
void CAN_RoboModule_DRV_PWM_Mode(unsigned char Number,short Temp_PWM);
void CAN_RoboModule_DRV_PWM_Current_Mode(unsigned char Number,short Temp_PWM,short Temp_Current);
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Number,short Temp_PWM,short Temp_Velocity);
void CAN_RoboModule_DRV_PWM_Position_Mode(unsigned char Number,short Temp_PWM,long Temp_Position);



#endif 
