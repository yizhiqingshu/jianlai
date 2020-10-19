/**
  ******************************************************************************
  * 文件名程: bsp_EncoderTIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2017-6-06
  * 功    能: 编码器
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "TS5303N616_1.h"
#include "tim.h"
//#include "TS5303N616_2.h"
//#include "TS5303N616_3.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
int16_t OverflowCount1 = 0;//定时器溢出次数
//extern int16_t OverflowCount2;
//extern int16_t OverflowCount3;
//extern TIM_HandleTypeDef htim4_Encoder;
//extern TIM_HandleTypeDef htim1_Encoder;
/* Timer handler declaration */
TIM_HandleTypeDef    htim3_Encoder;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ENCODER1_TIMx_Init(void)
{    
  htim3_Encoder.Instance = ENCODER_TIMx;
  htim3_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htim3_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htim3_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htim3_Encoder);
  
  sEncoderConfig.EncoderMode = TIM_ENCODERMODE_TIx;  
  
	sEncoderConfig.IC1Polarity = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC1Prescaler = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC1Filter = 0;
  
  sEncoderConfig.IC2Polarity = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC2Prescaler = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC2Filter = 0;
  __HAL_TIM_SET_COUNTER(&htim3_Encoder,0);
  
  HAL_TIM_Encoder_Init(&htim3_Encoder, &sEncoderConfig);
  
  //
  __HAL_TIM_CLEAR_IT(&htim3_Encoder, TIM_IT_UPDATE);  //清除更新中断标志位
  __HAL_TIM_URS_ENABLE(&htim3_Encoder);               //仅允许计数器溢出才产生更新中断
  __HAL_TIM_ENABLE_IT(&htim3_Encoder,TIM_IT_UPDATE);  //使能更新中断
  
  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    ENCODER_TIM_RCC_CLK_ENABLE();
    ENCODER_TIM_GPIO_CLK_ENABLE();     

    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    ENCODER_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
} 

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim3_Encoder)
	{
		if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3_Encoder))
    OverflowCount1--;       //向下计数溢出
  else
    OverflowCount1++;       //向上计数溢出
	}
  if(htim == &htimx)
	{
		time_count++;
	}

}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
