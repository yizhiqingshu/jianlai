#include "FTH-2.h"
#include "ZLG.h"

/***********
名称：FTH-2 气象站
通讯波特率为19200，1位起始位，8位数据位，1位停止位，无校验位。
精度：温度 0.1  湿度 0.1 气压 0.1
数据更新时间：0.5s
***********/

uint8_t FT_Rx_Buf[256];            // 接收缓存,最大256字节
uint8_t FT_Tx_Buf[256];            // 发送缓存,最大256字节
uint8_t FT_Flag;	//0表示数据正确，1表示数据错误,2为其他情况，需要检查代码

int16_t FT_wendu;	//FT_Rx_Buf[10]  FT_Rx_Buf[11]
uint16_t FT_shidu;	//FT_Rx_Buf[12] FT_Rx_Buf[13]
uint16_t FT_daqiya;	//FT_Rx_Buf[14]	FT_Rx_Buf[15]

extern uint8_t cmd_buf[2];

/***********
发送指令	读状态和数据命令
4EH 54H 33H 33H 53H	起始字节
03H									长度
01H 01H							命令
FAH									校验和
***********/
void Tx_FTH(void)	
{
	uint8_t i;
	/*读状态协议码*/
	FT_Tx_Buf[0]=0x4E;
	FT_Tx_Buf[1]=0x54;
	FT_Tx_Buf[2]=0x33;
	FT_Tx_Buf[3]=0x33;
	FT_Tx_Buf[4]=0x53;
	
	FT_Tx_Buf[5]=0x03;	//长度字节
	
	FT_Tx_Buf[6]=0x01;	//命令高字节
	FT_Tx_Buf[7]=0x01;	//命令低字节
	
	FT_Tx_Buf[8]=0xFA;	//校验和字节
	/**************
	发送状态指令
	**************/
	for(i=0;i<9;i++)
	{
		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, FT_Tx_Buf[i],cmd_buf);			//需要修改，所有仪器用的都是一个通道
	}
}
/**********
接收数据
应答指令：
4EH 54H 33H 33H 41H		起始字节
0DH										长度字节
01H 01H								命令字节
ST01									状态字 8bit unsigned
ST02									状态字 U8
CT01 CT02							温度（x10） 16bit signed
CT03 CT04							湿度（x10） 16bit unsigned
CT05 CT06							大气压（x10）16bit unsigned
CT07 CT08							待定（TBD1）
CS										校验和
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
		FT_Flag=0;	//数据正确
		FT_wendu=(((int16_t)FT_Rx_Buf[10]<<8)|(int16_t)FT_Rx_Buf[11])*10;
		FT_shidu=(((uint16_t)FT_Rx_Buf[12]<<8)|(uint16_t)FT_Rx_Buf[13])*10;
		FT_daqiya=(((uint16_t)FT_Rx_Buf[14]<<8)|(uint16_t)FT_Rx_Buf[15])*10;
	}
	else if(FT_Rx_Buf[4]==0x45)
		FT_Flag=1;	//数据错误
	else
		FT_Flag=2;
}


