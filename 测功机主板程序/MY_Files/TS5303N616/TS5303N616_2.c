/**
  ******************************************************************************
  * �ļ�����: bsp_EncoderTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-6-06
  * ��    ��: ������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "TS5303N616_2.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
int16_t OverflowCount2 = 0;//��ʱ���������
/* Timer handler declaration */
TIM_HandleTypeDef    htim4_Encoder;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoder2Config;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ENCODER2_TIMx_Init(void)
{    
  htim4_Encoder.Instance = ENCODER2_TIMx;
  htim4_Encoder.Init.Prescaler = ENCODER2_TIM_PRESCALER;
  htim4_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4_Encoder.Init.Period = ENCODER2_TIM_PERIOD;
  htim4_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htim4_Encoder);
  
  sEncoder2Config.EncoderMode = TIM4_ENCODERMODE_TIx;  
  
	sEncoder2Config.IC1Polarity = TIM_ICPOLARITY_RISING;   
  sEncoder2Config.IC1Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoder2Config.IC1Prescaler = TIM_ICPSC_DIV1; 
  sEncoder2Config.IC1Filter = 0;
  
  sEncoder2Config.IC2Polarity = TIM_ICPOLARITY_RISING;   
  sEncoder2Config.IC2Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoder2Config.IC2Prescaler = TIM_ICPSC_DIV1; 
  sEncoder2Config.IC2Filter = 0;
  __HAL_TIM_SET_COUNTER(&htim4_Encoder,0);
  
  HAL_TIM_Encoder_Init(&htim4_Encoder, &sEncoder2Config);
  
  //
  __HAL_TIM_CLEAR_IT(&htim4_Encoder, TIM_IT_UPDATE);  //��������жϱ�־λ
  __HAL_TIM_URS_ENABLE(&htim4_Encoder);               //���������������Ų��������ж�
  __HAL_TIM_ENABLE_IT(&htim4_Encoder,TIM_IT_UPDATE);  //ʹ�ܸ����ж�
  
  HAL_NVIC_SetPriority(ENCODER2_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER2_TIM_IRQn);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder2_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER2_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    ENCODER2_TIM_RCC_CLK_ENABLE();
    ENCODER2_TIM_GPIO_CLK_ENABLE();     

    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER2_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER2_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER2_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��2��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER2_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER2_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER2_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder2_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER2_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    ENCODER2_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER2_TIM_CH1_GPIO, ENCODER2_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER2_TIM_CH2_GPIO, ENCODER2_TIM_CH2_PIN);
  }
} 
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4_Encoder))
    OverflowCount2--;       //���¼������
  else
    OverflowCount2++;       //���ϼ������
}*/
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
