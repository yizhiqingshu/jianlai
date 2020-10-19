/**
  ******************************************************************************
  * �ļ�����: bsp_dac.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: DACʵ�ֵ�ѹ���
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
#include "dac.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
DAC_HandleTypeDef hdac;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: DAC��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void MX_DAC_Init(void)
{
  DAC_ChannelConfTypeDef sConfig;

  /* DAC��ʼ�� */
  hdac.Instance = DACx;
  HAL_DAC_Init(&hdac);

 /* DACͨ��������� */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DACx_CHANNEL1);
  
  HAL_DAC_ConfigChannel(&hdac, &sConfig, DACx_CHANNEL2);
}

/**
  * ��������: DAC��ʼ������
  * �������: hdac��DAC����������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hdac->Instance==DACx)
  {
    /* DAC����ʱ��ʹ�� */
    DACx_CLK_ENABLE();
    
    /* DACͨ�����Ŷ˿�ʱ��ʹ�� */
    DACx_CHANNEL_GPIO_CLK_ENABLE();
    
    /* DACͨ���������� */
    GPIO_InitStruct.Pin = DACx_CHANNEL1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = DACx_CHANNEL2_PIN;
    HAL_GPIO_Init(DACx_CHANNEL2_GPIO_PORT, &GPIO_InitStruct);
  }
}

/**
  * ��������: DAC����ʼ������
  * �������: hdac��DAC����������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{

  if(hdac->Instance==DAC)
  {
    /* ��λDAC */
    DACx_FORCE_RESET();
    DACx_RELEASE_RESET();
    
    /* ����DAC����ʱ�� */
    __HAL_RCC_DAC_CLK_DISABLE();
  
    /* ����ʼ��DACͨ������ */
    HAL_GPIO_DeInit(DACx_CHANNEL1_GPIO_PORT, DACx_CHANNEL1_PIN);
    HAL_GPIO_DeInit(DACx_CHANNEL2_GPIO_PORT, DACx_CHANNEL2_PIN);
  }
} 

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
