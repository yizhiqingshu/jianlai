#ifndef _DAC_1
#define _DAC_1

#include "stm32f10x.h"

//DAC DHR12RD寄存器，12位、右对齐、双通道
#define DAC_DHR12RD_ADDRESS      (DAC_BASE+0x20) 


void Dac1_Init(void);
void GPIO_DAC(void);
void Dac1_Set_Vol(u16 vol);
void DAC_1_Config(void);

#endif

