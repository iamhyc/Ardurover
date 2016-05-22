#ifndef _CHASIS_H_             //如果没有定义宏_MY_HEAD_H_
#define _CHASIS_H_             //则，定义该宏名

//以下是被保护的代码区
//进行相应的全局变量和结构体类型定义
typedef struct
{
  short pPin;
  short nPin;
  short ENPin;
  float para;
}AWheel;

void Wheel_Stop(void);
void Wheel_Ctrl(AWheel, float);
void TransMove(float, float, float);
#endif
