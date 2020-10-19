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
#include "TS5303N616_1.h"
#include "tim.h"
//#include "TS5303N616_2.h"
//#include "TS5303N616_3.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
int16_t OverflowCount1 = 0;//��ʱ���������
//extern int16_t OverflowCount2;
//extern int16_t OverflowCount3;
//extern TIM_HandleTypeDef htim4_Encoder;
//extern TIM_HandleTypeDef htim1_Encoder;
/* Timer handler declaration */
TIM_HandleTypeDef    htim3_Encoder;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ENCODER1_TIMx_Init(void)
{    
  htim3_Encoder.Instance = ENCODER_TIMx;
  htim3_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htim3_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htim3_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htim3_Encoder);
  
  sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TIx;  
  
	sEncoderConfig.IC1Polarity = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC1Filter = 0;
  
  sEncoderConfig.IC2Polarity = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC2Filter = 0;
  __HAL_TIM_SET_COUNTER(&htim3_Encoder,0);
  
  HAL_TIM_Encoder_Init(&htim3_Encoder, &sEncoderConfig);
  
  //
  __HAL_TIM_CLEAR_IT(&htim3_Encoder, TIM_IT_UPDATE);  //��������жϱ�־λ
  __HAL_TIM_URS_ENABLE(&htim3_Encoder);               //���������������Ų��������ж�
  __HAL_TIM_ENABLE_IT(&htim3_Encoder,TIM_IT_UPDATE);  //ʹ�ܸ����ж�
  
  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    ENCODER_TIM_RCC_CLK_ENABLE();
    ENCODER_TIM_GPIO_CLK_ENABLE();     

    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��2��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    ENCODER_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
} 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3_Encoder)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3_Encoder))
    OverflowCount1--;       //���¼������
  else
    OverflowCount1++;       //���ϼ������
	}
  if(htim == &htimx)
	{
		time_count++;
	}

}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
