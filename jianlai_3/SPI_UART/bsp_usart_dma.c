/*******************************************************************************************************
**------------------------------------------------------------------------------------------------------
** �ļ�: Usart.c 
** �汾: v1.0
**------------------------------------------------------------------------------------------------------
** ����:
**      ����ģ��
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "bsp_usart_dma.h"

#define  USART2_RXNE  1
#define  USART2_IDLE  0
#define  USART2_INT   USART2_RXNE

uint8_t  Touch_tx_buffer[16];   /* ���ڷ��ͻ��� */
uint8_t  Touch_rx_buffer[16];   /* ���ڽ��ܻ��� */
uint16_t Touch_tx_flag = 0;       /* ����һ֡���ݱ�־������¼�������ݸ��� */
	 


/********************************************************************************************************
** ����: USART1_Init,  ����1��ʼ��
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��											  
********************************************************************************************************/
void USART2_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ��GPIO��USART������ʱ�� */
	RCC_APB2PeriphClockCmd(UART_X_GPIOrcc | UART_X_GPIOAF_rcc, ENABLE);
	RCC_APB1PeriphClockCmd(UART_X_RCCcmd, ENABLE);

	/* ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=UART_X_GPIO_PINTX;		       // �������PA2(TX)
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    // IO��Ƶ��
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	   // �����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  			   // ��ʼ��
	GPIO_InitStructure.GPIO_Pin=UART_X_GPIO_PINRX;		   // ��������PA3(RX)
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;// ��������
	GPIO_Init(GPIOA,&GPIO_InitStructure); 			   // ��ʼ�� 

	/* ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = bound;    /* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART_X, &USART_InitStructure);

	USART_ITConfig(UART_X, USART_IT_IDLE, ENABLE);    /* ʹ�ܴ��ڿ����ж� */
	USART_ClearFlag(UART_X, USART_FLAG_TC | USART_FLAG_IDLE);
	USART_Cmd(UART_X, ENABLE);        /* ʹ�ܴ��� */
	/* ���������շ�DMA���� */
	USART_DMACmd(UART_X,USART_DMAReq_Tx | USART_DMAReq_Rx,ENABLE);

	/* ʹ�ܴ���2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel = UART_X_NVICn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************************************************
** ����: USART1_DMA_Init,  ����1��ʼ���� ���ô���1��DMA��������DMA�����жϡ�
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��											  
********************************************************************************************************/
void USART2_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* ��DMAͨ��ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	DMA_DeInit(UART_X_DMA_TXCH);//TX ͨ��
	DMA_DeInit(UART_X_DMA_RXCH);//RX ͨ��

	/* TX ͨ�� ����*/
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

	/* RX ͨ�� ����*/
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

	/* ʹ��DMA�շ�ͨ�� */
	DMA_Cmd(UART_X_DMA_TXCH,ENABLE);
	DMA_Cmd(UART_X_DMA_RXCH,ENABLE);

	/* ����û��ʹ��DMA������ɻ��߷�������жϣ�����Ҫ��������������� */
}

/********************************************************************************************************
** ����: USART2_IRQHandler,  ����1�жϷ�����
**------------------------------------------------------------------------------------------------------
** ����: ��
** ����: ��											  
********************************************************************************************************/
/********************************************************************************************************
** ����: fput,  �ض���,��ʹ��printf����ʱ�Զ�����
**------------------------------------------------------------------------------------------------------
** ����: int ch, FILE *p
** ����: ch											  
********************************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(UART_X,(uint8_t)ch);
		/* �ȴ�������� */
		while (USART_GetFlagStatus(UART_X,USART_FLAG_TXE)==RESET);		
	
		return (ch);
}

u16 USART2_RX_STA=0;
void UART_X_HANDLER(void)
{
	uint8_t  ClearFlag;
//	uint16_t Count = 0;

	/* ������տ����ж�  */
	if(USART_GetITStatus(UART_X,USART_IT_IDLE)!=RESET)
	{
		/* �����־���ȶ�SR���ٶ�DR */
		ClearFlag = UART_X->SR;
		ClearFlag = UART_X->DR;
		
		
		/* ��ȡ�������ݸ��� */
//		Count = DMA_GetCurrDataCounter(UART_X_DMA_RXCH);
//		Touch_tx_flag = 16 - Count;
		
		/* �������ݣ�����������ʾ�����յ�������ʹ��DMA���ͳ�ȥ */
		
		
		//memcpy((uint8_t *)Touch_tx_buffer,(uint8_t *)Touch_rx_buffer,Touch_tx_flag);
		if(Touch_rx_buffer[6]==0x5b)
		{
		OPenMV_ARR((uint8_t *)Touch_rx_buffer);  
	    Openmv_Data();//openmv���ݴ�����	
		}
		//USART2_RX_STA=0;
		
//		USART2_RX_STA++;
//		if(USART2_RX_STA==4)
//		{
//		OPenMV_ARR((uint8_t *)Touch_rx_buffer);  
//	    Openmv_Data();//openmv���ݴ�����	
//		USART2_RX_STA=0;
//		Touch_tx_flag=1;
//		}
		
		
//		if(Touch_tx_flag!=0)
//		{
//		Touch_rx_buffer[i];
//		if(Touch_rx_buffer[0]!=0xb3)//֡ͷ
//			i=0;
//		if((i==2)&&(Touch_rx_buffer[1]!=0xb3))//�ж�֡ͷ
//			i=0;
//		if(i>=7)//����һ֡�������
//		{
//			memcpy(Touch_tx_buffer,Touch_rx_buffer,i);			
//			i = 0;
//		}
//		}
		/* DMA���ո�λ */
		DMA_Cmd(UART_X_DMA_RXCH,DISABLE);
		DMA_SetCurrDataCounter(UART_X_DMA_RXCH,16);
		DMA_Cmd(UART_X_DMA_RXCH,ENABLE);
	}
}

void Delay_us(u32 i)
{
    u32 temp;
    SysTick->LOAD=9*i;         //������װ��ֵ, 72MHZʱ
    SysTick->CTRL=0X01;         //ʹ�ܣ����������޶����������ⲿʱ��Դ
    SysTick->VAL=0;                //���������
    do
    {
        temp=SysTick->CTRL;           //��ȡ��ǰ������ֵ
    }
    while((temp&0x01)&&(!(temp&(1<<16))));     //�ȴ�ʱ�䵽��
    SysTick->CTRL=0;    //�رռ�����
    SysTick->VAL=0;        //��ռ�����
}


void Delay_ms(u32 i)
{
    u32 temp;
    SysTick->LOAD=9000*i;      //������װ��ֵ, 72MHZʱ
    SysTick->CTRL=0X01;        //ʹ�ܣ����������޶����������ⲿʱ��Դ
    SysTick->VAL=0;            //���������
    do
    {
        temp=SysTick->CTRL;       //��ȡ��ǰ������ֵ
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //�ȴ�ʱ�䵽��
    SysTick->CTRL=0;    //�رռ�����
    SysTick->VAL=0;        //��ռ�����
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
else if(State==4)		//����Ƿ���ܵ�������־
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


int16_t OpenMV_X;          /*OPENMV X �ᷴ������*/
int16_t OpenMV_Y;          /*OPENMV X �ᷴ������*/
int16_t OpenMV_X1;          /*OPENMV X �ᷴ������*/
int16_t OpenMV_Y1;          /*OPENMV X �ᷴ������*/

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

