#ifndef __DAC_H__
#define	__DAC_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* 定义DAC通道引脚，这里使用PA4 */
#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA

#define DACx_CHANNEL2_PIN                GPIO_PIN_5
#define DACx_CHANNEL2_GPIO_PORT          GPIOA

/* 定义DAC通道，这里使用通道1 */
#define DACx_CHANNEL1                    DAC_CHANNEL_1
#define DACx_CHANNEL2                    DAC_CHANNEL_2

/* 扩展变量 ------------------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
/* 函数声明 ------------------------------------------------------------------*/
void MX_DAC_Init(void);

#endif /* __DAC_H */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
