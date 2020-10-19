#include "VMT_2000.h"
#include "ZLG.h"
#include "usart.h"
/***********
���ƣ����������٣� ���ڲ��ͳ����
������Χ��360rpm��10000rpm���ĸ��ĳ�̣���
�ֱ��ʣ�  1rpm��
���ݸ���ʱ�䣺1s��
�������ȣ���10rpm�����1%

ͨѶ������Ϊ19200��1λ��ʼλ��8λ����λ��1λֹͣλ����У��λ��
***********/

uint8_t VMT_Rx_Buf[256];            // ���ջ���,���256�ֽ�
uint8_t VMT_Tx_Buf[256];            // ���ͻ���,���256�ֽ�
uint16_t VMT_rpm;	//ת��	[10] [11]
uint16_t VMT_q;	//Ƶ��	[12] [13]
uint8_t cylinder=4;	//���⳵���ĸ���   4ΪĬ��ֵ
uint8_t n=1; //1,�����,2˫���    1ΪĬ��ֵ
uint8_t VTM_Flag;	//0��ʾ������ȷ��1��ʾ���ݴ���,2Ϊ�����������Ҫ������
extern uint8_t cmd_buf[2];
extern uint8_t RS232_flag;

//��ϵͳ״̬������
void Tx_VMT(void)
{
//	uint8_t i;
	/*��״̬Э����*/
	VMT_Tx_Buf[0]=0x4E;	//[0]-[4]Ϊ��ʼ�ֽ�
	VMT_Tx_Buf[1]=0x54;
	VMT_Tx_Buf[2]=0x30;
	VMT_Tx_Buf[3]=0x31;
	VMT_Tx_Buf[4]=0x53;
	
	VMT_Tx_Buf[5]=0x03;	//�����ֽ�
	
	VMT_Tx_Buf[6]=0x01;	//������ֽ�
	VMT_Tx_Buf[7]=0x01;	//������ֽ�
	
	VMT_Tx_Buf[8]=0xFA;	//У����ֽ�
	/**************
	����״ָ̬��
	**************/
//	for(i=0;i<9;i++)
//	{
//		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, VMT_Tx_Buf[i],cmd_buf);	//��Ҫ�޸ģ����������õĶ���һ��ͨ��
//	}
		HAL_UART_Transmit(&husart5,VMT_Tx_Buf,strlen((char *)VMT_Tx_Buf),1000);
//		RS232_flag=3;
}

/*����ָ��
ת��	VMT_Rx_Buf[10] VMT_Rx_Buf[11]
Ƶ��	VMT_Rx_Buf[12] VMT_Rx_Buf[13]
*/
void Rx_VTM(void)
{
	uint8_t i;
	for(i=0;i<19;i++)
	{
		VMT_Rx_Buf[i]=zlg9518s_read_reg_pack(ZLG9518S_RHR_REG, UART_CHAN_0, 1);
	}
	if(VMT_Rx_Buf[4]==0x41)    
	{
		VTM_Flag=0;	//������ȷ
		VMT_rpm=((uint16_t)VMT_Rx_Buf[10]<<8)|(uint16_t)VMT_Rx_Buf[11];
		VMT_q=(((uint16_t)VMT_Rx_Buf[12]<<8)|(uint16_t)VMT_Rx_Buf[13])*100;
	}
	VMT_rpm=VMT_rpm*4/(cylinder*n);	//��׼Ϊ4��4��㣬����𣬽��в���ת�ٵĻ���
	/*else if(VMT_Rx_Buf[4]==0x45)
		VTM_Flag=1;	//���ݴ���
	else
		VTM_Flag=2;*/
}



