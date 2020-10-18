/*******************************************************************************************************
**------------------------------------------------------------------------------------------------------
** 文件: Usart.c 
** 版本: v1.0
**------------------------------------------------------------------------------------------------------
** 描述:
**      串口模块
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "bsp_usart_dma.h"

#define  USART2_RXNE  1
#define  USART2_IDLE  0
#define  USART2_INT   USART2_RXNE

uint8_t  Touch_tx_buffer[16];   /* 串口发送缓存 */
uint8_t  Touch_rx_buffer[16];   /* 串口接受缓存 */
uint16_t Touch_tx_flag = 0;       /* 接收一帧数据标志，并记录接收数据个数 */
	 


/********************************************************************************************************
** 函数: USART1_Init,  串口1初始化
**------------------------------------------------------------------------------------------------------
** 参数: 无
** 返回: 无											  
********************************************************************************************************/
void USART2_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 打开GPIO和USART部件的时钟 */
	RCC_APB2PeriphClockCmd(UART_X_GPIOrcc | UART_X_GPIOAF_rcc, ENABLE);
	RCC_APB1PeriphClockCmd(UART_X_RCCcmd, ENABLE);

	/* 配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=UART_X_GPIO_PINTX;		       // 串口输出PA2(TX)
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    // IO口频率
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	   // 复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			   // 初始化
	GPIO_InitStructure.GPIO_Pin=UART_X_GPIO_PINRX;		   // 串口输入PA3(RX)
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;// 浮空输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); 			   // 初始化 

	/* 配置串口硬件参数 */
	USART_InitStructure.USART_BaudRate = bound;    /* 波特率 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART_X, &USART_InitStructure);

	USART_ITConfig(UART_X, USART_IT_IDLE, ENABLE);    /* 使能串口空闲中断 */
	USART_ClearFlag(UART_X, USART_FLAG_TC | USART_FLAG_IDLE);
	USART_Cmd(UART_X, ENABLE);        /* 使能串口 */
	/* 开启串口收发DMA请求 */
	USART_DMACmd(UART_X,USART_DMAReq_Tx | USART_DMAReq_Rx,ENABLE);

	/* 使能串口2中断 */
	NVIC_InitStructure.NVIC_IRQChannel = UART_X_NVICn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************************************************
** 函数: USART1_DMA_Init,  串口1初始化。 配置串口1的DMA，并开启DMA接收中断。
**------------------------------------------------------------------------------------------------------
** 参数: 无
** 返回: 无											  
********************************************************************************************************/
void USART2_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* 打开DMA通道时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	DMA_DeInit(UART_X_DMA_TXCH);//TX 通道
	DMA_DeInit(UART_X_DMA_RXCH);//RX 通道

	/* TX 通道 配置*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART_X->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Touch_tx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UART_X_DMA_TXCH,&DMA_InitStructure);

	/* RX 通道 配置*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(UART_X->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Touch_rx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1024;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UART_X_DMA_RXCH,&DMA_InitStructure);

	/* 使能DMA收发通道 */
	DMA_Cmd(UART_X_DMA_TXCH,ENABLE);
	DMA_Cmd(UART_X_DMA_RXCH,ENABLE);

	/* 这里没有使用DMA接收完成或者发送完成中断，如需要，可以在这里添加 */
}

