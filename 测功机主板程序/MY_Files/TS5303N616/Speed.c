#include "Speed.h"
#include "main.h"
#include "TS5303N616_3.h"
#include "dac.h"
#include "stm_flash.h"
#include "usart.h"
#include "opt.h"
#include "init.h"
#include "netif.h"
#include "timeouts.h"
#include "etharp.h"
#include "ethernetif.h"
#include "app_ethernet.h"
#include "tcp_echoclient.h"
#include "ZLG.h"
#include "tim.h"
#include "gpio.h"
#include "MY_Talk.h"
extern struct netif gnetif;

//extern uint8_t i;
#define abs(x)                    ((x)<0?(-x):(x))
#define abs(x)                    ((x)<0?(-x):(x))

#define ENCODER     11    // ����������
#define SPEEDRATIO  1   // ������ٱ�
#define PPR         (SPEEDRATIO*ENCODER*4) // Pulse/r ÿȦ�ɲ����������

uint8_t count;

motor Speed3;

uint16_t time_count1;
uint16_t time_count4=0;
uint8_t time_count5=0;
uint8_t time_count3=0;
double T_IN,Rh_IN,P_IN,S_IN,T1_IN,T2_IN,T3_IN;	//��ADCÿ��ͨ����Ӧ����
uint32_t ADC_ConvertedValue[ADC_CHANNEL_NUMBER];
double ADC_ConvertedValueLocal[ADC_CHANNEL_NUMBER];
double T[2],Rh[2],P[2],T1[2],T2[2],T3[2];	//��ADCÿ��ͨ����Ӧ����,��Ϊ�м��������ֵ���ݸ� T_IN

double low_filter(double low_buf[]);

void Speed_Init(void)
{
	ENCODER_TIMx_Init();
	HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_1);	//�����������ӿ�A1
	HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_2);	//�����������ӿ�B1
	printf("����������\n");
}

void Get_Speed(void)
{
	if(time_count>=20)	// �����Ϳ�������Ϊ20ms
	{   
		Speed3.CaptureNumber = OverflowCount*65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);	// ��ñ�����������ֵ
      Speed3.MSF = Speed3.CaptureNumber  - Speed3.Last_CaptureNumber; // M�� ���ٶ� M���ǲ�����λʱ���ڵ�������Ȼ�����Ƶ�ʣ����ﲻ��Ƶ��
      Speed3.Last_CaptureNumber = Speed3.CaptureNumber;  
      Speed3.MSF = abs(Speed3.MSF);
      Speed3.SUM_Pulse += Speed3.MSF;	//���ٶȽ����ۼ�,�õ�1s�ڵ�������

		time_count=0;	//��ʱ����ʱ��1ms
		time_count1++;	
		time_count2++;	//�������ڷ������ݶ�ʱ
		time_count3++;	//������Ļ��ʾ���ݣ���ʱ������ʾ����
		time_count4++;	//���ڳ�������ָʾ�ƣ�LED1  1s��˸һ��
		time_count5++;
	}
	if(time_count1==50)		//����Ϊ1s���м���
	{
		Speed3.Speed = (double)Speed3.SUM_Pulse/PPR;
		printf("1s������������Ϊ��%d \n",Speed3.SUM_Pulse);
		Speed3.SUM_Pulse=0;
		time_count1=0;
		LED1_Toggle;	//������������ָʾ��
	}
}

void ADC_DAC_Init(void)
{
	MX_DMA_Init();
  MX_ADCx_Init();
  HAL_ADC_Start_DMA(&hadcx,ADC_ConvertedValue,ADC_CHANNEL_NUMBER);  //����ADת����ʹ��DMA������ж� 
	MX_DAC_Init();
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL1, DAC_ALIGN_12B_R, 0);	//����DACͨ��ֵ
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL2, DAC_ALIGN_12B_R, 0);	
	HAL_DAC_Start(&hdac, DACx_CHANNEL1);	//����DAC���
  HAL_DAC_Start(&hdac, DACx_CHANNEL2);
}

void Get_ADC(void)
{
	uint8_t i;
	for(i=0;i<ADC_CHANNEL_NUMBER;i++)
    {
			//�����˲�����ת����ѹ����ֵ����ص�����������д��޸�  ������ת��Ϊ��ѹ�ڽ����˲�����
      ADC_ConvertedValueLocal[i] =(double)(ADC_ConvertedValue[i])/*&0xFFF)*3.3/4096*/; //ȡ���12��Ч���ݲ�תΪ0-3.3Vֱ�ӵĵ�ѹ
    }
		for(i=0;i<ADC_CHANNEL_NUMBER;)
		{
			T[count]=ADC_ConvertedValueLocal[i++];
			Rh[count]=ADC_ConvertedValueLocal[i++];
			P[count]=ADC_ConvertedValueLocal[i++];
			T1[count]=ADC_ConvertedValueLocal[i++];
			T2[count]=ADC_ConvertedValueLocal[i++];
			T3[count]=ADC_ConvertedValueLocal[i++];
			count++;
		}
		if(count==2)
		{
			T_IN=low_filter(T);
			Rh_IN=low_filter(Rh);
			P_IN=low_filter(P);
			T1_IN=low_filter(T1);
			T2_IN=low_filter(T2);
			T3_IN=low_filter(T3);
			//T=Rh=P=T1=T2=T3=0;
			count=0;
		}
}
//��ͨ�˲�
double low_filter(double low_buf[])  
{  
	double sample_value;  
	double X=0.01;  
	sample_value=(1-X)*low_buf[1]+X*low_buf[0];  
	return sample_value; 
}  
/**
  * ��������: ��������ӿ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  
  /* Initializes the dynamic memory heap defined by MEM_SIZE.*/
  mem_init();
  
  /* Initializes the memory pools defined by MEMP_NUM_x.*/
  memp_init();  
#ifdef USE_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
  
	printf("��̬IP��ַ........................%d.%d.%d.%d\r\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	printf("��������..........................%d.%d.%d.%d\r\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	printf("Ĭ������..........................%d.%d.%d.%d\r\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  
  /* Add the network interface */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
  /* Registers the default network interface */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    printf("�ɹ���������\n");
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }
  
  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
}
void Device_Init(void)
{
	MX_GPIO_Init();
	LED1_ON;
	LED2_ON;
	MX_USARTx_Init();    //��ʼ������1�����ô����ж����ȼ� 
	HAL_UART_Receive_IT(&husart1,&aRxBuffer[0],1);
  HAL_UART_Receive_IT(&husart2,&aRxBuffer[1],1);
	HAL_UART_Receive_IT(&husart5,&aRxBuffer[2],1);
	
	sprintf((char*)&TxBuf1,"����ͨѶ�ɹ�\n");    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf3,"RS232ͨѶ�ɹ�\n");    
	HAL_UART_Transmit(&husart5,TxBuf3,strlen((char *)TxBuf3),1000);
	
	lwip_init();
  Netif_Config();
  User_notification(&gnetif);
	Speed_Init();
	ADC_DAC_Init();
	ZLG_Uart_Init();
	BASIC_TIMx_Init();	//1ms��ʱ�ж�
  HAL_TIM_Base_Start_IT(&htimx);//���ж�ģʽ��������ʱ�� 
}

