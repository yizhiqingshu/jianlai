#include "JIANLAI_.h"
#include "PWM_IC.h"
#include "PWM.h"
#include "bmp.h"
extern int16_t OpenMV_X;          /*OPENMV X �ᷴ������*/
extern int16_t OpenMV_Y;          /*OPENMV X �ᷴ������*/
extern int16_t OpenMV_X1;          /*OPENMV X �ᷴ������*/
extern int16_t OpenMV_Y1;          /*OPENMV X �ᷴ������*/
 extern uint8_t ARR_OPENMV[7];


//���벶��
#if TIM1_CH1_CH2_IC
extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;
#endif

int main()
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	#if UART2_PRINTF
	USART2_Init(115200);
	USART2_DMA_Init();
	    printf("STM32+USART+DMA+IDLE\n");
	#endif
	#if OLED_spi
	u8 t;
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear()  	; 
	t=' ';
	#endif

	#if NRF2401_RX
	uint8_t tmp_buf[33]="Yingshi NRF24L01 Test";
	NRF24L01_Init();    		//��ʼ��NRF24L01
  
  while(NRF24L01_Check())
	{
    printf("zhao bu dao NRF24L01\n"); 
		Delay_ms(1000);
	}
  printf("NRF24L01lian jie zhengchang \n");
  NRF24L01_RX_Mode();
	#endif
	#if NRF2401_TX
    NRF24L01_Init();    		//��ʼ��NRF24L01
	uint8_t tmp_buf[33]="Ӳʯ������ NRF24L01 ʵ��";

	while(NRF24L01_Check())
	{
    printf("Ӳ����Ѱ����NRF24L01����ģ��\n"); 
	Delay_ms(1000);
	}
  printf("NRF24L01����ģ��Ӳ����������\n");
  
  NRF24L01_TX_Mode();
	#endif
   
	#if LCD_INIT
	LCD_Init();
	POINT_COLOR=RED;
	#endif
	#if TIM3_PWM_CH1_4
	TIMx_PWM_Init(999,719);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	#endif
	#if DAC_INIT 
	DAC_1_Config();
	#endif
//Dac1_Set_Vol(1000);//dac��ʼ��ֵ 1000=1V
  #if TIM1_CH1_CH2_IC
  ADVANCE_TIMX_Init();
  #endif
while(1)
{
	OLED_Clear();

//		OLED_ShowCHinese(54,0,3);//��
//		OLED_ShowCHinese(72,0,4);//��
//		OLED_ShowCHinese(90,0,5);//��
//		OLED_ShowCHinese(108,0,6);//��
//		OLED_ShowString(0,3,"OLED TEST");
//		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
//	 //	OLED_ShowString(20,4,"2014/05/01");  
//		OLED_ShowString(0,6,"ASCII:");  
//		OLED_ShowString(63,6,"CODE:");  
//		OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
//		t++;
//		if(t>'~')t=' ';
//		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ
	
	
		OLED_ShowString(0,0,"OpenMV_X1:");
		OLED_ShowString(0,2,"OpenMV_Y1:");
		OLED_ShowString(0,4,"OpenMV_X:");
		OLED_ShowString(0,6,"OpenMV_Y:");  
		OLED_ShowNum(80,0,OpenMV_X1,3,16);//��ʾASCII�ַ�����ֵ 
		OLED_ShowNum(80,2,OpenMV_Y1,3,16);//��ʾASCII�ַ�����ֵ
		OLED_ShowNum(70,4,OpenMV_X,3,16);//��ʾASCII�ַ�����ֵ 
		OLED_ShowNum(70,6,OpenMV_Y,3,16);//��ʾASCII�ַ�����ֵ 		 		
		Delay_ms(8000);
//		Delay_ms(8000);
//		OLED_Clear();
//		Delay_ms(8000);
//		OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
//		Delay_ms(8000);
//		OLED_DrawBMP(0,0,128,8,BMP2);
//		Delay_ms(8000);
/* �жϽ��������Ƿ���� */
//		if(Touch_tx_flag != 0)
//		{
//			/* �ȹر�DMA����ͨ�� */
//			DMA_Cmd(DMA1_Channel7,DISABLE);
//			/* �����־ */
//			DMA_ClearFlag(DMA1_FLAG_TC7);
//			/* ����DMA���ͳ��� */
//			DMA_SetCurrDataCounter(DMA1_Channel7,Touch_tx_flag);
//			/* ʹ��DMA����ͨ�� */
//			DMA_Cmd(DMA1_Channel7,ENABLE);
//			/* �ȴ�������ɣ�����û��ʹ��DMA��������ж� */
//			while(!DMA_GetFlagStatus(DMA1_FLAG_TC7));

//			/* ���������ɱ�־ */
//			Touch_tx_flag = 0;
//		}
	
	#if UART2_PRINTF
	if(Touch_tx_flag!=0)
	{
	printf("x_pianyi=%d\n",OpenMV_X);
	printf("y_pianyi=%d\n",OpenMV_Y);
	printf("x1_pianyi=%d\n",OpenMV_X1);
	printf("y1_pianyi=%d\n",OpenMV_Y1);
	Touch_tx_flag = 0;
	}
	#endif
		 #if TIM3_PWM_CH1_4
TIM_SetCompare1(TIM3,499);
TIM_SetCompare2(TIM3,100);
TIM_SetCompare3(TIM3,499);
TIM_SetCompare4(TIM3,10);
		printf("ռ�ձȣ�%0.2f%%   Ƶ�ʣ�%0.2fHz\n",DutyCycle,Frequency);
	  printf("chuan");	
	Delay_ms(100);
#endif
	
	#if NRF2401_RX
	
	if(NRF24L01_RxPacket(tmp_buf)==0)
    {
      tmp_buf[32]=0;//�����ַ���������      
      printf("NRF24L01jie shou chenggong ��%s\n",tmp_buf);
    }
    Delay_ms(10);
	#endif
	#if NRF2401_TX
	  
    Delay_ms(1000);  
    if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
    {
      printf("NRF24L01����ģ�����ݷ��ͳɹ���%s\n",tmp_buf);
    }
    else
    {
      printf("NRF24L01����ģ�����ݷ���ʧ��\n");
    }     
	#endif

}
}

