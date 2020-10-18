#ifndef _PID_position_h
#define _PID_position_h
#include "stm32f10x.h"


float PID_realize(float speed);
struct _pid{
float SetSpeed; //定义设定值
float ActualSpeed; //定义实际测量值
float err; //定义偏差值
float err_last; //定义上一个周期偏差值
float Kp,Ki,Kd; //定义比例、积分、微分系数
float Kp1,Ki1,Kd1;
float voltage; //定义电压值（控制执行器的变量）
float Err_integral; //定义偏差累积值
float Err_last_integral; //定义上一个周期偏差累积值
};
#endif

