#ifndef CONTROL_H_
#define CONTROL_H_
#include "stm32f10x.h"
typedef struct
{
  u16 buffer[128];//���ݴ����
	u8 	Flag;		//�Ƿ����
	u32  timer;		//һ�����us
  float adc_V;  //ÿ��ADԭʼֵ������ĵ�ѹ(��λΪmv)
	float x;		//���������x
	float y;		//���������y
}xianshi;
void xianshi_MEMU(void);
void xianshi_boxing(void);

#endif