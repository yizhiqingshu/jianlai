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
#include "TS5303N616_2.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
int16_t OverflowCount2 = 0;//定时器溢出次数
/* Timer handler declaration */
TIM_HandleTypeDef    htim4_Encoder;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoder2Config;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ENCODER2_TIMx_Init(void)
{    
  htim4_Encoder.Instance = ENCODER2_TIMx;
  htim4_Encoder.Init.Prescaler = ENCODER2_TIM_PRESCALER;
  htim4_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4_Encoder.Init.Period = ENCODER2_TIM_PERIOD;
  htim4_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htim4_Encoder);
  
  sEncoder2Config.EncoderMode = TIM4_ENCODERMODE_TIx;  
  
	sEncoder2Config.IC1Polarity = TIM_ICPOLARITY_RISING;   
  sEncoder2Config.IC1Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoder2Config.IC1Prescaler = TIM_ICPSC_DIV1; 
  sEncoder2Config.IC1Filter = 0;
  
  sEncoder2Config.IC2Polarity = TIM_ICPOLARITY_RISING;   
  sEncoder2Config.IC2Selection = TIM_ICSELECTION_DIRECTTI;  
  sEncoder2Config.IC2Prescaler = TIM_ICPSC_DIV1; 
  sEncoder2Config.IC2Filter = 0;
  __HAL_TIM_SET_COUNTER(&htim4_Encoder,0);
  
  HAL_TIM_Encoder_Init(&htim4_Encoder, &sEncoder2Config);
  
  //
  __HAL_TIM_CLEAR_IT(&htim4_Encoder, TIM_IT_UPDATE);  //清除更新中断标志位
  __HAL_TIM_URS_ENABLE(&htim4_Encoder);               //仅允许计数器溢出才产生更新中断
  __HAL_TIM_ENABLE_IT(&htim4_Encoder,TIM_IT_UPDATE);  //使能更新中断
  
  HAL_NVIC_SetPriority(ENCODER2_TIM_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENCODER2_TIM_IRQn);
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder2_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER2_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    ENCODER2_TIM_RCC_CLK_ENABLE();
    ENCODER2_TIM_GPIO_CLK_ENABLE();     

    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER2_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER2_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER2_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER2_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER2_TIM_GPIO_AFx;
    HAL_GPIO_Init(ENCODER2_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder2_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER2_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    ENCODER2_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER2_TIM_CH1_GPIO, ENCODER2_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER2_TIM_CH2_GPIO, ENCODER2_TIM_CH2_PIN);
  }
} 
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim4_Encoder))
    OverflowCount2--;       //向下计数溢出
  else
    OverflowCount2++;       //向上计数溢出
}*/
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
