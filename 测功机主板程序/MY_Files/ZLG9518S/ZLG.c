#include "ZLG.h"


uint8_t ZLG_Rx_Buf[256];            // ���ջ���,���256�ֽ�
uint8_t ZLG_Tx_Buf[256];            // ���ͻ���,���256�ֽ�
uint8_t cmd_buf[2];
uint8_t UART_CHAN[8];
uint8_t ZLG_Flag[8];

void ZLG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
		//�������źţ��͵�ƽ��Ч    MCU���룬
	GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//��λ�źţ��͵�ƽ��Ч  MCU���
//	GPIO_InitStruct.Pin = GPIO_PIN_14;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	//�����ⲿ�����жϣ��½��ش���
	GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	//��ZLG9518SоƬ�����ϵ縴λ���� 
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);	
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		
	SPI2_Init();	//��ʼ��SPI3
}

/*�жϻص�����
 *���н��������ж�
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
			if(Ier_Reg[i]&&0x10==0x10)//�����ж�
			{
				if(Lsr_Reg[i]&&0x40==0x40)//�������ж�,��ʾ�Ѿ�������������
					ZLG_Flag[i]=1;
				else 
					ZLG_Flag[i]=0;
			}
		}
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
  }
}

//SPI2��ʼ��
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
  * ��������: SPI����ϵͳ����ʼ��
  * �������: hspi��SPI�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI2)
  {
    /* SPI����ʱ��ʹ�� */
	__HAL_RCC_SPI2_CLK_ENABLE();
    /* GPIO����ʱ��ʹ�� */
  __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /**SPI1 GPIO Configuration    

		PB12     ------> SPI1_NSS	Ƭѡ�͵�ƽ��Ч
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
		//оƬ�ϵ� Ƭѡ��Ч
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
  }
}
/**
  * ��������: SPI����ϵͳ������ʼ��
  * �������: hspi��SPI�������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{

  if(hspi->Instance==SPI2)
  {
    /* SPI����ʱ�ӽ��� */
    __HAL_RCC_SPI2_CLK_ENABLE();
  
    /**SPI1 GPIO Configuration    
		PB12     ------> SPI1_NSS	Ƭѡ�͵�ƽ��Ч
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
** Descriptions: ZLG9518S оƬ������Э���ʽ
** input parameters: rw����дλ��1λ����0λд��reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm���������
** output parameters: cmd_buf�������
** Returned value: ��
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
** Descriptions: ZLG9518S оƬ��дһ���ֽ�
** input parameters: reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm�����������cmd_buf������
** output parameters: ��
** Returned value: ��
******************************************************************************************/
void zlg9518s_send_write_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
	zlg9518s_set_cmd(S2UC_WRITE_FORM, reg, uart_chan, prm, cmd_buf);
	HAL_SPI_Transmit(&hspi2, cmd_buf, 2, 1000);
}

/******************************************************************************************
** Function name: zlg9518s_send_read_cmd
** Descriptions: ZLG9518S оƬ�Ķ�����
** input parameters: reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm�����������cmd_buf������
** output parameters: ��
** Returned value: ��
******************************************************************************************/
void zlg9518s_send_read_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
	zlg9518s_set_cmd(S2UC_READ_FORM, reg, uart_chan, prm, cmd_buf);
	HAL_SPI_Transmit(&hspi2, cmd_buf, 2, 1000);
} 
/******************************************************************************************
** Function name: zlg9518s_read_reg_pack
** Descriptions: ZLG9518S оƬ��д���ݺ���
** input parameters: reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm���������,��ȡ�Ķ���
** output parameters: ��
** Returned value: 
******************************************************************************************/
uint8_t zlg9518s_read_reg_pack(uint8_t reg, uint8_t uart_chan, uint8_t prm)
{
	zlg9518s_send_read_cmd(reg, uart_chan, prm, cmd_buf); //���Ͷ����� 
	if (prm > 1)    //��ȡ����һ������
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
** Descriptions: ���� ZLG9518S оƬ��һ������
** input parameters: uart_chan: ����ͨ����
** output parameters: ��
** Returned value: ��
******************************************************************************************/

void s2u_configure_one_uart(uint8_t uart_chan)
{
  
	#ifdef UART_CHAN_0
	uint8_t cmd_prm=0; /* ����һ������������� */
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_1
 cmd_prm=0; /* ����һ������������� */
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_2
 cmd_prm=0;
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_3
 cmd_prm=0;
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_4
 cmd_prm=0;
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_5
 cmd_prm=0;
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
 */
 cmd_prm = S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 
 | S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TLR_REG, uart_chan, cmd_prm, cmd_buf);
 #endif
 
 #ifdef UART_CHAN_6
 cmd_prm=0;
 /*
 * ���ô���Ϊ 8 λ���ݳ��ȡ���У�鷽ʽ��һλֹͣλ�ʹ�״̬
 */
 cmd_prm = S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE)
	| S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 
	| S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 
	| S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCRO_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ô��ڵĲ�����Ϊ 115200bps���ر����ع��ܡ������жϺʹ����жϣ�ʹ�� FIFO �ж�
 */
 cmd_prm = S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 
	| S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 
	| S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 
	| S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN)
	| S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS)
	| S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS);
	
 zlg9518s_send_write_cmd(ZLG9518S_LCR1_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * �������ػָ�������Ϊ 32 ���ֽڣ���ֹ������Ϊ 224 �ֽ�
 */ 
 cmd_prm = S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 
 | S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255);
 
 zlg9518s_send_write_cmd(ZLG9518S_TCR_REG, uart_chan, cmd_prm, cmd_buf);

 /*
 * ���ý��� FIFO �жϴ�����Ϊ 224 ���ֽڣ����� FIFO ������Ϊ 240 ���ֽ�
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

