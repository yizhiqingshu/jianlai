#ifndef _DAC_1
#define _DAC_1

#include "stm32f10x.h"

//DAC DHR12RD�Ĵ�����12λ���Ҷ��롢˫ͨ��
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) 


void Dac1_Init(void);
void GPIO_DAC(void);
void Dac1_Set_Vol(u16 vol);
void DAC_1_Config(void);

#endif

