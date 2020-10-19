#include "50B.h"
#include "ZLG.h"
#include "usart.h"

/***********
名称：五气分析仪，用于汽油车的检测
波特率 9600 起始位 1 数据位 8 校验位 无 停止位 1
***********/

uint8_t MQ50B_Rx_Buf[256]={0};            // 接收缓存,最大256字节
uint8_t MQ50B_Tx_Buf[256];            // 发送缓存,最大256字节

uint8_t MQ50B_Flag; //0表示数据正确，1其他情况，需要检查代码
extern uint8_t cmd_buf[2];
extern uint8_t RS232_flag;
extern uint8_t RS232_Rxbuf[256];

extern uint8_t TxBuf1[256];

MQ50B_test_data MQ50B;
/*读系统状态和数据
	实时获取数据  
	命令格式：02H-A3H-03H-58H
*/
void Tx_50B(void)
{
//	uint8_t i;
	/*读状态协议码*/
	MQ50B_Tx_Buf[0]=0x02;	//起始字节
	MQ50B_Tx_Buf[1]=0xA3;	//命令字节
	MQ50B_Tx_Buf[2]=0x03;	//数据长度
	MQ50B_Tx_Buf[3]=0x58; //校验和
	
	/**************
	发送状态指令
	**************/
	//memcpy(txbuf,"这是一个RS232和RS485同时使用测试实验\n",100);
  HAL_UART_Transmit(&husart5,MQ50B_Tx_Buf,strlen((char *)MQ50B_Tx_Buf),1000);
	RS232_flag=1;
//	for(i=0;i<4;i++)
//	{
//		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, MQ50B_Tx_Buf[i],cmd_buf);	//需要修改，所有仪器用的都是一个通道
//	}
}

/*接收指令
响应格式 06H-A3H-17H-CO2高位-CO2低位-CO高位-CO低位-HC高位-HC
					 低位-NO高位-NO低位-O2高位-O2低位-油温高位-油温低位-转
					 速高位-转速低位-气路压力高位-气路压力低位-空燃比高位-空
					 燃比低位-PEF值高位-PEF值低位-校验码

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
		MQ50B_Flag=0;	//数据正确
		MQ50B.CO2=((uint16_t)MQ50B_Rx_Buf[3]<<8)|(uint16_t)MQ50B_Rx_Buf[4];
		MQ50B.CO=((uint16_t)MQ50B_Rx_Buf[5]<<8)|(uint16_t)MQ50B_Rx_Buf[6];
		MQ50B.HC=((uint16_t)MQ50B_Rx_Buf[7]<<8)|(uint16_t)MQ50B_Rx_Buf[8];
		MQ50B.NO=((uint16_t)MQ50B_Rx_Buf[9]<<8)|(uint16_t)MQ50B_Rx_Buf[10];
		MQ50B.O2=((uint16_t)MQ50B_Rx_Buf[11]<<8)|(uint16_t)MQ50B_Rx_Buf[12];
		
		sprintf((char*)&TxBuf1,"五气分析仪测量数据：\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"CO2 ：%.4f\n",MQ50B.CO2);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"CO ：%.4f\n",MQ50B.CO);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"HC ：%d\n",MQ50B.HC);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"NO ：%d\n",MQ50B.NO);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"O2 ：%.4f\n",MQ50B.O2);    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
		sprintf((char*)&TxBuf1,"五气分析仪测量完毕：\n");    
		HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	}
	/*else if(VMT_Rx_Buf[4]==0x45)
		VTM_Flag=1;	//数据错误
	else
		VTM_Flag=2;*/
}

