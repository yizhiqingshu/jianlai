#include "stm32f10x.h"
#include "bsp_usart.h"
#include "PWM_IC.h"
#include "ADC.h"
#include "DAC.h"
#include "adc_Double.h"
#include "Control.h"
#include "lcd.h"
#include "arm_math.h"
#include "stm32_dsp.h"
#define ulong unsigned long
#define N 100000/1024

extern long MagBufArray[NPT/2];          //幅值
extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;
ulong set_max(ulong a[],uint16_t low,uint16_t high);
uint32_t MAX=0;
uint16_t F_fft;
uint16_t Vpp;
int main(void)
{	
    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  USART_Config();
//  ADVANCE_TIMX_Init();
    LCD_Init();
	POINT_COLOR=BLUE;
	Frame();
    TIM2_Init(79,8,40);
    ADC_DOUBLE_Init();
	TIM4_Init();
//	arm_status Status;
//	arm_rfft_fast_instance_f32 P_S;
//	Status = arm_rfft_fast_init_f32(&P_S,FFT_LENGTH);
//	if(Status!=ARM_MATH_SUCCESS)
//	{
//	
//	}
//	arm_rfft_fast_f32(&P_S,fft_src,fft_dst,FFT_type_fft);
//	for(i_fft=0;i_fft<FFT_LENGTH;i_fft+=2)
//	{
//	fft_fre[i_fft]=sqrt(fft_dst[i_fft] * fft_dst[i_fft] + fft_dst[i_fft+1] * fft_dst[i_fft+1]);
//	}
  while(1)
	{	
        
        MAX=set_max(MagBufArray,0,511);
        F_fft=N*MAX;
        Vpp=MAX/512;
	}	
}

ulong set_max(ulong *a,uint16_t low,uint16_t high)//分治法求数组最大值
{
    if(low==high)
        return a[low];
    else
    {
        uint32_t mid=(low+high)/2;
        uint32_t left_max=set_max(a,low,mid);
        uint32_t right_max=set_max(a,mid+1,high);
        if(left_max>right_max)
            return left_max;
        else return right_max;
    }
}
/*********************************************END OF FILE**********************/
