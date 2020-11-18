#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_1_                          ADC1
#define    ADC_1_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_1_CLK                       RCC_APB2Periph_ADC1

#define    ADC_1_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_1_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_1_PORT                      GPIOC

// PC1-ͨ��11 ����IO
#define    ADC_1_PIN                       GPIO_Pin_1
#define    ADC_1_CHANNEL                   ADC_Channel_11
#define    ADC_1_DMA_CHANNEL               DMA1_Channel1

#define RCC_TIM_4 RCC_APB1Periph_TIM4
#define TIM_AD_4    TIM4
void ADC1_Init(void);
void TIM4_ADC_Init(void);
#endif /* __ADC_H */

