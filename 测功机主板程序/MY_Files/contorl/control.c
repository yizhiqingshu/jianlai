#include "contorl.h"
#include "dac.h"
#include "Speed.h"

PID_varible close_T1,close_T2,close_T3,close_S;

int16_t close_T1_PID(int16_t target,double out,double Kp,double Ki,double Kd)	// Ŀ��ֵ   ��ǰ����ֵ  PID����
{
	int16_t output;
	double error;
	error=target-out;
	close_T1.error_sum+=error;
	close_T1.error_d=error-close_T1.last_error;
	output=(int32_t)(Kp*error+Ki*close_T1.error_sum+Kd*close_T1.error_d);
	//����޷�����
	if(output>T1_output_max)
		output=T1_output_max;		//��ֵΪ4095,3.3V
	if(output<T1_output_min)
		output=T1_output_min;
	close_T1.last_error=error;
	return output;
}

int16_t close_T2_PID(int16_t target,double out,double Kp,double Ki,double Kd)	// Ŀ��ֵ   ��ǰ����ֵ  PID����
{
	int16_t output;
	double error;
	error=target-out;
	close_T2.error_sum+=error;
	close_T2.error_d=error-close_T2.last_error;
	output=(int32_t)(Kp*error+Ki*close_T2.error_sum+Kd*close_T2.error_d);
	//����޷�����
	if(output>T2_output_max)
		output=T2_output_max;
	if(output<T2_output_min)
		output=T2_output_min;
	close_T2.last_error=error;
	return output;
}

int16_t close_T3_PID(int16_t target,double out,double Kp,double Ki,double Kd)	// Ŀ��ֵ   ��ǰ����ֵ  PID����
{
	int16_t output;
	double error;
	error=target-out;
	close_T3.error_sum+=error;
	close_T3.error_d=error-close_T3.last_error;
	output=(int32_t)(Kp*error+Ki*close_T3.error_sum+Kd*close_T3.error_d);
	//����޷�����
	if(output>T3_output_max)
		output=T3_output_max;
	if(output<T3_output_min)
		output=T3_output_min;
	close_T3.last_error=error;
	return output;
}

int16_t close_S_PID(int16_t target,double out,double Kp,double Ki,double Kd)	// Ŀ��ֵ   ��ǰ����ֵ  PID����
{
	int16_t output;
	double error;
	error=target-out;
	close_S.error_sum+=error;
	close_S.error_d=error-close_S.last_error;
	output=(int32_t)(Kp*error+Ki*close_S.error_sum+Kd*close_S.error_d);
	//����޷�����
	if(output>S_output_max)
		output=S_output_max;
	if(output<S_output_min)
		output=S_output_min;
	close_T3.last_error=error;
	return output;
}

void Contorl_T(void)
{
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL1, DAC_ALIGN_12B_R, close_T1_PID(T1_IN,close_T1.out,close_T1.Kp,close_T1.Ki,close_T1.Kd));//out��ǰ����ֵ
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL2, DAC_ALIGN_12B_R, close_T2_PID(T2_IN,close_T2.out,close_T2.Kp,close_T2.Ki,close_T2.Kd));
	//close_T3_PID(double target,double out,double Kp,double Ki,double Kd);
}

void contorl(void)
{
	if(time_count5==1)
	{
		Contorl_T();	//��������Ϊ20ms
		time_count5=0;
	}
}
