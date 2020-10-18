#include "PID_UP.h"
#include "string.h"
#include "math.h"
#include "UART_1.h"



struct pid_ _PID_;
void pid_Init(void)
{
	_PID_.Set_=0.0;
	_PID_.Actual_=0.0;
	_PID_.err_=0.0;
	_PID_.err_Last_=0.0;
	_PID_.err_Last__=0.0;
	_PID_.Kp_=0.2;
	_PID_.Ki_=0.015;
	_PID_.Kd_=0.2;
	_PID_.Kp_1=0.2;
	_PID_.Ki_1=0.015;
	_PID_.Kd_1=0.2;
	_PID_.voltage_=0.0;
	
	
}

float PID_up(float Current)
{
_PID_.Set_=Current;
_PID_.err_=_PID_.Set_-_PID_.Actual_;

//	if(fabs(_PID_.err_)>50)
//	{
//	_PID_.voltage_=_PID_.Kp_*(_PID_.err_-_PID_.err_Last_)
//	                +_PID_.Kd_*(_PID_.err_-2*_PID_.err_Last_-_PID_.err_Last__);}
//	else 
	//if(fabs(pid.err)>100)
		//	{pid.voltage=pid.Kp1*pid.err+pid.Ki1*pid.Err_integral+pid.Kd1*(pid.err-pid.err_last);}
	//else 
	_PID_.voltage_=_PID_.Kp_*(_PID_.err_-_PID_.err_Last_)
	                +_PID_.Ki_*_PID_.err_+
	                _PID_.Kd_*(_PID_.err_-2*_PID_.err_Last_-_PID_.err_Last__);
	_PID_.err_Last__=_PID_.err_Last_;
	_PID_.err_Last_=_PID_.err_;
	_PID_.voltage__+=_PID_.voltage_;
	return (uint16_t)_PID_.voltage__;
}


