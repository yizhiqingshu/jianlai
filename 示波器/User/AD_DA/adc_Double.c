#include "adc_Double.h"
#include "lcd.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0};

float ADC_ConvertedValueLocal[NOFCHANEL]; 
/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = 	ADC_PIN1|
																		ADC_PIN2;
//																		|ADC_PIN3|
//																		ADC_PIN4|
//																		ADC_PIN5|
//																		ADC_PIN6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}

/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */

//static void TIM_1_Config(void)
//{
//TIM_TimeBaseInitTypeDef TIM_TimebaseStructure;
//TIM_OCInitTypeDef TIM_OCInitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
//	TIM_TimebaseStructure.TIM_Period=30000;
//	TIM_TimebaseStructure.TIM_Prescaler=7199;
//	TIM_TimebaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimebaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM1,&TIM_TimebaseStructure);

//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStructure.TIM_Pulse = 1000;
//	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
//	TIM_Cmd(TIM1,ENABLE);
//  TIM_CtrlPWMOutputs(TIM1, ENABLE); 

//}


void TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//ʱ��ʹ��

	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 1000; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =7199; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//�������:TIM����Ƚϼ��Ե�
	TIM_OC2Init(TIM2, & TIM_OCInitStructure);		//��ʼ������TIM2_CH2
	
	TIM_Cmd(TIM2, ENABLE); 			//ʹ��TIMx
	TIM_CtrlPWMOutputs(TIM2, ENABLE); 
}

static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	//��ʼ����ʱ��8
	TIM2_Init();
	
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);		//ʹ�ܴ�������ж�

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// �ر�����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;

	//��ʱ��2ͨ��2����   
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ������
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	
		
	// ��ʼ��ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
//	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC_x);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC_x));
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC_x);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC_x))
		;
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
		ADC_ExternalTrigConvCmd( ADC_x, ENABLE);
}

/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
uint16_t fangda=0;
uint16_t SET_I=100;
void ADC_DOUBLE_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
void  DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET){
		//�жϴ������
		  ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
			ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
//			ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
//			ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
//			ADC_ConvertedValueLocal[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
//			ADC_ConvertedValueLocal[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
//		
		   printf("\r\n CH0 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
			printf("\r\n CH1 value = %f mA \r\n",ADC_ConvertedValueLocal[1]);

//			printf("\r\n CH2 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
//			printf("\r\n CH3 value = %f V \r\n",ADC_ConvertedValueLocal[3]);
//			printf("\r\n CH4 value = %f V \r\n",ADC_ConvertedValueLocal[4]);
//			printf("\r\n CH5 value = %f V \r\n",ADC_ConvertedValueLocal[5]);
		
			printf("\r\n\r\n");
		//Delay_ms(1000);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}


/*********************************************END OF FILE**********************/
