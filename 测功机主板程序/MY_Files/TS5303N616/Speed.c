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

#define ENCODER     11    // 编码器线数
#define SPEEDRATIO  1   // 电机减速比
#define PPR         (SPEEDRATIO*ENCODER*4) // Pulse/r 每圈可捕获的脉冲数

uint8_t count;

motor Speed3;

uint16_t time_count1;
uint16_t time_count4=0;
uint8_t time_count5=0;
uint8_t time_count3=0;
double T_IN,Rh_IN,P_IN,S_IN,T1_IN,T2_IN,T3_IN;	//与ADC每个通道对应起来
uint32_t ADC_ConvertedValue[ADC_CHANNEL_NUMBER];
double ADC_ConvertedValueLocal[ADC_CHANNEL_NUMBER];
double T[2],Rh[2],P[2],T1[2],T2[2],T3[2];	//与ADC每个通道对应起来,作为中间变量，把值传递给 T_IN

double low_filter(double low_buf[]);

void Speed_Init(void)
{
	ENCODER_TIMx_Init();
	HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_1);	//启动编码器接口A1
	HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_2);	//启动编码器接口B1
	printf("启动编码器\n");
}

void Get_Speed(void)
{
	if(time_count>=20)	// 采样和控制周期为20ms
	{   
		Speed3.CaptureNumber = OverflowCount*65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);	// 获得编码器的脉冲值
      Speed3.MSF = Speed3.CaptureNumber  - Speed3.Last_CaptureNumber; // M法 测速度 M法是测量单位时间内的脉数，然后换算成频率，这里不算频率
      Speed3.Last_CaptureNumber = Speed3.CaptureNumber;  
      Speed3.MSF = abs(Speed3.MSF);
      Speed3.SUM_Pulse += Speed3.MSF;	//对速度进行累计,得到1s内的脉冲数

		time_count=0;	//定时器计时，1ms
		time_count1++;	
		time_count2++;	//用于网口发送数据定时
		time_count3++;	//用于屏幕显示数据，定时发送显示数据
		time_count4++;	//用于程序运行指示灯，LED1  1s闪烁一次
		time_count5++;
	}
	if(time_count1==50)		//周期为1s进行计算
	{
		Speed3.Speed = (double)Speed3.SUM_Pulse/PPR;
		printf("1s内数到的脉冲为：%d \n",Speed3.SUM_Pulse);
		Speed3.SUM_Pulse=0;
		time_count1=0;
		LED1_Toggle;	//程序正常运行指示灯
	}
}

void ADC_DAC_Init(void)
{
	MX_DMA_Init();
  MX_ADCx_Init();
  HAL_ADC_Start_DMA(&hadcx,ADC_ConvertedValue,ADC_CHANNEL_NUMBER);  //启动AD转换并使能DMA传输和中断 
	MX_DAC_Init();
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL1, DAC_ALIGN_12B_R, 0);	//设置DAC通道值
	HAL_DAC_SetValue(&hdac, DACx_CHANNEL2, DAC_ALIGN_12B_R, 0);	
	HAL_DAC_Start(&hdac, DACx_CHANNEL1);	//启动DAC输出
  HAL_DAC_Start(&hdac, DACx_CHANNEL2);
}

void Get_ADC(void)
{
	uint8_t i;
	for(i=0;i<ADC_CHANNEL_NUMBER;i++)
    {
			//先做滤波，在转化电压，把值给相关的输入变量，有待修改  或者先转换为电压在进行滤波处理
      ADC_ConvertedValueLocal[i] =(double)(ADC_ConvertedValue[i])/*&0xFFF)*3.3/4096*/; //取最低12有效数据并转为0-3.3V直接的电压
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
//低通滤波
double low_filter(double low_buf[])  
{  
	double sample_value;  
	double X=0.01;  
	sample_value=(1-X)*low_buf[1]+X*low_buf[0];  
	return sample_value; 
}  
/**
  * 函数功能: 配置网络接口
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
  
	printf("静态IP地址........................%d.%d.%d.%d\r\n",IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
	printf("子网掩码..........................%d.%d.%d.%d\r\n",NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
	printf("默认网关..........................%d.%d.%d.%d\r\n",GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* USE_DHCP */
  
  /* Add the network interface */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
  /* Registers the default network interface */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    printf("成功连接网卡\n");
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
	MX_USARTx_Init();    //初始化串口1并配置串口中断优先级 
	HAL_UART_Receive_IT(&husart1,&aRxBuffer[0],1);
  HAL_UART_Receive_IT(&husart2,&aRxBuffer[1],1);
	HAL_UART_Receive_IT(&husart5,&aRxBuffer[2],1);
	
	sprintf((char*)&TxBuf1,"串口通讯成功\n");    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf3,"RS232通讯成功\n");    
	HAL_UART_Transmit(&husart5,TxBuf3,strlen((char *)TxBuf3),1000);
	
	lwip_init();
  Netif_Config();
  User_notification(&gnetif);
	Speed_Init();
	ADC_DAC_Init();
	ZLG_Uart_Init();
	BASIC_TIMx_Init();	//1ms定时中断
  HAL_TIM_Base_Start_IT(&htimx);//在中断模式下启动定时器 
}

