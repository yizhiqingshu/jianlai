#include "ZLG9518S.h"

uint8_t ZL_Rx_Buf[256];            // 接收缓存,最大256字节
uint8_t ZL_Tx_Buf[256];            // 发送缓存,最大256字节

void ZLG9518S_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
		//读允许信号，低电平有效
	GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//复位信号，低电平有效
	GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//对ZLG9518S芯片进行上电复位操作 
	/*HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);*/
	
	SPI3_Init();	//初始化SPI1
}
//SPI口初始化
//这里针是对SPI1的初始化
SPI_HandleTypeDef hspi3;
void SPI3_Init(void)
{
 	//GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  ///SPI_InitTypeDef  SPI_InitStructure;
	
	//RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	//RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
  __HAL_RCC_SPI3_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉*/
	
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	
	/*SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器*/
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi3.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.CRCPolynomial = 7;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
	
	//SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	__HAL_SPI_ENABLE(&hspi3);
	//SPI1_ReadWriteByte(0xff);//启动传输	 

}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
  
void SPI3_SetSpeed(u8 SpeedSet)
{
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&hspi3);
	//SPI_Cmd(SPI2,ENABLE); 
} 

/******************************************************************************************
** Function name: s2u_configure_one_uart
** Descriptions: 配置 ZLG9518S 芯片的一个串口
** input parameters: uart_chan: 串口通道号
** output parameters: 无
** Returned value: 无
******************************************************************************************/
uint8_t cmd_buf[2];
void s2u_configure_one_uart(uint8_t uart_chan)
{
 uint8_t cmd_prm; /* 定义一个命令参数变量 */
 /*
 * 配置串口为 8 位数据长度、无校验方式、一位停止位和打开状态
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * 配置串口的波特率为 115200bps、关闭流控功能、流控中断和错误中断，使能 FIFO 中断
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * 配置流控恢复触发点为 32 个字节，终止触发点为 224 字节
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * 配置接收 FIFO 中断触发点为 224 个字节，发送 FIFO 触发点为 240 个字节
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 
} 

/******************************************************************************************
** Function name: zlg9518s_set_cmd
** Descriptions: ZLG9518S 芯片的命令协议格式
** input parameters: rw：读写位；reg：寄存器地址；uart_chan：串口通道号；prm：命令参数
** output parameters: cmd_buf：命令缓存
** Returned value: 无
******************************************************************************************/ 
void zlg9518s_set_cmd(uint8_t rw, uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
 uint8_t cmd;
 cmd = S2UC_PUT_RW_BIT(rw)
 | S2UC_PUT_REG_BIT(reg)
 | S2UC_PUT_CHAN_BIT(uart_chan);
// | S2UC_PUT_DATA_BIT(prm);

// cmd_buf[0] = (cmd >> 8) & 0xFF;
// cmd_buf[1] = (cmd >> 0) & 0xFF;
	cmd_buf[0] = cmd;
	cmd_buf[1] = prm;
} 

/******************************************************************************************
** Function name: zlg9518s_send_write_cmd
** Descriptions: ZLG9518S 芯片的写命令
** input parameters: reg：寄存器地址；uart_chan：串口通道号；prm：命令参数；cmd_buf：缓存
** output parameters: 无
** Returned value: 无
******************************************************************************************/
void zlg9518s_send_write_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
 zlg9518s_set_cmd(S2UC_WRITE_FORM, reg, uart_chan, prm, cmd_buf);
 HAL_SPI_Transmit(&hspi3, cmd_buf, 2, 1000);
 //SPI_OUT(cmd_buf, 2); /* 发送写命令 */
} 

/******************************************************************************************
** Function name: zlg9518s_send_read_cmd
** Descriptions: ZLG9518S 芯片的读命令
** input parameters: reg：寄存器地址；uart_chan：串口通道号；prm：命令参数；cmd_buf：缓存
** output parameters: 无
** Returned value: 无
******************************************************************************************/
void zlg9518s_send_read_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
 zlg9518s_set_cmd(S2UC_READ_FORM, reg, uart_chan, prm, cmd_buf);
 //SPI_OUT(cmd_buf, 2); /* 发送读命令 */
	HAL_SPI_Transmit(&hspi3, cmd_buf, 2, 1000);
} 

/******************************************************************************************
** Function name: zlg9518s_read_reg_pack
** Descriptions: ZLG9518S 芯片的读数据函数
** input parameters: reg：寄存器地址；uart_chan：串口通道号；prm：命令参数
** output parameters: 无
** Returned value: 无
** prm只能为1
******************************************************************************************/
uint8_t zlg9518s_read_reg_pack(uint8_t reg, uint8_t uart_chan, uint8_t prm)
{
 uint8_t uart_data;
 uint8_t count;
 zlg9518s_send_read_cmd(reg, uart_chan, prm, cmd_buf); //发送读命令 

 /*if (reg == ZLG9518S_RHR_REG)  // RHR 寄存器连读 
	 {		 
		 if (prm > 1)    //读取大于一个数据
			 {			 
					zlg9518s_read_mul_data(prm, data_buf);
					return 0;
			}
		}*/
	     //需要加入INR判断 
	//zlg9518s_ren(); // 判断读使能信号 且等待10ms
	while(HAL_GPIO_ReadPin(GPIO_REN,GPIO_Pin_REN) == GPIO_PIN_SET)
	{
		HAL_Delay(1);
		count++;
		if(count>10)
			return 0;
	}
	
	HAL_SPI_Receive(&hspi3, &uart_data, prm, 1000);
	//uart_data = zlg9518s_read_data(reg_buf); // 读所有寄存器 
	return uart_data;
}
