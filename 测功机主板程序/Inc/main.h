/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define USE_LCD        /* enable LCD  */  
#define USE_DHCP       /* enable DHCP, if disabled static address is used */

/* USER CODE BEGIN Includes */
#define DEST_IP_ADDR0   (uint8_t) 192
#define DEST_IP_ADDR1   (uint8_t) 168
#define DEST_IP_ADDR2   (uint8_t) 3
#define DEST_IP_ADDR3   (uint8_t) 176

#define DEST_PORT       (uint16_t) 1234
 

/* USER CODE BEGIN Includes */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 3
#define IP_ADDR3   (uint8_t) 174
   
/*NETMASK*/
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/*Gateway Address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 3
#define GW_ADDR3   (uint8_t) 1

//#define TCP_SERVER_RX_BUFSIZE	200		//定义tcp server最大接收数据长度
//#define TCP_SERVER_PORT			1234	//定义tcp server的端口
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define D00_Pin GPIO_PIN_6
#define D00_GPIO_Port GPIOC
#define D01_Pin GPIO_PIN_7
#define D01_GPIO_Port GPIOC
#define D02_Pin GPIO_PIN_8
#define D02_GPIO_Port GPIOC
#define D03_Pin GPIO_PIN_9
#define D03_GPIO_Port GPIOC
#define D04_Pin GPIO_PIN_7
#define D04_GPIO_Port GPIOG
#define D05_Pin GPIO_PIN_8
#define D05_GPIO_Port GPIOG

extern uint8_t TxBuf1[256];
extern uint8_t TxBuf3[256];
extern uint8_t aRxBuffer[3];
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
