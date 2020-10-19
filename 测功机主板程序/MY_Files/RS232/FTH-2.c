#include "FTH-2.h"
#include "ZLG.h"

/***********
���ƣ�FTH-2 ����վ
ͨѶ������Ϊ19200��1λ��ʼλ��8λ����λ��1λֹͣλ����У��λ��
���ȣ��¶� 0.1  ʪ�� 0.1 ��ѹ 0.1
���ݸ���ʱ�䣺0.5s
***********/

uint8_t FT_Rx_Buf[256];            // ���ջ���,���256�ֽ�
uint8_t FT_Tx_Buf[256];            // ���ͻ���,���256�ֽ�
uint8_t FT_Flag;	//0��ʾ������ȷ��1��ʾ���ݴ���,2Ϊ�����������Ҫ������

int16_t FT_wendu;	//FT_Rx_Buf[10]  FT_Rx_Buf[11]
uint16_t FT_shidu;	//FT_Rx_Buf[12] FT_Rx_Buf[13]
uint16_t FT_daqiya;	//FT_Rx_Buf[14]	FT_Rx_Buf[15]

extern uint8_t cmd_buf[2];

/***********
����ָ��	��״̬����������
4EH 54H 33H 33H 53H	��ʼ�ֽ�
03H									����
01H 01H							����
FAH									У���
***********/
void Tx_FTH(void)	
{
	uint8_t i;
	/*��״̬Э����*/
	FT_Tx_Buf[0]=0x4E;
	FT_Tx_Buf[1]=0x54;
	FT_Tx_Buf[2]=0x33;
	FT_Tx_Buf[3]=0x33;
	FT_Tx_Buf[4]=0x53;
	
	FT_Tx_Buf[5]=0x03;	//�����ֽ�
	
	FT_Tx_Buf[6]=0x01;	//������ֽ�
	FT_Tx_Buf[7]=0x01;	//������ֽ�
	
	FT_Tx_Buf[8]=0xFA;	//У����ֽ�
	/**************
	����״ָ̬��
	**************/
	for(i=0;i<9;i++)
	{
		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, FT_Tx_Buf[i],cmd_buf);			//��Ҫ�޸ģ����������õĶ���һ��ͨ��
	}
}
/**********
��������
Ӧ��ָ�
4EH 54H 33H 33H 41H		��ʼ�ֽ�
0DH										�����ֽ�
01H 01H								�����ֽ�
ST01									״̬�� 8bit unsigned
ST02									״̬�� U8
CT01 CT02							�¶ȣ�x10�� 16bit signed
CT03 CT04							ʪ�ȣ�x10�� 16bit unsigned
CT05 CT06							����ѹ��x10��16bit unsigned
CT07 CT08							������TBD1��
CS										У���
**********/
void Rx_FTH(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
	{
		FT_Rx_Buf[i]=zlg9518s_read_reg_pack(ZLG9518S_RHR_REG, UART_CHAN_0, 1);
	}
	if(FT_Rx_Buf[4]==0x41)    
	{
		FT_Flag=0;	//������ȷ
		FT_wendu=(((int16_t)FT_Rx_Buf[10]<<8)|(int16_t)FT_Rx_Buf[11])*10;
		FT_shidu=(((uint16_t)FT_Rx_Buf[12]<<8)|(uint16_t)FT_Rx_Buf[13])*10;
		FT_daqiya=(((uint16_t)FT_Rx_Buf[14]<<8)|(uint16_t)FT_Rx_Buf[15])*10;
	}
	else if(FT_Rx_Buf[4]==0x45)
		FT_Flag=1;	//���ݴ���
	else
		FT_Flag=2;
}


