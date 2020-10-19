/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "contorl.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "ZLG.h"
#include "MY_Talk.h"
#include "Speed.h"
#include "stm_flash.h"
#include "gpio.h"
#include "MQW5102.h"
#include "50B.h"
#include "VMT_2000.h"
#include "app_ethernet.h"
#include "ethernetif.h"
#include "timeouts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/***********
 我的变量
***********/
uint8_t i;
//RS232
struct netif gnetif;
/***********
与DAC相关变量
***********/
uint16_t dac_value;
/***********
与串口相关变量
***********/
uint8_t aRxBuffer[3];
uint8_t TxBuf1[256];	//串口通讯
uint8_t RxBuf1[256];	
uint8_t TxBuf2[256];	//串口屏通讯
uint8_t RxBuf2[256];
uint8_t TxBuf3[256];	//RS232通讯
uint8_t RxBuf3[256];
uint8_t Rx_MSG = 0;   // 接收报文状态
uint8_t RS232_Rxbuf[256]={0}; // 接收缓冲区
uint8_t RS232_flag;
uint8_t UART_count1=0;
uint8_t UART_count2=0;
uint8_t UART_count3=0;
uint8_t usart_rx_flag;	//用于判断接收是否接收
/* USER CODE END PTD */
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
                      
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Rs232_Read(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */
 
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  
  /* USER CODE BEGIN Init */
	HAL_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
	
  /* USER CODE BEGIN SysInit */
	/****************************/
	zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0,  0x95, cmd_buf);	//测试ZLG,串口0发送数据0x95,还未对ZLG串口进行初始化
	/****************************/
	Device_Init();	//初始化串口1/2、cpu内部flash、初始化网口、编码器、ADC、DAC、扩展串口
	stm_flash_Rx();	//读取cpu内部的PID参数
	HAL_Delay(1000);
	LED1_OFF;
	LED2_OFF;
//	s2u_configure_one_uart(UART_CHAN_0);  //初始化扩展串口0
	
//	/***********

  /* USER CODE END SysInit */
			
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
   /* USER CODE END WHILE */
		ethernetif_input(&gnetif);
    sys_check_timeouts();
		Get_ADC();	//通过DMA依次读取AD通道的值以及进行均值滤波处理,采集速度很快
		Get_Speed();	//编码器采集脉冲以及计算 r/s
		contorl();
		Tcp_send_car(); 	//在测量时网口实时发送测量数据
		Rs232_Read();
		HMI_show();				//在测量时实时显示测量数据
		/***********
		用户程序结束
		***********/
#ifdef USE_DHCP
    /* LwIP需要定时处理的函数 */
    DHCP_Periodic_Handle(&gnetif);    
#endif  
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance==USART1)
		{
			if((RxBuf1[0]==0x4A)&&(aRxBuffer[0]==0x3B))    // 接收到0x3B，代表数据结束
			{
				if((RxBuf1[1]==0x01)&&(RxBuf1[2]==0x01))   
				{
					sprintf((char*)&TxBuf1,"与NOx分析仪进行数据链接\n");    
					HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
					Tx_MQW5102();
				}
				if((RxBuf1[1]==0x01)&&(RxBuf1[2]==0x02))   
				{
					sprintf((char*)&TxBuf1,"与五气分析仪进行数据链接\n");    
					HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
					Tx_50B();
				}
				if((RxBuf1[1]==0x01)&&(RxBuf1[2]==0x03))
				{
					sprintf((char*)&TxBuf1,"与发动机测速进行数据链接\n");    
					HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
					//husart5.Init.BaudRate = 19200;
					//sprintf((char*)&TxBuf1,"修改通讯波特率为：19200\n");    
					//HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
					Tx_VMT();
				}
				RxBuf1[0]=0;
				usart_rx_flag=0;
				UART_count1=0; 
			}
			else // 没有检测到分号，数据保存到缓冲区
				{
					if((aRxBuffer[0]==0x4A)&&(usart_rx_flag==0)) //判断到数据开始
					{
						RxBuf1[0]=0x4A;
						UART_count1=1;
						usart_rx_flag=0x10;
					}
					else if((RxBuf1[0]==0x4A)&&(usart_rx_flag!=0)) // 继续接收数据
					{
						uint8_t data;            // 定义这个变量参与赋值主要是解决IAR编译器warning问题。
						data = aRxBuffer[0];
						RxBuf1[UART_count1] = data;     // 保存接收到的数据
						UART_count1++;
					}
				}
			HAL_UART_Receive_IT(&husart1,&aRxBuffer[0],1);    
  }
	if(UartHandle->Instance==USART2)
	{
		if((RxBuf2[0]==0x4A)&&(aRxBuffer[1]==0x3B))    // 接收到0x3B，代表数据结束
		{
			HAL_UART_Transmit(&husart5,RxBuf2,strlen((char *)RxBuf2),1000);
			HMI_Rx_TX();
			while(UART_count2--)
				RxBuf2[UART_count2]=0;
			RxBuf2[0]=0;
			usart_rx_flag=0;
			UART_count2=0; 
		}
		else // 没有检测到分号，数据保存到缓冲区
		{
			if((aRxBuffer[1]==0x4A)&&(usart_rx_flag==0)) //判断到数据开始
			{
				RxBuf2[0]=0x4A;
				UART_count2=1;
				usart_rx_flag=0x10;
			}
			else if((RxBuf2[0]==0x4A)&&(usart_rx_flag!=0)) // 继续接收数据
			{
				uint8_t data;            // 定义这个变量参与赋值主要是解决IAR编译器warning问题。
				data = aRxBuffer[1];
				RxBuf2[UART_count2] = data;     // 保存接收到的数据
				UART_count2++;
			}
		}
		HAL_UART_Receive_IT(&husart2,&aRxBuffer[1],1);    
  }
}
//串口5中断，后面换成周立功
void USART5x_IRQHANDLER(void)
{
  if(__HAL_USART_GET_FLAG(&husart5,USART_FLAG_RXNE)!= RESET) // 接收中断：接收到数据
	{	
		uint8_t data;
		data=READ_REG(husart5.Instance->DR); // 读取数据
		if(UART_count3==0) // 如果是重新接收到数据帧，开启串口空闲中断
		{
			__HAL_UART_CLEAR_FLAG(&husart5,USART_FLAG_IDLE); // 清除空闲中断标志
		  __HAL_UART_ENABLE_IT(&husart5,UART_IT_IDLE);     // 使能空闲中断	    
		}
		if(UART_count3<255)    // 判断接收缓冲区未满
		{
			RxBuf3[UART_count3]=data;  // 保存数据
			UART_count3++;                // 增加接收字节数计数
		}
	}
	else	if(__HAL_USART_GET_FLAG(&husart5,USART_FLAG_IDLE)!= RESET) // 串口空闲中断
	{
		__HAL_UART_CLEAR_FLAG(&husart5,USART_FLAG_IDLE); // 清除空闲中断标志
		__HAL_UART_DISABLE_IT(&husart5,UART_IT_IDLE);    // 关闭空闲中断
		Rx_MSG=1;		                                 // 数据帧置位，标识接收到一个完整数据帧
		
	}
}
//串口5 后面换成周立功
void Rs232_Read(void)
{
	if(Rx_MSG)
	{
		printf("RS232接收到完整数据,数据为：\n");
		HAL_UART_Transmit(&husart1,RxBuf3,strlen((char *)RxBuf3),1000);
		Rx_MSG=0;
		UART_count3=0;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
