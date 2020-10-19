#include "MY_Talk.h"
#include "string.h"
#include <stdio.h>
#include "math.h"
#include "tcp_echoclient.h"
#include "usart.h"
#include "stm_flash.h"
#include "VMT_2000.h"
#include "50B.h"
#include "contorl.h"

extern uint8_t TxBuf2[256];
extern uint8_t RxBuf2[256];

uint8_t id;
uint8_t flag;
uint8_t flag1;
uint8_t dot_position;
uint8_t qiyou_car=0;
uint8_t chaiyou_car=0;
extern uint8_t tcp_flag;	//�������ͳ����߲��ͳ�����  0x01���ͳ�  0x02 ���ͳ�  Ĭ�ϲ���������


extern uint16_t VMT_rpm;
extern uint8_t cylinder;
extern uint8_t n;
extern int16_t FT_wendu;	
extern uint16_t FT_shidu;	
extern uint16_t FT_daqiya;

extern uint8_t TxBuf1[256];
extern uint8_t RxBuf2[256];
extern uint8_t time_count3;
void show_T1_Kp(void);
void show_T2_Kp(void);
void show_T3_Kp(void);
void show_S_Kp(void);
void show_T1_Ki(void);
void show_T2_Ki(void);
void show_T3_Ki(void);
void show_S_Ki(void);
void show_T1_Kd(void);
void show_T2_Kd(void);
void show_T3_Kd(void);
void show_S_Kd(void);
void show_PID(void);

void write_end(void)
{
	uint8_t TxBuf2[3];
	TxBuf2[0]=0xff;
	TxBuf2[1]=0xff;
	TxBuf2[2]=0xff;
	HAL_UART_Transmit(&husart2,TxBuf2,3,100);
}

