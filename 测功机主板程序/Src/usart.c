/**
  ******************************************************************************
  * 文件名程: bsp_usartx.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-03-30
  * 功    能: 板载串口底层驱动程序
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "usart.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
UART_HandleTypeDef husart1;
UART_HandleTypeDef husart2;
UART_HandleTypeDef husart5;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART1x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART1x_Tx_GPIO_PIN|USART1x_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART1x_AFx;
    HAL_GPIO_Init(USART1x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART1x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART1x_Rx_GPIO, &GPIO_InitStruct);
  }
  if(huart->Instance==USART2x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART2x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART2x_Tx_GPIO_PIN|USART2x_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2x_AFx;
    HAL_GPIO_Init(USART2x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART2x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART2x_Rx_GPIO, &GPIO_InitStruct);    
  } 
	if(huart->Instance==USART5x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART5x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART5x_Tx_GPIO_PIN|USART5x_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART5x_AFx;
    HAL_GPIO_Init(USART5x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART5x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART5x_Rx_GPIO, &GPIO_InitStruct);    
  } 
}

/**
  * 函数功能: NVIC配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void MX_NVIC_USARTx_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1x_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART1x_IRQn);
  
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2x_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART2x_IRQn);
	
	HAL_NVIC_SetPriority(USART5x_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART5x_IRQn);
}


/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void MX_USARTx_Init(void)
{
  /* 配置串口中断并使能，需要放在HAL_UART_Init函数后执行修改才有效 */
  MX_NVIC_USARTx_Init();
  
  /* 串口外设时钟使能 */
  USART1_RCC_CLK_ENABLE();
  
  husart1.Instance = USART1x;
  husart1.Init.BaudRate = USART1x_BAUDRATE;
  husart1.Init.WordLength = UART_WORDLENGTH_8B;
  husart1.Init.StopBits = UART_STOPBITS_1;
  husart1.Init.Parity = UART_PARITY_NONE;
  husart1.Init.Mode = UART_MODE_TX_RX;
  husart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart1);


  /* 串口外设时钟使能 */
  USART2_RCC_CLK_ENABLE();
  
  husart2.Instance = USART2x;
  husart2.Init.BaudRate = USART2x_BAUDRATE;
  husart2.Init.WordLength = UART_WORDLENGTH_8B;
  husart2.Init.StopBits = UART_STOPBITS_1;
  husart2.Init.Parity = UART_PARITY_NONE;
  husart2.Init.Mode = UART_MODE_TX_RX;
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart2);
  

  /* 串口外设时钟使能 */
  USART5_RCC_CLK_ENABLE();
  
  husart5.Instance = USART5x;
  husart5.Init.BaudRate = USART5x_BAUDRATE;
  husart5.Init.WordLength = UART_WORDLENGTH_8B;
  husart5.Init.StopBits = UART_STOPBITS_1;
  husart5.Init.Parity = UART_PARITY_NONE;
  husart5.Init.Mode = UART_MODE_TX_RX;
  husart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart5);
	
}

/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart1,&ch, 1, 0xffff);
  return ch;
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
