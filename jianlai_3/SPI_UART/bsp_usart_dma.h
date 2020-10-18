/********************************************************************************************************
**                      
**------------------------------------------------------------------------------------------------------
** 文件: usart.h 
** 版本: v1.0
**------------------------------------------------------------------------------------------------------
** 描述:
**      串口模块
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef __USART2_H
#define __USART2_H

#include "stm32f10x.h"
#include "JIANLAI_.h"
extern uint8_t  Touch_tx_buffer[16];   /* 串口发送缓存 */
extern uint8_t  Touch_rx_buffer[16];   /* 串口接受缓存 */
extern uint16_t Touch_tx_flag;           /* 接收一帧数据标志，并记录接收数据个数 */


#define UART_X 			   	USART2
#define UART_X_RCCcmd      	RCC_APB1Periph_USART2
#define UART_2_APB1		   	RCC_APB1PeriphClockCmd
#define UART_X_GPIOrcc     	RCC_APB2Periph_GPIOA
#define UART_X_GPIOAF_rcc   RCC_APB2Periph_AFIO
#define UART_X_GPIO_PINTX	GPIO_Pin_2
#define UART_X_GPIO_PINRX	GPIO_Pin_3
#define	UART_X_NVICn	 	USART2_IRQn	
#define UART_X_DMA_TXCH		DMA1_Channel7
#define UART_X_DMA_RXCH		DMA1_Channel6
#define UART_X_HANDLER		USART2_IRQHandler


#if 0
#define UART_X 			   	USART1
#define UART_X_RCCcmd      	RCC_APB2Periph_USART1
#define UART_2_APB1		   	RCC_APB2PeriphClockCmd
#define UART_X_GPIOrcc     	RCC_APB2Periph_GPIOA
#define UART_X_GPIOAF_rcc   RCC_APB2Periph_AFIO
#define UART_X_GPIO_PINTX	GPIO_Pin_9
#define UART_X_GPIO_PINRX	GPIO_Pin_10
#define	UART_X_NVICn	 	USART1_IRQn	
#define UART_X_DMA_TXCH		DMA1_Channel4
#define UART_X_DMA_RXCH		DMA1_Channel5
#define UART_X_HANDLER		USART1_IRQHandler

#endif




int fputc(int ch, FILE *p);
// 对外函数声明

void USART2_Init(uint32_t bound);
void USART2_DMA_Init(void);
void Delay_us(u32 i);
void Delay_ms(u32 i);

void Openmv_Data(void);


//

void OPenMV_ARR(uint8_t* ARRDATA);
//
#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

