#include "VMT_2000.h"
#include "ZLG.h"
#include "usart.h"
/***********
名称：发动机测速， 用于柴油车检测
测量范围：360rpm～10000rpm（四缸四冲程）；
分辨率：  1rpm；
数据更新时间：1s；
测量精度：±10rpm，或±1%

通讯波特率为19200，1位起始位，8位数据位，1位停止位，无校验位。
***********/

uint8_t VMT_Rx_Buf[256];            // 接收缓存,最大256字节
uint8_t VMT_Tx_Buf[256];            // 发送缓存,最大256字节
uint16_t VMT_rpm;	//转速	[10] [11]
uint16_t VMT_q;	//频率	[12] [13]
uint8_t cylinder=4;	//被测车辆的缸数   4为默认值
uint8_t n=1; //1,单点火,2双点火    1为默认值
uint8_t VTM_Flag;	//0表示数据正确，1表示数据错误,2为其他情况，需要检查代码
extern uint8_t cmd_buf[2];
extern uint8_t RS232_flag;

//读系统状态和数据
void Tx_VMT(void)
{
//	uint8_t i;
	/*读状态协议码*/
	VMT_Tx_Buf[0]=0x4E;	//[0]-[4]为起始字节
	VMT_Tx_Buf[1]=0x54;
	VMT_Tx_Buf[2]=0x30;
	VMT_Tx_Buf[3]=0x31;
	VMT_Tx_Buf[4]=0x53;
	
	VMT_Tx_Buf[5]=0x03;	//长度字节
	
	VMT_Tx_Buf[6]=0x01;	//命令高字节
	VMT_Tx_Buf[7]=0x01;	//命令低字节
	
	VMT_Tx_Buf[8]=0xFA;	//校验和字节
	/**************
	发送状态指令
	**************/
//	for(i=0;i<9;i++)
//	{
//		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, VMT_Tx_Buf[i],cmd_buf);	//需要修改，所有仪器用的都是一个通道
//	}
		HAL_UART_Transmit(&husart5,VMT_Tx_Buf,strlen((char *)VMT_Tx_Buf),1000);
//		RS232_flag=3;
}

/*接收指令
转速	VMT_Rx_Buf[10] VMT_Rx_Buf[11]
频率	VMT_Rx_Buf[12] VMT_Rx_Buf[13]
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
		VTM_Flag=0;	//数据正确
		VMT_rpm=((uint16_t)VMT_Rx_Buf[10]<<8)|(uint16_t)VMT_Rx_Buf[11];
		VMT_q=(((uint16_t)VMT_Rx_Buf[12]<<8)|(uint16_t)VMT_Rx_Buf[13])*100;
	}
	VMT_rpm=VMT_rpm*4/(cylinder*n);	//标准为4缸4冲层，单点火，进行测量转速的换算
	/*else if(VMT_Rx_Buf[4]==0x45)
		VTM_Flag=1;	//数据错误
	else
		VTM_Flag=2;*/
}



