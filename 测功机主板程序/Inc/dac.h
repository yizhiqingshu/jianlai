#ifndef __DAC_H__
#define	__DAC_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* ����DACͨ�����ţ�����ʹ��PA4 */
#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA

#define DACx_CHANNEL2_PIN                GPIO_PIN_5
#define DACx_CHANNEL2_GPIO_PORT          GPIOA

/* ����DACͨ��������ʹ��ͨ��1 */
#define DACx_CHANNEL1                    DAC_CHANNEL_1
#define DACx_CHANNEL2                    DAC_CHANNEL_2

/* ��չ���� ------------------------------------------------------------------*/
extern DAC_HandleTypeDef hdac;
/* �������� ------------------------------------------------------------------*/
void MX_DAC_Init(void);

#endif /* __DAC_H */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
