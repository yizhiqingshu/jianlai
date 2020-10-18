#ifndef _JIANLAI_H
#define _JIANLAI_H
#include "stm32f10x.h"

#include "exti_key.h"
#include "UART_1.h"
#include "TIM_x.h"
#include "adc_Double.h"
#include "UART_1.h"
#include <stdio.h>
#include "lcd.h"
#include "TIM_x.h"
#include "DAC.h"
#include "PID_UP.h"
#include "PWM.h"
#include "NRF24l01.h"
#include "string.h"
#include "lcd.h"
#include "bsp_usart_dma.h"
#include "oled.h"
#define UART_INIT 1
#define UART2_OPENMV 1
#define LCD_INIT 0
#define OLED_spi 1

#define DAC_INIT 0
#define ADVANCE_TIM1 0 //互补输出带死区的PWM
#define TIM_5_IC 0
#define TIM1_CH1_CH2_IC 0
#define TIM3_PWM_CH1_4 0
#define TIM4_PWM_CH1_4 0
#define UART2_PRINTF 1
#define UART1_PRINTF 0
#define NRF2401_RX 0
#define NRF2401_TX 0
#endif

