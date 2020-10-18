#ifndef _PWM_UP_H
#define _PWM_UP_H
struct pid_
{
float Set_;
float Actual_;
float err_;
float err_Last_;
float err_Last__;
float Kp_,Ki_,Kd_;
float Kp_1,Ki_1,Kd_1;
float voltage_;
float voltage__;
};
float PID_up(float Current);
void pid_Init(void);
#endif
