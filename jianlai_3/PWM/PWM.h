#ifndef _PWM_1
#define _PWM_1
#include "stm32f10x.h"
#include "JIANLAI_.h"

#if ADVANCE_TIM1
#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (8-1)
#define            ADVANCE_TIM_PSC               (9-1)
#define            ADVANCE_TIM_PULSE             4

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 输出比较通道的互补通道
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH1N_PORT          GPIOB
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_13

// TIM1 输出比较通道的刹车通道
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12
void ADVANCE_TIM_Init(void);
#endif

/**************************函数声明********************************/

#if TIM3_PWM_CH1_4
#define			TIM_X_PWM			TIM3
#define 		TIM_X_PWM_RCC 		RCC_APB1Periph_TIM3
#define			TIM_GPIOA_RCC   	RCC_APB2Periph_GPIOA
#define 		TIM_GPIOB_RCC		RCC_APB2Periph_GPIOB
#define  		TIM_GPIOC_RCC  		RCC_APB2Periph_GPIOC

#endif	
#if TIM4_PWM_CH1_4
#define			TIM_4_PWM			TIM4
#define 		TIM_4_PWM_RCC 		RCC_APB1Periph_TIM4
#define			TIM_GPIOD_RCC   	RCC_APB2Periph_GPIOD
#define 		TIM_GPIOB_RCC		RCC_APB2Periph_GPIOB
void TIM4_PWM_Init(u16 arr,u16 psc);
#endif

void TIMx_PWM_Init(u16 arr,u16 psc);
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}

#endif

