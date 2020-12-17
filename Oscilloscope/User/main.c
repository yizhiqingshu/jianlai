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


extern long MagBufArray[NPT/2];          //幅值
extern __IO uint16_t IC2Value ;
extern __IO uint16_t IC1Value;
extern __IO float DutyCycle;
extern __IO float Frequency;
ulong index_1=0;
long set_max(long a[],uint16_t low,uint16_t high);
long set_max_idx(long a[],uint16_t low,uint16_t high);
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
    
    ADC_DOUBLE_Init();
    TIM2_Init(79,8,40);
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
        index_1 = set_max_idx(MagBufArray,4,511);
        F_fft=(Fs/NPT)*index_1;
        //Vpp=MAX/512;
        //printf("F_fft->%d Vpp->%d\n",F_fft,Vpp);
	}	
}

long set_max(long *a,uint16_t low,uint16_t high)//分治法求数组最大值
{
    if(low==high)
        return a[low];
    else
    {
        long mid=(low+high)/2;
        long left_max=set_max(a,low,mid);
        long right_max=set_max(a,mid+1,high);
        if(left_max>right_max)
            return left_max;
        else return right_max;
    }
}

long set_max_idx(long a[],uint16_t low,uint16_t high)//分治法求数组最大值
{
    long P=low;
    for(uint16_t i=low;i<high;i++)
    {
    if(a[i]>a[P])
        P=i;
    else if(a[P]==a[i+1])
        P=i+1;
    }
    return P;
}



/*********************************************END OF FILE**********************/
