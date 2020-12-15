#ifndef CONTROL_H_
#define CONTROL_H_
#include "stm32f10x.h"
#include "lcd.h"
typedef struct
{
  u16 buffer[128];//���ݴ����
	u8 	Flag;		//�Ƿ����
	u32  timer;		//һ�����us
  float adc_V;  //ÿ��ADԭʼֵ������ĵ�ѹ(��λΪmv)
	float x;		//���������x
	float y;		//���������y
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

