#include "stm32f10x.h"
#include "bsp_usart.h"
#include "PWM_IC.h"
#include "ADC.h"
#include "DAC.h"
#include "adc_Double.h"
#include "Control.h"
#include "lcd.h"
#include "arm_math.h"

#define FFT_LENGTH 256
#define FFT_type_fft 0
#define FFT_type_ifft 1
float fft_src[FFT_LENGTH];
float fft_dst[FFT_LENGTH];
float fft_fre[FFT_LENGTH/2];


extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;

int main(void)
{	
	uint32_t i_fft;
  USART_Config();
//  ADVANCE_TIMX_Init();
  ADC_DOUBLE_Init();
	LCD_Init();
	POINT_COLOR=BLUE;
	Frame();
	
	arm_status Status;
	arm_rfft_fast_instance_f32 P_S;
	Status = arm_rfft_fast_init_f32(&P_S,FFT_LENGTH);
	if(Status!=ARM_MATH_SUCCESS)
	{
	
	}
	arm_rfft_fast_f32(&P_S,fft_src,fft_dst,FFT_type_fft);
	for(i_fft=0;i_fft<FFT_LENGTH;i_fft+=2)
	{
	fft_fre[i_fft]=sqrt(fft_dst[i_fft] * fft_dst[i_fft] + fft_dst[i_fft+1] * fft_dst[i_fft+1]);
	}
  while(1)
	{	
	}	
}

/*********************************************END OF FILE**********************/
