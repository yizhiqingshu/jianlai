#include "TIM.h"
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void BASIC_TIM_Mode_Config(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	

    TIM_TimeBaseStructure.TIM_Period = arr;//BASIC_TIM_Period;	

    TIM_TimeBaseStructure.TIM_Prescaler= psc;//BASIC_TIM_Prescaler;
	

    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

    TIM_Cmd(BASIC_TIM, ENABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config(71,999);
}

void  BASIC_TIM_IRQHandler(void)
{
	if (TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		//єЇКэМе
	
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);  		 
	}		 	
}

