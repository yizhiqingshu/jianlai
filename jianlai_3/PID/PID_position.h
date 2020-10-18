#ifndef _PID_position_h
#define _PID_position_h
#include "stm32f10x.h"


float PID_realize(float speed);
struct _pid{
float SetSpeed; //�����趨ֵ
float ActualSpeed; //����ʵ�ʲ���ֵ
float err; //����ƫ��ֵ
float err_last; //������һ������ƫ��ֵ
float Kp,Ki,Kd; //������������֡�΢��ϵ��
float Kp1,Ki1,Kd1;
float voltage; //�����ѹֵ������ִ�����ı�����
float Err_integral; //����ƫ���ۻ�ֵ
float Err_last_integral; //������һ������ƫ���ۻ�ֵ
};
#endif

