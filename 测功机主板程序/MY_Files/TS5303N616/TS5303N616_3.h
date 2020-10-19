#ifndef __ENCODER3_TIM_H__
#define __ENCODER3_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define Encode_3
//#define Encode_2
//#define Encode_3

/* 使用A1 B1 */
#ifdef Encode_1
#define ENCODER_TIMx                        TIM3
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM3_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM3_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_6
#define ENCODER_TIM_CH1_GPIO                GPIOC
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_7
#define ENCODER_TIM_CH2_GPIO                GPIOC
#define ENCODER_TIM_GPIO_AFx                GPIO_AF2_TIM3
#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER_TIM_IRQn                    TIM3_IRQn
#define ENCODER_TIM_IRQHANDLER              TIM3_IRQHandler
#endif
/* 使用A2 B2 */
#ifdef  Encode_2
#define ENCODER2_TIMx                        TIM4
#define ENCODER2_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM4_CLK_ENABLE()
#define ENCODER2_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM4_CLK_DISABLE()

#define ENCODER2_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define ENCODER2_TIM_CH1_PIN                 GPIO_PIN_12
#define ENCODER2_TIM_CH1_GPIO                GPIOD
#define ENCODER2_TIM_CH2_PIN                 GPIO_PIN_13
#define ENCODER2_TIM_CH2_GPIO                GPIOD
#define ENCODER2_TIM_GPIO_AFx                GPIO_AF2_TIM4
#define TIM4_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER2_TIM_IRQn                    TIM4_IRQn
#define ENCODER2_TIM_IRQHANDLER              TIM4_IRQHandler
#endif
/* 使用A3 B3 */
#ifdef  Encode_3
#define ENCODER_TIMx                        TIM1
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_8
#define ENCODER_TIM_CH1_GPIO                GPIOA
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_9
#define ENCODER_TIM_CH2_GPIO                GPIOA
#define ENCODER_TIM_GPIO_AFx                GPIO_AF1_TIM1
#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER_TIM_IRQn                    TIM1_CC_IRQn
#define ENCODER_TIM_IRQHANDLER              TIM1_CC_IRQHandler
#endif
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER               0  // 实际时钟频率为：36kHz

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ENCODER_TIM_PERIOD                  0xFFFF

#define ENCODER_FSPR              			      600     		// 编码器单圈线数
//#define MPR                         		      5       		// 丝杠单圈距离;单位：mm/r       
//#define PSPM      							              ((ENCODER_FSPR*4)/MPR)  		// 单位距离内的脉冲数;单位:P/mm

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_Encoder;
extern int16_t OverflowCount;
/* 函数声明 ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
