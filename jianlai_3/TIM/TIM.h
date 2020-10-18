#ifndef TIM_6_PID_
#define TIM_6_PID_
#include "stm32f10x.h"
#include "JIANLAI_.h"

#define BASIC_TIM6 // 如果使用TIM7，注释掉这个宏即可

#ifdef  BASIC_TIM6 // 使用基本定时器TIM6
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
//#define            BASIC_TIM_Period            1000-1
//#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#elif BASI_TIM7// 使用基本定时器TIM7
#define            BASIC_TIM                   TIM7
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM7
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM7_IRQn
#define            BASIC_TIM_IRQHandler        TIM7_IRQHandler

#elif UNIVERSAL_TIM2_5
#define            UNIVERSAL_TIM2_5                   TIM7
#define            UNIVERSAL_TIM2_5APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            UNIVERSAL_TIM_CLK               RCC_APB1Periph_TIM7
#define            UNIVERSAL_TIM_Period            1000-1
#define            UNIVERSAL_TIM_Prescaler         71
#define            UNIVERSAL_TIM_IRQ               TIM7_IRQn
#define            UNIVERSAL_TIM_IRQHandler        TIM7_IRQHandler

#endif


/**************************函数声明********************************/
void BASIC_TIM_Mode_Config(uint16_t arr,uint16_t psc);
void BASIC_TIM_Init(void);


#endif

