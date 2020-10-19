#ifndef __Speed_H
#define __Speed_H	

#include "main.h"
#include "adc.h"

typedef struct _motor{
	int32_t CaptureNumber;		// 输入捕获数
	int32_t Last_CaptureNumber;		// 上一次捕获值
	uint16_t SUM_Pulse;		//1s内的脉冲数  把|MSF|加上去
	int16_t MSF;		// 电机反馈速度    每次采样与上次采样的脉冲差数
	int16_t Speed;
}motor;

extern double T_IN,Rh_IN,P_IN,S_IN,T1_IN,T2_IN,T3_IN;
extern uint8_t time_count5;
void Speed_Init(void);
void Get_Speed(void);
void ADC_DAC_Init(void);
void Get_ADC(void);
void Device_Init(void);


#endif

