#ifndef CHASIS_H_             //如果没有定义宏_MY_HEAD_H_
#define CHASIS_H_             //则，定义该宏名

//以下是被保护的代码区
//进行相应的全局变量和结构体类型定义
/********************CONSTANT AREA**********************/
const int w1n = 22;	const int w1p = 23;
const int w1e = 3;

const int w2n = 24;	const int w2p = 25;
const int w1e = 4;

const int w3n = 26;	const int w3p = 27;
const int w1e = 5;

const int w4n = 28;	const int w4p = 29;
const int w1e = 6;
/********************CONSTANT AREA**********************/

void Mov_Clear(void);

void Mov_For(float);
void Mov_Back(float);
void Mov_Left(float);
void Mov_Right(float);

void Rot_Left(void);
void Rot_Right(void);

#endif
