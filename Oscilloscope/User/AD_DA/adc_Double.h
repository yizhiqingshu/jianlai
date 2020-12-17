#ifndef __ADC_DOUBLE_H
#define	__ADC_DOUBLE_H


#include "stm32f10x.h"
#include "bsp_usart.h"
#include "arm_math.h"
#define NPT 1024
#define FFT_LENGTH 256
#define FFT_type_fft 0
#define FFT_type_ifft 1
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOC  
#define    ADC_PORT                      GPIOC

// 注意
// 1-PC0 在指南者里面接的是蜂鸣器，默认被拉低
// 2-PC0 在指南者里面接的是SPI FLASH的 片选，默认被拉高
// 所以 PC0 做 ADC 转换通道的时候，结果可能会有误差

// 转换通道个数
#define    NOFCHANEL										 2

#define    ADC_PIN1                      GPIO_Pin_0
#define    ADC_CHANNEL1                  ADC_Channel_10

#define    ADC_PIN2                      GPIO_Pin_1
#define    ADC_CHANNEL2                  ADC_Channel_11

#define    ADC_PIN3                      GPIO_Pin_2
#define    ADC_CHANNEL3                  ADC_Channel_12

#define    ADC_PIN4                      GPIO_Pin_3
#define    ADC_CHANNEL4                  ADC_Channel_13

#define    ADC_PIN5                      GPIO_Pin_4
#define    ADC_CHANNEL5                  ADC_Channel_14

#define    ADC_PIN6                      GPIO_Pin_5
#define    ADC_CHANNEL6                  ADC_Channel_15


// ADC1 对应 DMA1通道1，ADC3对应DMA2通道5，ADC2没有DMA功能
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1

#define ulong unsigned long
    #define Fs 100000
#define PI2 6.28318530717959
//#define N 100000/1024
/**************************函数声明********************************/

void ADC_DOUBLE_Init(void);
float *FFT(float fft_src[],
float fft_dst[],
float fft_fre[]);
void TIM2_Init(uint16_t Per,uint16_t PSC,uint16_t Pluse);
void InitBufInArray();
#endif /* __ADC_H */