/********************************************************************************************************
** 函数: USART2_IRQHandler,  串口1中断服务函数
**------------------------------------------------------------------------------------------------------
** 参数: 无
** 返回: 无											  
********************************************************************************************************/
/********************************************************************************************************
** 函数: fput,  重定向,在使用printf函数时自动调用
**------------------------------------------------------------------------------------------------------
** 参数: int ch, FILE *p
** 返回: ch											  
********************************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(UART_X,(uint8_t)ch);
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(UART_X,USART_FLAG_TXE)==RESET);		
	
		return (ch);
}

u16 USART2_RX_STA=0;
void UART_X_HANDLER(void)
{
	uint8_t  ClearFlag;
//	uint16_t Count = 0;

	/* 处理接收空闲中断  */
	if(USART_GetITStatus(UART_X,USART_IT_IDLE)!=RESET)
	{
		/* 清除标志：先读SR，再读DR */
		ClearFlag = UART_X->SR;
		ClearFlag = UART_X->DR;
		
		
		/* 获取接收数据个数 */
//		Count = DMA_GetCurrDataCounter(UART_X_DMA_RXCH);
//		Touch_tx_flag = 16 - Count;
		
		/* 处理数据，这里用于演示，将收到的数据使用DMA发送出去 */
		
		
		//memcpy((uint8_t *)Touch_tx_buffer,(uint8_t *)Touch_rx_buffer,Touch_tx_flag);
		if(Touch_rx_buffer[6]==0x5b)
		{
		OPenMV_ARR((uint8_t *)Touch_rx_buffer);  
	    Openmv_Data();//openmv数据处理函数	
		}
		//USART2_RX_STA=0;
		
//		USART2_RX_STA++;
//		if(USART2_RX_STA==4)
//		{
//		OPenMV_ARR((uint8_t *)Touch_rx_buffer);  
//	    Openmv_Data();//openmv数据处理函数	
//		USART2_RX_STA=0;
//		Touch_tx_flag=1;
//		}
		
		
//		if(Touch_tx_flag!=0)
//		{
//		Touch_rx_buffer[i];
//		if(Touch_rx_buffer[0]!=0xb3)//帧头
//			i=0;
//		if((i==2)&&(Touch_rx_buffer[1]!=0xb3))//判断帧头
//			i=0;
//		if(i>=7)//代表一帧数据完毕
//		{
//			memcpy(Touch_tx_buffer,Touch_rx_buffer,i);			
//			i = 0;
//		}
//		}
		/* DMA接收复位 */
		DMA_Cmd(UART_X_DMA_RXCH,DISABLE);
		DMA_SetCurrDataCounter(UART_X_DMA_RXCH,16);
		DMA_Cmd(UART_X_DMA_RXCH,ENABLE);
	}
}

void Delay_us(u32 i)
{
    u32 temp;
    SysTick->LOAD=9*i;         //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;                //清零计数器
    do
    {
        temp=SysTick->CTRL;           //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}


void Delay_ms(u32 i)
{
    u32 temp;
    SysTick->LOAD=9000*i;      //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;        //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;            //清零计数器
    do
    {
        temp=SysTick->CTRL;       //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}
//openmv chulishuju 
uint8_t ARR_OPENMV[7];
uint16_t i=0;
void OPenMV_ARR(uint8_t* ARRDATA)	
{
static u8 State = 0;
if(State==0&&ARRDATA[0]==0x2c)
{
		State=1;
		ARR_OPENMV[0]=ARRDATA[0];
}
if(State==1&&ARRDATA[1]==0x12)
	{
		State=2;
		ARR_OPENMV[1]=ARRDATA[1];
	}
if(State==2)
	{
		State=4;
		ARR_OPENMV[2]=ARRDATA[2];
		ARR_OPENMV[3]=ARRDATA[3];
		ARR_OPENMV[4]=ARRDATA[4];
		ARR_OPENMV[5]=ARRDATA[5];
		Touch_tx_flag=1;
	}
//else if(State==3)
//	{
//		State = 4;
//		ARR_OPENMV[3]=ARRDATA[3];
//	}
else if(State==4)		//检测是否接受到结束标志
	{
        if(ARRDATA[6] == 0x5B)
        {
            State = 0;
//     ARR_OPENMV[4]=ARRDATA[4];
            Openmv_Data();
        }
        else if(ARRDATA[6]!= 0x5B)
        {
            State = 0;
            for(i=0;i<7;i++)
            {
                ARR_OPENMV[i]=0x00;
            }           
        }
	}    
else
		{
			State = 0;
            for(i=0;i<7;i++)
            {
                ARR_OPENMV[i]=0x00;
            }
		}
}


int16_t OpenMV_X;          /*OPENMV X 轴反馈坐标*/
int16_t OpenMV_Y;          /*OPENMV X 轴反馈坐标*/
int16_t OpenMV_X1;          /*OPENMV X 轴反馈坐标*/
int16_t OpenMV_Y1;          /*OPENMV X 轴反馈坐标*/

void Openmv_Data(void)
{
    OpenMV_X=ARR_OPENMV[2];
    OpenMV_Y=ARR_OPENMV[3];
	OpenMV_X1=ARR_OPENMV[4];
    OpenMV_Y1=ARR_OPENMV[5];
}



/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

