#ifndef _USARTX_H__
#define _USARTX_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
//#define USAR                                 USART2
#define USART2_BAUDRATE                        9600
#define USART2_RCC_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define USART2_RCC_CLK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()

#define USART2_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2_Tx_GPIO_PIN                     GPIO_PIN_5
#define USART2_Tx_GPIO                         GPIOD
#define USART2_Rx_GPIO_PIN                     GPIO_PIN_6   
#define USART2_Rx_GPIO                         GPIOD

#define USART2_AFx                             GPIO_AF7_USART2

#define UARTX_IRQHANDLER                       USART2_IRQHandler
#define USART2_IRQn                            USART2_IRQn


/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart2;

/* 函数声明 ------------------------------------------------------------------*/
void MX_USART2_Init(void);


#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