void HMI_Rx_TX(void)
{
	if(RxBuf2[1]==0x66)  //˵���������л�ҳ��
	{
		id=RxBuf2[2];
		qiyou_car=0;
		chaiyou_car=0;
	}
	if(id==0x01)	//PID��������ҳ��
	{
		sprintf((char*)&TxBuf1,"�л�������ҳ��\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		//stm_flash_Rx();
		show_PID();
		if(RxBuf2[1]==0x06)	//���в����޸�
		{
			flag=RxBuf2[2];	//�޸ĵ�Ŀ��ֵ��flag
			flag1=RxBuf2[3];	//flag��flag1��ͬ��������Ĳ���
		}
		else if(RxBuf2[1]==0x5A)	//����ȷ��OK������4A    5A 
		{
			sprintf((char*)&TxBuf1,"�޸�PID����\n");    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			Recive_PID();
		}
		else if(RxBuf2[1]==0x3A)
		{
			stm_flash_Tx();	//�Ѹı���PID�洢��flash��
		}
	}
	else if(id==0x02)	//���ͳ����ҳ��
	{
		sprintf((char*)&TxBuf1,"�л������ͳ����ҳ��\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		if(RxBuf2[1]==0x08)	//�Ƿ��¿�ʼ���İ�ť
		{
			sprintf((char*)&TxBuf1,"�������ͳ���ʼ���\n");    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			tcp_flag=RxBuf2[2]; 
			qiyou_car=1;
			MQ5B_tx();	//��Ļ��ʾ���ͳ��������
		}
		else if(RxBuf2[1]==0x09)		//�������
		{
			sprintf((char*)&TxBuf1,"�������ͳ��������\n");    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			qiyou_car=0;
		}
	}
	else if(id==0x03)	//���ͳ����ҳ��
	{
		sprintf((char*)&TxBuf1,"�л������ͳ����ҳ��\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		if(RxBuf2[1]==0x08)	//�Ƿ��¿�ʼ���İ�ť
		{
			sprintf((char*)&TxBuf1,"���²��ͳ���ʼ���\n");    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			tcp_flag=RxBuf2[2];
			chaiyou_car=1;
			VMT_tx();		//��Ļ��ʾ���ͳ�����
		}
		else if(RxBuf2[1]==0x5A)		//�ı���ͳ��ı��������Ĭ��Ϊ4
		{
			cylinder=RxBuf2[2]-0x30;
			sprintf((char*)&TxBuf2,"n3.val=%d",cylinder);
			HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
			write_end();
			sprintf((char*)&TxBuf1,"�ı䱻�����Ϊ��%d\n",cylinder);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(RxBuf2[1]==0x07)		//�ı���ͳ��ĵ��ʽ��Ĭ��Ϊ����� 1
		{
			n=RxBuf2[2];
			sprintf((char*)&TxBuf1,"�ı���ʽΪ��%d\n",n);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(RxBuf2[1]==0x09)		//�������
		{
			sprintf((char*)&TxBuf1,"���²��ͳ��������\n");    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			chaiyou_car=0;
		}
	}
	else //����ҳ���յ� 01 0m 0n
	{
		sprintf((char*)&TxBuf1,"�л�����ҳ\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		if(RxBuf2[1]==0x01)
			open_close();
	}
}

void Recive_PID(void)
{
	dot_position=double_check();
	if(flag==0x01)
	{
		if(flag1==0x01)	//������1
			{
				if(dot_position!=0)
					close_T1.Kp=gain_doublePID();
				else
					close_T1.Kp=gain_PID();
				show_T1_Kp();
				sprintf((char*)&TxBuf1,"�޸�close_T1.KpΪ��%.3f\n",close_T1.Kp);    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			}
			else if(flag1==0x05)	//������2
			{
				if(dot_position!=0)
					close_T2.Kp=gain_doublePID();
				else
					close_T2.Kp=gain_PID();
				show_T2_Kp();
				sprintf((char*)&TxBuf1,"�޸�close_T2.KpΪ��%.3f\n",close_T2.Kp);    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			}
			else if(flag1==0x02)	//������3
			{
				if(dot_position!=0)
					close_T3.Kp=gain_doublePID();
				else
					close_T3.Kp=gain_PID();
				show_T3_Kp();
				
				sprintf((char*)&TxBuf1,"�޸�close_T3.KpΪ��%.3f\n",close_T3.Kp);    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			}
			else if(flag1==0x03)	//ת��
			{
				if(dot_position!=0)
					close_S.Kp=gain_doublePID();
				else
					close_S.Kp=gain_PID();
				show_S_Kp();
				
				sprintf((char*)&TxBuf1,"�޸�close_S.KpΪ��%.3f\n",close_S.Kp);    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
			}
		}
	else if(flag==0x02)
	{
		if(flag1==0x01)	//������1
		{
			if(dot_position!=0)
				close_T1.Ki=gain_doublePID();
			else
				close_T1.Ki=gain_PID();
			show_T1_Ki();
			sprintf((char*)&TxBuf1,"�޸�close_T1.KiΪ��%.3f\n",close_T1.Ki);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(flag1==0x05)	//������2
		{
			if(dot_position!=0)
				close_T2.Ki=gain_doublePID();
			else
				close_T2.Ki=gain_PID();
			show_T2_Ki();
			
			sprintf((char*)&TxBuf1,"�޸�close_T2.KiΪ��%.3f\n",close_T2.Ki);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(flag1==0x02)	//������3
		{
			if(dot_position!=0)
				close_T3.Ki=gain_doublePID();
			else
				close_T3.Ki=gain_PID();
			show_T3_Ki();
			
			sprintf((char*)&TxBuf1,"�޸�close_T3.KiΪ��%.3f\n",close_T3.Ki);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(flag1==0x03)	//ת��
		{
			if(dot_position!=0)
				close_S.Ki=gain_doublePID();
			else
				close_S.Ki=gain_PID();
			show_S_Ki();
			
			
			sprintf((char*)&TxBuf1,"�޸�close_S.KiΪ��%.3f\n",close_S.Ki);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
	}
	else if(flag==0x03)
	{
		if(flag1==0x01)	//������1
		{
			if(dot_position!=0)
				close_T1.Kd=gain_doublePID();
			else
				close_T1.Kd=gain_PID();
			show_T1_Kd();
			
			
			sprintf((char*)&TxBuf1,"�޸�close_T1.KdΪ��%.3f\n",close_T1.Kd);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(flag1==0x05)	//������2
		{
			if(dot_position!=0)
				close_T2.Kd=gain_doublePID();
			else
				close_T2.Kd=gain_PID();
			show_T2_Kd();
			
			
			sprintf((char*)&TxBuf1,"�޸�close_T2.KdΪ��%.3f\n",close_T2.Kd);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		if(flag1==0x02)	//������3
		{
			if(dot_position!=0)
				close_T3.Kd=gain_doublePID();
			else
				close_T3.Kd=gain_PID();
			show_T3_Kd();
			
			
			sprintf((char*)&TxBuf1,"�޸�close_T3.KdΪ��%.3f\n",close_T3.Kd);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
		else if(flag1==0x03)	//ת��
		{
			if(dot_position!=0)
				close_S.Kd=gain_doublePID();
			else
				close_S.Kd=gain_PID();
			show_S_Kd();
			
			
			sprintf((char*)&TxBuf1,"�޸�close_S.KdΪ��%.3f\n",close_S.Kd);    
			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		}
	}
}

void MQ5B_tx(void)
{	
	/*
	����CO2  %
	*/
	/*MQ50B.CO2=10;
	MQ50B.CO=20;
	MQ50B.HC=30;
	MQ50B.NO=40;
	MQ50B.O2=50;*/
	
	sprintf((char*)&TxBuf2,"t11.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.2f",MQ50B.CO2);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
	/*
	����CO  %
	*/
	sprintf((char*)&TxBuf2,"t12.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.2f",MQ50B.CO);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
	/*
	����HC  *10^-6
	*/
	sprintf((char*)&TxBuf2,"n2.val=%d",MQ50B.HC);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();
	/*
	����NO  *10^-6
	*/
	sprintf((char*)&TxBuf2,"n3.val=%d",MQ50B.NO);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();	
	/*
	����O2  %
	*/
	sprintf((char*)&TxBuf2,"t13.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.2f",MQ50B.O2);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
	
//	sprintf((char*)&TxBuf1,"���ͳ���ʾ�������\n");    
//	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
}


void VMT_tx(void)
{
	VMT_rpm=1000;
	sprintf((char*)&TxBuf2,"n0.val=%d",VMT_rpm);	//������ת��
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();

	/*sprintf((char*)&TxBuf2,"n1.val=%d",MQ50B_CO);	//��Ҫ�޸�
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();

	sprintf((char*)&TxBuf2,"n2.val=%d",MQ50B_HC);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();*/
//	sprintf((char*)&TxBuf1,"���ͳ���ʾ�������\n");    
//	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
}

//����վ���ݴ���
void FHT_tx(void)
{
	sprintf((char*)&TxBuf2,"n0.val=%d",FT_wendu);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();

	sprintf((char*)&TxBuf2,"n1.val=%d",FT_shidu);	//��Ҫ�޸�
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();

	sprintf((char*)&TxBuf2,"n2.val=%d",FT_daqiya);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	write_end();
}

void open_close(void)
{
	switch(RxBuf2[2])
	{
		case 0x00: 
		{
			if(RxBuf2[3]==0x01)
			{
				sprintf((char*)&TxBuf1,"open_sw0\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw0;
			}
			else if(RxBuf2[3]==0x00)
			{
				sprintf((char*)&TxBuf1,"close_sw0\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw0;
			}
		}break;
		case 0x01:
		{
			if(RxBuf2[3])
			{
				sprintf((char*)&TxBuf1,"open_sw1\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw1;
			}
			else if(!RxBuf2[3])
			{	
				sprintf((char*)&TxBuf1,"close_sw1\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw1;
			}
		}break;
		case 0x02:
		{
			if(RxBuf2[3]==0x01)
			{
				sprintf((char*)&TxBuf1,"open_sw2\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw2;
			}
			else if(RxBuf2[3]==0x00)
			{
				sprintf((char*)&TxBuf1,"close_sw2\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw2;
			}
		}break;
		case 0x03:
		{
			if(RxBuf2[3]==0x01)
			{
				sprintf((char*)&TxBuf1,"open_sw3\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw3;
			}
			else if(RxBuf2[3]==0x00)
			{
				sprintf((char*)&TxBuf1,"close_sw3\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw3;
			}
		}break;
		case 0x04:
		{
			if(RxBuf2[3]==0x01)
			{
				sprintf((char*)&TxBuf1,"open_sw4\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw4;
			}
			else if(RxBuf2[3]==0x00)
			{
				sprintf((char*)&TxBuf1,"close_sw4\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw4;
			}
		}break;
		case 0x05:
		{
			if(RxBuf2[3]==0x01)
			{
				sprintf((char*)&TxBuf1,"open_sw5\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				open_sw5;
			}
			else if(RxBuf2[3]==0x00)
			{
				sprintf((char*)&TxBuf1,"close_sw5\n");    
				HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
				close_sw5;
			}
		}break;
		default:warning;break;
	}
}

uint8_t Ascall_zhuanhua(uint8_t ascall)
{
	uint8_t math;
	math=ascall-0x30;
	return math;
}

uint16_t gain_PID(void)
{
	uint8_t i,j=0;
	uint16_t PID=0;
	uint8_t math[5]={0,0,0,0,0};
	for(i=2;;i++)
	{
		if(RxBuf2[i]<0x3A&&RxBuf2[i]>0x2F)
			math[i-2]=Ascall_zhuanhua(RxBuf2[i]);
		else
			break;
	}
	while((--i)>1)
	{
		PID+=math[j]*pow(0x0A,(i-2));
		j++;
	}
	return PID;
}

double gain_doublePID(void)
{
	uint8_t i,j=0;
	double PID=0;
	uint8_t PID_int=0;
	double double_PID=0;
	uint8_t math[5]={0,0,0,0,0};
	PID_int=gain_PID();
	for(i=dot_position;;i++)
	{
		if(RxBuf2[i+1]<0x3A&&RxBuf2[i+1]>0x2F)
			math[i-dot_position]=Ascall_zhuanhua(RxBuf2[i+1]);
		else
			break;
	}
	while(i-dot_position)
	{
		double_PID+=math[j]*pow(0x0A,(i-dot_position-1));
		j++;
		--i;
	}
	double_PID=double_PID/(pow(0x0A,j));
	PID=PID_int+double_PID;
	return PID;
}

//����0������û��С���� �����ط�0��������С����
uint8_t double_check(void)
{
	uint8_t a=0;
	uint8_t i;
	for(i=2;i<8;i++)
	{
		if(RxBuf2[i]==0x2E)
			a=i;
	}
	return a;
}

uint16_t time_count2=0;

void Tcp_send_car(void)
{
	if(time_count2>=50)	//1s		һ���ӽ������ε��������ݶ�ȡ�Լ����ڵļ�ʱ����
		{
			if(chaiyou_car==1)	//���ͳ����ҳ��
			{
				printf("���ڷ�������");
				VMT_tx();
				Tx_50B();
				Rx_50B();
				YSH7_TCP_SENDData(tcp_flag);
			}
			else if(qiyou_car==1)	//���ͳ����ҳ��
			{
				printf("���ڷ�������");
				MQ5B_tx();
				Tx_VMT();	//֧�ֵ�ģʽ ������ģʽ��1s�������ݻش�1��
				Rx_VTM();
				YSH7_TCP_SENDData(tcp_flag);
			}
			time_count2=0;
		}
}

void HMI_show(void)
{
	if(time_count3>=50)	//1s����
	{
		if(qiyou_car==1)
		{
			MQ5B_tx();	
		}
		else if(chaiyou_car==1)
		{
			VMT_tx();		//��Ļʵʱ��ʾ��������
		}
		time_count3=0;
	}
}

void show_T1_Kp(void)
{
	sprintf((char*)&TxBuf2,"t4.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T1.Kp);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T2_Kp(void)
{
	sprintf((char*)&TxBuf2,"t11.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T2.Kp);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T3_Kp(void)
{
	sprintf((char*)&TxBuf2,"t18.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T3.Kp);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_S_Kp(void)
{
	sprintf((char*)&TxBuf2,"t25.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_S.Kp);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T1_Ki(void)
{
	sprintf((char*)&TxBuf2,"t5.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T1.Ki);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T2_Ki(void)
{
	sprintf((char*)&TxBuf2,"t12.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T2.Ki);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T3_Ki(void)
{
	sprintf((char*)&TxBuf2,"t19.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T3.Ki);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_S_Ki(void)
{
	sprintf((char*)&TxBuf2,"t26.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_S.Ki);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T1_Kd(void)
{
	sprintf((char*)&TxBuf2,"t6.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T1.Kd);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T2_Kd(void)
{
	sprintf((char*)&TxBuf2,"t13.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T2.Kd);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_T3_Kd(void)
{
	sprintf((char*)&TxBuf2,"t20.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_T3.Kd);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_S_Kd(void)
{
	sprintf((char*)&TxBuf2,"t27.txt=");
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	sprintf((char*)&TxBuf2,"%.3f",close_S.Kd);
	HAL_UART_Transmit(&husart2,TxBuf2,strlen((char *)TxBuf2),1000);
	TxBuf2[0]='"';
	HAL_UART_Transmit(&husart2,TxBuf2,1,1000);
	write_end();
}
void show_PID(void)
{
	show_T1_Kp();
	show_T2_Kp();
	show_T3_Kp();
	show_S_Kp();
	show_T1_Ki();
	show_T2_Ki();
	show_T3_Ki();
	show_S_Ki();
	show_T1_Kd();
	show_T2_Kd();
	show_T3_Kd();
	show_S_Kd();
}
