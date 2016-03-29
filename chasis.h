#ifndef _CHASIS_H_             //如果没有定义宏_MY_HEAD_H_
#define _CHASIS_H_             //则，定义该宏名

//以下是被保护的代码区
//进行相应的全局变量和结构体类型定义
/********************CONSTANT AREA**********************/
#define w1n  22	
#define w1p  23
#define w1e  3

#define w2n  24	
#define w2p  25
#define w2e  4

#define w3n  26	
#define w3p  27
#define w3e  5

#define w4n  28	
#define w4p  29
#define w4e  6
/********************CONSTANT AREA**********************/
typedef struct
{
  short pPin;
  short nPin;
  short ENPin;
  float para;
}AWheel;

void Mov_Clear(void);

void Mov_For(float);
void Mov_Back(float);
void Mov_Left(float);
void Mov_Right(float);

void Rot_Left(void);
void Rot_Right(void);

#endif
