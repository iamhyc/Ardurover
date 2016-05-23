#ifndef _DBUS_CTRL_H_             //如果没有定义宏_MY_HEAD_H_
#define _DBUS_CTRL_H_             //则，定义该宏名

#define R3H_CH1	67
#define R3M_CH1	0
#define R3L_CH1	-61//
#define R3H_CH2	69
#define R3M_CH2	6
#define R3L_CH2	-63//
#define R3H_CH3	69
#define R3M_CH3	7
#define R3L_CH3	-65//
#define R3H_CH4	69
#define R3M_CH4	2
#define R3L_CH4	-64//

#define S3H		92
#define S3M		3
#define S3L		-85
#define S2H		60
#define S2L		-60

void SBUS_update(void);
void SBUS_Normlize();
char norm_sgn2(int);
char norm_sgn3(int);
int norm_sgn255(int, int, int, int);//ls, ms, hs, val
//long map(long value, long fromStart, long fromEnd, long toStart, long toEnd);

#endif
