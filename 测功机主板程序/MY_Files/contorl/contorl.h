#ifndef __contorl_H
#define __contorl_H	

#include "main.h"

#define T1_output_max 4095
#define T1_output_min 0
#define T2_output_max 4095
#define T2_output_min 0
#define T3_output_max 4095
#define T3_output_min 0
#define S_output_max 4095
#define S_output_min 0

typedef struct _PID_varible{
	double Kp;
	double Ki;
	double Kd;
	double out;	//当前测量值
	double last_error;
	double error_sum;
	double error_d;
}PID_varible;


extern PID_varible close_T1,close_T2,close_T3,close_S;

int16_t T1_PID(int16_t target,double out,double Kp,double Ki, double Kd);
int16_t T2_PID(int16_t target,double out,double Kp,double Ki, double Kd);
int16_t T3_PID(int16_t target,double out,double Kp,double Ki, double Kd);
int16_t close_S_PID(int16_t target,double out,double Kp,double Ki,double Kd);

void contorl(void);
#endif  
