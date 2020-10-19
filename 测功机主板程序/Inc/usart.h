#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/* 选择USART1作为通信串口 */

#define USART1x                                 USART1
#define USART1x_BAUDRATE                        115200
#define USART1_RCC_CLK_ENABLE()                 __HAL_RCC_USART1_CLK_ENABLE()
#define USART1_RCC_CLK_DISABLE()                __HAL_RCC_USART1_CLK_DISABLE()
#define USART1x_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART1x_Tx_GPIO_PIN                     GPIO_PIN_6
#define USART1x_Tx_GPIO                         GPIOB
#define USART1x_Rx_GPIO_PIN                     GPIO_PIN_7   
#define USART1x_Rx_GPIO                         GPIOB
#define USART1x_AFx                             GPIO_AF7_USART1
#define USART1x_IRQHANDLER                      USART1_IRQHandler
#define USART1x_IRQn                            USART1_IRQn

/* 选择USART2作为通信串口 */
#define USART2x                                 USART2
#define USART2x_BAUDRATE                        9600
#define USART2_RCC_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define USART2_RCC_CLK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()
#define USART2x_GPIO_ClK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART2x_Tx_GPIO_PIN                     GPIO_PIN_5
#define USART2x_Tx_GPIO                         GPIOD
#define USART2x_Rx_GPIO_PIN                     GPIO_PIN_6   
#define USART2x_Rx_GPIO                         GPIOD
#define USART2x_AFx                             GPIO_AF7_USART2
#define USART2x_IRQHANDLER                      USART2_IRQHandler
#define USART2x_IRQn                            USART2_IRQn

#define USART5x                                 UART5
#define USART5x_BAUDRATE                        115200
#define USART5_RCC_CLK_ENABLE()                 __HAL_RCC_UART5_CLK_ENABLE()
#define USART5_RCC_CLK_DISABLE()                __HAL_RCC_UART5_CLK_DISABLE()
#define USART5x_GPIO_ClK_ENABLE()               {__HAL_RCC_GPIOC_CLK_ENABLE();__HAL_RCC_GPIOD_CLK_ENABLE();}
#define USART5x_Tx_GPIO_PIN                     GPIO_PIN_12
#define USART5x_Tx_GPIO                         GPIOC
#define USART5x_Rx_GPIO_PIN                     GPIO_PIN_2
#define USART5x_Rx_GPIO                         GPIOD
#define USART5x_AFx                             GPIO_AF8_UART5
#define USART5x_IRQHANDLER                      UART5_IRQHandler
#define USART5x_IRQn                            UART5_IRQn

/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husart1;
extern UART_HandleTypeDef husart2;
extern UART_HandleTypeDef husart5;

/* 函数声明 ------------------------------------------------------------------*/
void MX_USARTx_Init(void);


#endif  /* __BSP_USARTX_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
