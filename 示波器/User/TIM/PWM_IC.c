#include "PWM_IC.h"


static void ADVANCEX_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  �߼���ʱ��PWM�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCEX_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);	
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADVANCEX_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
  // ʹ��PWM����ģʽʱ����Ҫռ����������Ĵ�����һ�������ڣ�����һ����ռ�ձ�
	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	// ����ͨ��IC1����
	// ѡ�񲶻�ͨ��
  TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
	// ���ò���ı���
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// ���ò���ͨ�����ź��������ĸ�����ͨ������ֱ���ͷ�ֱ������
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1��Ƶ���������źŵ�ÿ����Ч���ض�����
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// ���˲�
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// ��ʼ��PWM����ģʽ
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// ��������PWM����ģʽʱ,ֻ��Ҫ���ô����źŵ���һ·���ɣ����ڲ������ڣ�
	// ����һ·�����ڲ���ռ�ձȣ�����Ӳ���Դ����ã�����Ҫ������
	
	// ����ͨ��IC2����	
//	TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// ѡ�����벶��Ĵ����ź�
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

	// ѡ���ģʽ: ��λģʽ
	// PWM����ģʽʱ,��ģʽ���빤���ڸ�λģʽ��������ʼʱ,������CNT�ᱻ��λ
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 

  // ʹ�ܲ����ж�,����ж���Ե���������ͨ�������������Ǹ���
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
	// ����жϱ�־λ
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
	 // ʹ�ܸ߼����ƶ�ʱ������������ʼ����
  TIM_Cmd(ADVANCE_TIM, ENABLE);
}
/**
  * @brief  �߼���ʱ��PWM�����ʼ�����õ���GPIO��ʼ��
  * @param  ��
  * @retval ��
  */
void ADVANCE_TIMX_Init(void)
{
	ADVANCEX_TIM_GPIO_Config();
	ADVANCEX_TIM_NVIC_Config();
	ADVANCEX_TIM_Mode_Config();		
}

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

/*
 * ����ǵ�һ���������жϣ��������ᱻ��λ�����浽CCR1�Ĵ�����ֵ��0��CCR2�Ĵ�����ֵҲ��0
 * �޷�����Ƶ�ʺ�ռ�ձȡ����ڶ��������ص�����ʱ��CCR1��CCR2���񵽵Ĳ�����Ч��ֵ������
 * CCR1��Ӧ�������ڣ�CCR2��Ӧ����ռ�ձȡ�
 */
void TIM1_CC_IRQHandler(void)
{
  /* ����жϱ�־λ */
  TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

  /* ��ȡ���벶��ֵ */
  IC1Value = TIM_GetCapture1(ADVANCE_TIM);
  IC2Value = TIM_GetCapture2(ADVANCE_TIM);
	
  // ע�⣺����Ĵ���CCR1��CCR2��ֵ�ڼ���ռ�ձȺ�Ƶ�ʵ�ʱ������1
	if (IC1Value != 0)
  {
    /* ռ�ձȼ��� */
    DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);

    /* Ƶ�ʼ��� */
    Frequency = (72000000/(ADVANCE_TIM_PSC+1))/(float)(IC1Value+1);
	  printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",DutyCycle,Frequency);

  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}

