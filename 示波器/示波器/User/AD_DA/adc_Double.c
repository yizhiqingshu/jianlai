#include "adc_Double.h"
#include "lcd.h"
#include "arm_math.h"
#include "table_fft.h"
#include "stm32_dsp.h" 
__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0};
float fft_src1[256];
uint16_t fft_src2[256];
float fft_dst[FFT_LENGTH];
float fft_fre[FFT_LENGTH/2];
long fuzhi[FFT_LENGTH/2];
float ADC_ConvertedValueLocal[NOFCHANEL]; 


//   long��32λ

long InBufArray[NPT]={0};         //������������
long OutBufArray[NPT/2];          //�����������
long MagBufArray[NPT/2];          //��ֵ

/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */

void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (OutBufArray[i] << 16) >> 16;
        lY  = (OutBufArray[i] >> 16);
        
        //����32768�ٳ�65536��Ϊ�˷��ϸ������������
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;
        if(i == 0)
            MagBufArray[i] = (unsigned long)(Mag * 32768);
        else
            MagBufArray[i] = (unsigned long)(Mag * 65536);   //MagBufArray[]Ϊ��������ķ�ֵ����
    }
}

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


void TIM2_Init(uint16_t Per,uint16_t PSC,uint16_t Pluse)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 		//ʱ��ʹ��

	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = Per; 		//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =PSC; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = Pluse;
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
	
	
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );
	
	// �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;//(u32)fft_src1;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = 2;
	
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
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_1Cycles5);
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
uint16_t fft_i=0;
void  DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET){
		//�жϴ������
//		    ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
//			ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
//			ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
//			ADC_ConvertedValueLocal[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
//			ADC_ConvertedValueLocal[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
//			ADC_ConvertedValueLocal[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
//		  
DMA_Cmd(DMA1_Channel1, DISABLE);
        uint16_t i=0;
         if(fft_i<256)
        {  
        fft_i++;
        InBufArray[fft_i]=ADC_ConvertedValue[0]<< 16;
        InBufArray[fft_i]=ADC_ConvertedValue[1]<< 16;  
        }
//    for(i=0;i<NPT;i++)
//	InBufArray[i] = ((signed short)(ADC_ConvertedValue[i])) << 16;  //����16λ����16λ���ʵ��

        else if(fft_i==256)
        { 
            for(int index=0;index<256;index++)
            {
//                printf("index->%d fft_src->%d\n",index,fft_src1[index]);
//               printf("fft_src2->%d\n",fft_src2[index]);
//              printf("index %d fft_fre->%f\n",index,fft_fre[index]);
            }
        cr4_fft_256_stm32(OutBufArray, InBufArray, NPT);
        GetPowerMag();	                      //��ȡ�ź�г�������ķ�ֵ
        DMA_Cmd(DMA1_Channel1, ENABLE);    
        fft_i=0;
        }

		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}

float *FFT(float fft_src[],
float fft_dst[],
float fft_fre[])
{
	uint32_t i_fft;
	arm_status Status;
	arm_rfft_fast_instance_f32 P_S;
	Status = arm_rfft_fast_init_f32(&P_S,FFT_LENGTH);
	if(Status!=ARM_MATH_SUCCESS)
	{
	
	}
	arm_rfft_fast_f32(&P_S,fft_src,fft_dst,FFT_type_fft);
	for(i_fft=0;i_fft<FFT_LENGTH;i_fft+=2)
	{
	fft_fre[i_fft]=sqrt(fft_dst[i_fft] * fft_dst[i_fft] + fft_dst[i_fft+1] * fft_dst[i_fft+1]);
	}
    return fft_fre;
}


/*********************************************END OF FILE**********************/
