#ifndef _MY_H
#define _MY_H

#include "main.h"
/*
输出采用低电平驱动方式
*/
#define open_sw0	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)  
#define close_sw0	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)

#define open_sw1	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET)
#define close_sw1	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET)

#define open_sw2	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET)
#define close_sw2	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET)

#define open_sw3	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET)
#define close_sw3	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET)

#define open_sw4	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET)
#define close_sw4	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET)

#define open_sw5	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)
#define close_sw5	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)

#define warning

void write_end(void);
void HMI_Rx_TX(void);
void open_close(void);
void MQ5B_tx(void);
void VMT_tx(void);
void FHT_tx(void);
void Recive_PID(void);
uint8_t Ascall_zhuanhua(uint8_t ascall);
uint16_t gain_PID(void);
double gain_doublePID(void);
uint8_t double_check(void);
void Tcp_send_car(void);
void HMI_show(void);

extern uint16_t time_count2;

#endif

