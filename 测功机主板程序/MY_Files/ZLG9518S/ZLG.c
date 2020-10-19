#include "ZLG.h"


uint8_t ZLG_Rx_Buf[256];            // 接收缓存,最大256字节
uint8_t ZLG_Tx_Buf[256];            // 发送缓存,最大256字节
uint8_t cmd_buf[2];
uint8_t UART_CHAN[8];
uint8_t ZLG_Flag[8];

void ZLG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
		//读允许信号，低电平有效    MCU输入，
	GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//复位信号，低电平有效  MCU输出
//	GPIO_InitStruct.Pin = GPIO_PIN_14;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//配置外部输入中断，下降沿触发
	GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	//对ZLG9518S芯片进行上电复位操作 
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		
	SPI2_Init();	//初始化SPI3
}

/*中断回调函数
 *进行接收数据中断
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	uint8_t Ier_Reg[7];
	uint8_t Lsr_Reg[7];
	uint8_t i;
  if(GPIO_Pin==GPIO_PIN_11)
  {
    for(i=0;i<8;i++)
		{
			Ier_Reg[i]=zlg9518s_read_reg_pack(ZLG9518S_IER_REG,UART_CHAN[i],0);
			Lsr_Reg[i]=zlg9518s_read_reg_pack(ZLG9518S_LSR_REG,UART_CHAN[i],0);
			if(Ier_Reg[i]&&0x10==0x10)//错误中断
			{
				if(Lsr_Reg[i]&&0x40==0x40)//读错误中断,表示已经读完所有数据
					ZLG_Flag[i]=1;
				else 
					ZLG_Flag[i]=0;
			}
		}
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
  }
}

//SPI2初始化
SPI_HandleTypeDef hspi2;
void SPI2_Init(void)
{
	hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi2);
}   
/**
  * 函数功能: SPI外设系统级初始化
  * 输入参数: hspi：SPI句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI2)
  {
    /* SPI外设时钟使能 */
	__HAL_RCC_SPI2_CLK_ENABLE();
    /* GPIO外设时钟使能 */
  __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /**SPI1 GPIO Configuration    

		PB12     ------> SPI1_NSS	片选低电平有效
    PB13     ------> SPI1_SCK
    PB14     ------> SPI1_MISO
    PB15     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
		//芯片上电 片选有效
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  }
}
/**
  * 函数功能: SPI外设系统级反初始化
  * 输入参数: hspi：SPI句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI2)
  {
    /* SPI外设时钟禁用 */
    __HAL_RCC_SPI2_CLK_ENABLE();
  
    /**SPI1 GPIO Configuration    
		PB12     ------> SPI1_NSS	片选低电平有效
    PB13     ------> SPI1_SCK
    PB14     ------> SPI1_MISO
    PB15     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_12);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
  }
} 

/******************************************************************************************
** Function name: zlg9518s_set_cmd
** Descriptions: ZLG9518S 芯片的命令协议格式
** input parameters: rw：读写位，1位读，0位写；reg：寄存器地址；uart_chan：串口通道号；prm：命令参数
** output parameters: cmd_buf：命令缓存
** Returned value: 无
******************************************************************************************/ 
void zlg9518s_set_cmd(uint8_t rw, uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
 uint8_t cmd;
 cmd = S2UC_PUT_RW_BIT(rw)
 | S2UC_PUT_REG_BIT(reg)
 | S2UC_PUT_CHAN_BIT(uart_chan);

	cmd_buf[0] = cmd;
	cmd_buf[1] = prm;
}
/******************************************************************************************
** Function name: zlg9518s_send_write_cmd
** Descriptions: ZLG9518S 芯片的写一个字节
** input parameters: reg：寄存器地址；uart_chan：串口通道号；prm：命令参数；cmd_buf：缓存
** output parameters: 无
** Returned value: 无
******************************************************************************************/
void zlg9518s_send_write_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
	zlg9518s_set_cmd(S2UC_WRITE_FORM, reg, uart_chan, prm, cmd_buf);
	HAL_SPI_Transmit(&hspi2, cmd_buf, 2, 1000);
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
	HAL_SPI_Transmit(&hspi2, cmd_buf, 2, 1000);
} 
/******************************************************************************************
** Function name: zlg9518s_read_reg_pack
** Descriptions: ZLG9518S 芯片的写数据函数
** input parameters: reg：寄存器地址；uart_chan：串口通道号；prm：命令参数,读取的多少
** output parameters: 无
** Returned value: 
******************************************************************************************/
uint8_t zlg9518s_read_reg_pack(uint8_t reg, uint8_t uart_chan, uint8_t prm)
{
	zlg9518s_send_read_cmd(reg, uart_chan, prm, cmd_buf); //发送读命令 
	if (prm > 1)    //读取大于一个数据
	{			 
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10) == GPIO_PIN_RESET)
		{
			HAL_Delay(10);
			HAL_SPI_Receive(&hspi2, ZLG_Rx_Buf, prm, 1000);
		}
		return 0xFF;
	}
	else 
	{
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10) == GPIO_PIN_RESET)
		{
			HAL_Delay(10);
			HAL_SPI_Receive(&hspi2, &ZLG_Rx_Buf[0], 0x01, 1000);
		}
		return ZLG_Rx_Buf[0];
	}
}

/******************************************************************************************
** Function name: s2u_configure_one_uart
** Descriptions: 配置 ZLG9518S 芯片的一个串口
** input parameters: uart_chan: 串口通道号
** output parameters: 无
** Returned value: 无
******************************************************************************************/

void s2u_configure_one_uart(uint8_t uart_chan)
{
  
	#ifdef UART_CHAN_0
	uint8_t cmd_prm=0; /* 定义一个命令参数变量 */
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
 #endif
 
 #ifdef UART_CHAN_1
 cmd_prm=0; /* 定义一个命令参数变量 */
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
 #endif
 
 #ifdef UART_CHAN_2
 cmd_prm=0;
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
 #endif
 
 #ifdef UART_CHAN_3
 cmd_prm=0;
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
 #endif
 
 #ifdef UART_CHAN_4
 cmd_prm=0;
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
 #endif
 
 #ifdef UART_CHAN_5
 cmd_prm=0;
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
 #endif
 
 #ifdef UART_CHAN_6
 cmd_prm=0;
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
 #endif
} 
void ZLG_Uart_Init(void)
{
	UART_CHAN[0]=UART_CHAN_0;
	UART_CHAN[1]=UART_CHAN_1;
	UART_CHAN[2]=UART_CHAN_2;
	UART_CHAN[3]=UART_CHAN_3;
	UART_CHAN[4]=UART_CHAN_4;
	UART_CHAN[5]=UART_CHAN_5;
	UART_CHAN[6]=UART_CHAN_6;
	UART_CHAN[7]=UART_CHAN_7;
	s2u_configure_one_uart(UART_CHAN_0);
	s2u_configure_one_uart(UART_CHAN_1);
	s2u_configure_one_uart(UART_CHAN_2);
	s2u_configure_one_uart(UART_CHAN_3);
	s2u_configure_one_uart(UART_CHAN_4);
}

