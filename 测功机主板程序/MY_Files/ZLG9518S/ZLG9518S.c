#include "ZLG9518S.h"

uint8_t ZL_Rx_Buf[256];            // ���ջ���,���256�ֽ�
uint8_t ZL_Tx_Buf[256];            // ���ͻ���,���256�ֽ�

void ZLG9518S_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();
		//�������źţ��͵�ƽ��Ч
	GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//��λ�źţ��͵�ƽ��Ч
	GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//��ZLG9518SоƬ�����ϵ縴λ���� 
	/*HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);*/
	
	SPI3_Init();	//��ʼ��SPI1
}
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
SPI_HandleTypeDef hspi3;
void SPI3_Init(void)
{
 	//GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
  ///SPI_InitTypeDef  SPI_InitStructure;
	
	//RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	//RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
  __HAL_RCC_SPI3_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����*/
	
	GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	
	/*SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���*/
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
	
	//SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	__HAL_SPI_ENABLE(&hspi3);
	//SPI1_ReadWriteByte(0xff);//��������	 

}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
  
void SPI3_SetSpeed(u8 SpeedSet)
{
	SPI2->CR1&=0XFFC7; 
	SPI2->CR1|=SpeedSet;
	__HAL_SPI_ENABLE(&hspi3);
	//SPI_Cmd(SPI2,ENABLE); 
} 

/******************************************************************************************
** Function name: s2u_configure_one_uart
** Descriptions: ���� ZLG9518S оƬ��һ������
** input parameters: uart_chan: ����ͨ����
** output parameters: ��
** Returned value: ��
******************************************************************************************/
uint8_t cmd_buf[2];
void s2u_configure_one_uart(uint8_t uart_chan)
{
 uint8_t cmd_prm; /* ����һ������������� */
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
 
} 

/******************************************************************************************
** Function name: zlg9518s_set_cmd
** Descriptions: ZLG9518S оƬ������Э���ʽ
** input parameters: rw����дλ��reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm���������
** output parameters: cmd_buf�������
** Returned value: ��
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
** Descriptions: ZLG9518S оƬ��д����
** input parameters: reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm�����������cmd_buf������
** output parameters: ��
** Returned value: ��
******************************************************************************************/
void zlg9518s_send_write_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf)
{
 zlg9518s_set_cmd(S2UC_WRITE_FORM, reg, uart_chan, prm, cmd_buf);
 HAL_SPI_Transmit(&hspi3, cmd_buf, 2, 1000);
 //SPI_OUT(cmd_buf, 2); /* ����д���� */
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
 //SPI_OUT(cmd_buf, 2); /* ���Ͷ����� */
	HAL_SPI_Transmit(&hspi3, cmd_buf, 2, 1000);
} 

/******************************************************************************************
** Function name: zlg9518s_read_reg_pack
** Descriptions: ZLG9518S оƬ�Ķ����ݺ���
** input parameters: reg���Ĵ�����ַ��uart_chan������ͨ���ţ�prm���������
** output parameters: ��
** Returned value: ��
** prmֻ��Ϊ1
******************************************************************************************/
uint8_t zlg9518s_read_reg_pack(uint8_t reg, uint8_t uart_chan, uint8_t prm)
{
 uint8_t uart_data;
 uint8_t count;
 zlg9518s_send_read_cmd(reg, uart_chan, prm, cmd_buf); //���Ͷ����� 

 /*if (reg == ZLG9518S_RHR_REG)  // RHR �Ĵ������� 
	 {		 
		 if (prm > 1)    //��ȡ����һ������
			 {			 
					zlg9518s_read_mul_data(prm, data_buf);
					return 0;
			}
		}*/
	     //��Ҫ����INR�ж� 
	//zlg9518s_ren(); // �ж϶�ʹ���ź� �ҵȴ�10ms
	while(HAL_GPIO_ReadPin(GPIO_REN,GPIO_Pin_REN) == GPIO_PIN_SET)
	{
		HAL_Delay(1);
		count++;
		if(count>10)
			return 0;
	}
	
	HAL_SPI_Receive(&hspi3, &uart_data, prm, 1000);
	//uart_data = zlg9518s_read_data(reg_buf); // �����мĴ��� 
	return uart_data;
}
