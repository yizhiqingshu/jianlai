#include "PWM.h"

#if TIM3_PWM_CH1_4
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

void TIMx_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(TIM_X_PWM_RCC, ENABLE);	//ʹ�ܶ�ʱ��xʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|TIM_GPIOC_RCC,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH1-4->PC6-PC9    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
	/* ������ӳ�� �ᱣ��Ĭ�϶˿� ȫ����ӳ�� ȫ��ӳ�䵽��Ӧ������ ���ᱣ��Ĭ�϶˿�
	RCC_APB1PeriphClockCmd(TIM_X_PWM_RCC, ENABLE);	//ʹ�ܶ�ʱ��xʱ��
 	RCC_APB2PeriphClockCmd(TIM_GPIOA_RCC |	TIM_GPIOB_RCC	| RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
    */
	
   //��ʼ��TIMx
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM_X_PWM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIMx Channel1-4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM_X_PWM,&TIM_OCInitStructure);
	TIM_OC2Init(TIM_X_PWM,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3Init(TIM_X_PWM,&TIM_OCInitStructure);
	TIM_OC4Init(TIM_X_PWM,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	TIM_OC1PreloadConfig(TIM_X_PWM, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM_X_PWM, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM_X_PWM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM_X_PWM, TIM_OCPreload_Enable);
	TIM_Cmd(TIM_X_PWM, ENABLE);  //ʹ��TIM3
 
}
#endif

#if TIM4_PWM_CH1_4 
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    /* ������ӳ�� �ᱣ��Ĭ�϶˿� ȫ����ӳ�� ȫ��ӳ�䵽��Ӧ������ ���ᱣ��Ĭ�϶˿�
    RCC_APB1PeriphClockCmd(TIM_4_PWM_RCC, ENABLE);	//ʹ�ܶ�ʱ��xʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|TIM_GPIOD_RCC,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer3������ӳ��  TIM3_CH1-4->PC6-PC9    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	*/
	
	RCC_APB1PeriphClockCmd(TIM_4_PWM_RCC, ENABLE);	//ʹ�ܶ�ʱ��xʱ��
 	RCC_APB2PeriphClockCmd(TIM_GPIOB_RCC| RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
    
	
   //��ʼ��TIMx
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM_4_PWM, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIMx Channel1-4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM_4_PWM,&TIM_OCInitStructure);
	TIM_OC2Init(TIM_4_PWM,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	TIM_OC3Init(TIM_4_PWM,&TIM_OCInitStructure);
	TIM_OC4Init(TIM_4_PWM,&TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	TIM_OC1PreloadConfig(TIM_4_PWM, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM_4_PWM, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM_4_PWM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM_4_PWM, TIM_OCPreload_Enable);
	TIM_Cmd(TIM_4_PWM, ENABLE);  //ʹ��TIM3
 
}

#endif
#if ADVANCE_TIM1
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ����Ƚ�ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

  // ����Ƚ�ͨ������ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);

  // ����Ƚ�ͨ��ɲ��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	// BKIN����Ĭ��������͵�ƽ
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);	
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
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
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

	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// �������ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	// ����ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// �������ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// �������ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------ɲ���������ṹ���ʼ��-------------------*/
	// �й�ɲ���������ṹ��ĳ�Ա����ɲο�BDTR�Ĵ���������
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// ����Ƚ��ź�����ʱ�����ã�������μ���ɲο� BDTR:UTG[7:0]������
	// �������õ�����ʱ��Ϊ152ns
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// ��BKIN���ż�⵽�ߵ�ƽ��ʱ������Ƚ��źű���ֹ���ͺ�����ɲ��һ��
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// ʹ�ܼ�����
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();		
}

#endif
