#include "MQW5102.h"
#include "ZLG.h"
#include "usart.h"
/**********
NOx分析仪
9600 1起始位 8数据位 无校验位 1停止位
**********/
uint8_t MQW5102_Rx_Buf[256];            // 接收缓存,最大256字节
uint8_t MQW5102_Tx_Buf[256];            // 发送缓存,最大256字节
extern uint8_t cmd_buf[2];

MQW5102_test_data MQW5102;

extern uint8_t RS232_flag;
extern uint8_t RS232_Rxbuf[256];
extern uint8_t TxBuf1[256];//1100 0010
void Tx_MQW5102(void)
{
//	uint8_t i;
	/*读状态协议码*/
	MQW5102_Tx_Buf[0]=0x05;	//起始字节
	MQW5102_Tx_Buf[1]=0x30;	//命令字节
	MQW5102_Tx_Buf[2]=0x05;	//数据长度
	MQW5102_Tx_Buf[3]=0x02; //数据区
	MQW5102_Tx_Buf[4]=0x01; //烟度计是适配位 0未连接烟度计 1连接烟度计
	MQW5102_Tx_Buf[5]=~(MQW5102_Tx_Buf[0]+MQW5102_Tx_Buf[1]+MQW5102_Tx_Buf[2]+MQW5102_Tx_Buf[3]+MQW5102_Tx_Buf[4])+0x01; //校验和
	
	/**************
	发送状态指令
	**************/
//	HAL_UART_Transmit(&husart5,MQW5102_Tx_Buf,strlen((char *)MQW5102_Tx_Buf),1000);
//	RS232_flag=2;
//	sprintf((char*)&TxBuf1,"发送的数据为：");    
//	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
//	HAL_UART_Transmit(&husart1,MQW5102_Tx_Buf,strlen((char *)MQW5102_Tx_Buf),1000);
//	for(i=0;i<6;i++)
//	{
//		zlg9518s_send_write_cmd(ZLG9518S_THR_REG, UART_CHAN_0, MQW5102_Tx_Buf[i],cmd_buf);	//需要修改，所有仪器用的都是一个通道
//	}
}

void Rx_MQW5102(void)
{
	uint8_t i;
	uint8_t flag=0;
	for(i=0;i<24;i++)
	{
		//MQW5102_Rx_Buf[i]=zlg9518s_read_reg_pack(ZLG9518S_RHR_REG, UART_CHAN_0, 1);
		MQW5102_Rx_Buf[i]=RS232_Rxbuf[i];
	}
	if(MQW5102_Rx_Buf[0]==0x06&&MQW5102_Rx_Buf[1]==0x30)    //接收错误应到0x15
	{
		flag=1;
		MQW5102.qiluya=((uint16_t)MQW5102_Rx_Buf[5]<<8)|(uint16_t)MQW5102_Rx_Buf[6];
		MQW5102.youwen=((uint16_t)MQW5102_Rx_Buf[7]<<8)|(uint16_t)MQW5102_Rx_Buf[8];
		MQW5102.zhuansu=((uint16_t)MQW5102_Rx_Buf[9]<<8)|(uint16_t)MQW5102_Rx_Buf[10];
		MQW5102.NOx=((uint16_t)MQW5102_Rx_Buf[11]<<8)|(uint16_t)MQW5102_Rx_Buf[12];
		MQW5102.CO2=((uint16_t)MQW5102_Rx_Buf[13]<<8)|(uint16_t)MQW5102_Rx_Buf[14];
		MQW5102.N=((uint16_t)MQW5102_Rx_Buf[15]<<8)|(uint16_t)MQW5102_Rx_Buf[16];
		MQW5102.Ns=((uint16_t)MQW5102_Rx_Buf[17]<<8)|(uint16_t)MQW5102_Rx_Buf[18];
		MQW5102.K=((uint16_t)MQW5102_Rx_Buf[19]<<8)|(uint16_t)MQW5102_Rx_Buf[20];
		MQW5102.yanqi_T=((uint16_t)MQW5102_Rx_Buf[21]<<8)|(uint16_t)MQW5102_Rx_Buf[22];
	}
	if(flag==1)
	{
		MQW5102.CO2=MQW5102.CO2/100;
		MQW5102.qiluya=MQW5102.qiluya/10;
		MQW5102.youwen=MQW5102.youwen/10;
		MQW5102.N=MQW5102.N/100;
		MQW5102.Ns=MQW5102.Ns/100;
		MQW5102.K=MQW5102.K/100;
	}
	sprintf((char*)&TxBuf1,"NOx分析仪测量数据：\n");    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.CO2);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.qiluya);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.youwen);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.N);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.Ns);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"CO2：%.4f\n",MQW5102.K);    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
	sprintf((char*)&TxBuf1,"NOx分析仪测量完毕：\n");    
	HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
}


