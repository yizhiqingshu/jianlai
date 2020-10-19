#include "50B.h"
#include "ZLG.h"
#include "usart.h"

/***********
���ƣ����������ǣ��������ͳ��ļ��
������ 9600 ��ʼλ 1 ����λ 8 У��λ �� ֹͣλ 1
***********/

uint8_t MQ50B_Rx_Buf[256]={0};            // ���ջ���,���256�ֽ�
uint8_t MQ50B_Tx_Buf[256];            // ���ͻ���,���256�ֽ�

uint8_t MQ50B_Flag; //0��ʾ������ȷ��1�����������Ҫ������
extern uint8_t cmd_buf[2];
extern uint8_t RS232_flag;
extern uint8_t RS232_Rxbuf[256];

extern uint8_t TxBuf1[256];

MQ50B_test_data MQ50B;
/*��ϵͳ״̬������
	ʵʱ��ȡ����  
	�����ʽ��02H-A3H-03H-58H
*/
void Tx_50B(void)
{
//	uint8_t i;
	/*��״̬Э����*/
	MQ50B_Tx_Buf[0]=0x02;	//��ʼ�ֽ�
	MQ50B_Tx_Buf[1]=0xA3;	//�����ֽ�
	MQ50B_Tx_Buf[2]=0x03;	//���ݳ���
	MQ50B_Tx_Buf[3]=0x58; //У���
	
	/**************
	����״ָ̬��
	**************/
	//memcpy(txbuf,"����һ��RS232��RS485ͬʱʹ�ò���ʵ��\n",100);
  HAL_UART_Transmit(&husart5,MQ50B_Tx_Buf,strlen((char *)MQ50B_Tx_Buf),1000);
	RS232_flag=1;
//	for(i=0;i<4;i++)
//	{
//		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, MQ50B_Tx_Buf[i],cmd_buf);	//��Ҫ�޸ģ����������õĶ���һ��ͨ��
//	}
}

/*����ָ��
��Ӧ��ʽ 06H-A3H-17H-CO2��λ-CO2��λ-CO��λ-CO��λ-HC��λ-HC
					 ��λ-NO��λ-NO��λ-O2��λ-O2��λ-���¸�λ-���µ�λ-ת
					 �ٸ�λ-ת�ٵ�λ-��·ѹ����λ-��·ѹ����λ-��ȼ�ȸ�λ-��
					 ȼ�ȵ�λ-PEFֵ��λ-PEFֵ��λ-У����

MQ50B_CO2	MQ50B_Rx_Buf[3] [4]
MQ50B_CO	MQ50B_Rx_Buf[5] [6]
MQ50B_HC	MQ50B_Rx_Buf[7] [8]
MQ50B_NO	MQ50B_Rx_Buf[9] [10]
MQ50B_O2	MQ50B_Rx_Buf[11] [12]
*/
void Rx_50B(void)
{
	uint8_t i;
	//for(i=0;i<23;i++)
	for(i=0;i<30;i++)
	{
		//MQ50B_Rx_Buf[i]=zlg9518s_read_reg_pack(ZLG9518S_RHR_REG, UART_CHAN_0, 1);
		MQ50B_Rx_Buf[i]=RS232_Rxbuf[i];
	}
	if(MQ50B_Rx_Buf[2]==0x17)    
	{
		MQ50B_Flag=0;	//������ȷ
		MQ50B.CO2=((uint16_t)MQ50B_Rx_Buf[3]<<8)|(uint16_t)MQ50B_Rx_Buf[4];
		MQ50B.CO=((uint16_t)MQ50B_Rx_Buf[5]<<8)|(uint16_t)MQ50B_Rx_Buf[6];
		MQ50B.HC=((uint16_t)MQ50B_Rx_Buf[7]<<8)|(uint16_t)MQ50B_Rx_Buf[8];
		MQ50B.NO=((uint16_t)MQ50B_Rx_Buf[9]<<8)|(uint16_t)MQ50B_Rx_Buf[10];
		MQ50B.O2=((uint16_t)MQ50B_Rx_Buf[11]<<8)|(uint16_t)MQ50B_Rx_Buf[12];
		
		sprintf((char*)&TxBuf1,"���������ǲ������ݣ�\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"CO2 ��%.4f\n",MQ50B.CO2);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"CO ��%.4f\n",MQ50B.CO);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"HC ��%d\n",MQ50B.HC);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"NO ��%d\n",MQ50B.NO);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"O2 ��%.4f\n",MQ50B.O2);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"���������ǲ�����ϣ�\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	}
	/*else if(VMT_Rx_Buf[4]==0x45)
		VTM_Flag=1;	//���ݴ���
	else
		VTM_Flag=2;*/
}

