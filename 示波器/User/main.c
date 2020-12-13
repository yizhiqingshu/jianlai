#include "stm32f10x.h"
#include "bsp_usart.h"
#include "PWM_IC.h"
#include "ADC.h"
#include "DAC.h"
#include "adc_Double.h"
#include "lcd.h"
extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;
void biankuang();
int main(void)
{	
  USART_Config();
//  ADVANCE_TIMX_Init();
  ADC_DOUBLE_Init();
	LCD_Init();
	POINT_COLOR=RED;
  biankuang();
  while(1)
	{	
		
	}	
}
void biankuang()
{
	LCD_DrawLine(0,0,240,0);
	LCD_DrawLine(0,0,10,10);
	LCD_DrawLine(10,10,230,10);
	
	LCD_DrawLine(240,0,240,320);
	LCD_DrawLine(240,0,230,10);
	LCD_DrawLine(230,10,230,310);
	
	LCD_DrawLine(240,320,0,320);
	LCD_DrawLine(240,320,230,310);	
	LCD_DrawLine(230,310,10,310);
	
	LCD_DrawLine(0,320,0,0);
	LCD_DrawLine(0,320,10,310);
	LCD_DrawLine(10,310,10,10);
}
/*********************************************END OF FILE**********************/
