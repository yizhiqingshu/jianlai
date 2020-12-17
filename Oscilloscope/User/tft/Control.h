#ifndef CONTROL_H_
#define CONTROL_H_
#include "stm32f10x.h"
#include "lcd.h"
typedef struct
{
  u16 buffer[128];//数据存放区
	u8 	Flag;		//是否空闲
	u32  timer;		//一格代表几us
  float adc_V;  //每个AD原始值代表多大的电压(单位为mv)
	float x;		//坐标轴参数x
	float y;		//坐标轴参数y
}Display;
typedef struct 
{
u16 Grid_X;
u16 Grid_Y;
}Grid;
void Frame(void);
void xianshi_MEMU(void);
void xianshi_boxing(void);

#endif

