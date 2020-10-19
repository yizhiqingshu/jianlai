/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "usart2.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husart2;

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
  if(huart->Instance==USART2)
  {
    /* ʹ�ܴ��ڹ�������GPIOʱ�� */
    USART2_GPIO_ClK_ENABLE();
  
    /* �������蹦��GPIO���� */
    GPIO_InitStruct.Pin = USART2_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2_AFx;
    HAL_GPIO_Init(USART2_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART2_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART2_Rx_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ����Ӳ������ʼ������
  * �������: huart�����ھ������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART2)
  {
    /* ��������ʱ�ӽ��� */
    USART2_RCC_CLK_DISABLE();
  
    /* �������蹦��GPIO���� */
    HAL_GPIO_DeInit(USART2_Tx_GPIO, USART2_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(USART2_Rx_GPIO, USART2_Rx_GPIO_PIN);
    
    /* �����жϽ��� */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
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
  HAL_NVIC_SetPriority(USART2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void MX_USART2_Init(void)
{
  /* ��������ʱ��ʹ�� */
  USART2_RCC_CLK_ENABLE();

  husart2.Instance = USART2;
  husart2.Init.BaudRate = USART2_BAUDRATE;
  husart2.Init.WordLength = UART_WORDLENGTH_8B;
  husart2.Init.StopBits = UART_STOPBITS_2;
  husart2.Init.Parity = UART_PARITY_NONE;
  husart2.Init.Mode = UART_MODE_TX_RX;
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart2);
  
  /* ���ô����жϲ�ʹ�ܣ���Ҫ����HAL_UART_Init������ִ���޸Ĳ���Ч */
  MX_NVIC_USARTx_Init();
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
