/**
  ******************************************************************************
  * �ļ�����: bsp_usartx.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ���ش��ڵײ���������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "usart.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husart1;
UART_HandleTypeDef husart2;
UART_HandleTypeDef husart5;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ����Ӳ����ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1x)
  {
    /* ʹ�ܴ��ڹ�������GPIOʱ�� */
    USART1x_GPIO_ClK_ENABLE();
  
    /* �������蹦��GPIO���� */
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
    /* ʹ�ܴ��ڹ�������GPIOʱ�� */
    USART2x_GPIO_ClK_ENABLE();
  
    /* �������蹦��GPIO���� */
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
    /* ʹ�ܴ��ڹ�������GPIOʱ�� */
    USART5x_GPIO_ClK_ENABLE();
  
    /* �������蹦��GPIO���� */
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
  * ��������: NVIC����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_USARTx_Init(void)
{
  /* ���ô����жϲ�ʹ�ܣ���Ҫ����HAL_UART_Init������ִ���޸Ĳ���Ч */
  MX_NVIC_USARTx_Init();
  
  /* ��������ʱ��ʹ�� */
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


  /* ��������ʱ��ʹ�� */
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
  

  /* ��������ʱ��ʹ�� */
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
  * ��������: �ض���c�⺯��printf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * ��������: �ض���c�⺯��getchar,scanf��DEBUG_USARTx
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&husart1,&ch, 1, 0xffff);
  return ch;
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
