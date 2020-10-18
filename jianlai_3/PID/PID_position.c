#include "PID_position.h"
#include "math.h"
#include "UART_1.h"


struct _pid pid;

void PID_init(){
printf("PID_init begin \n");
pid.SetSpeed=0.0;
pid.ActualSpeed=0.0;
pid.err=0.0;
pid.err_last=0.0;
pid.voltage=0.0;
pid.Err_integral=0.0;
pid.Err_last_integral=0.0;
pid.Kp=0.2;
pid.Ki=0.015;
pid.Kd=0.2;
pid.Kp1=0.5;
pid.Ki1=0;
pid.Kd1=0.2;
printf("PID_init end \n");
}


float PID_realize(float speed)
{
pid.SetSpeed=speed;
pid.err=pid.SetSpeed-pid.ActualSpeed;

		
pid.Err_integral=pid.Err_last_integral+pid.err;
	//if(fabs(pid.err)>100)
		//	{pid.voltage=pid.Kp1*pid.err+pid.Ki1*pid.Err_integral+pid.Kd1*(pid.err-pid.err_last);}

	//else 
	pid.voltage=pid.Kp*pid.err+pid.Ki*pid.Err_integral+pid.Kd*(pid.err-pid.err_last);
	pid.err_last=pid.err;
	pid.Err_last_integral=pid.Err_integral;
	return (uint16_t)pid.voltage;
}




