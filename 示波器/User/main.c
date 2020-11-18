#include "stm32f10x.h"
#include "bsp_usart.h"
#include "PWM_IC.h"
#include "ADC.h"
#include "DAC.h"
#include "adc_Double.h"
extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;

int main(void)
{	
  USART_Config();
//  ADVANCE_TIMX_Init();
  ADC_DOUBLE_Init();
	
  while(1)
	{	
		
	}	
}
/*********************************************END OF FILE**********************/
