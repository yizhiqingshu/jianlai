

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "NRF24l01.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};

/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ��ʼ��IO��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */
void NRF24L01_Init(void)
{ 	
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
  
  NRF24L01_SPI_APBxClock_FUN(NRF24L01_SPI_CLK,ENABLE);  
  NRF24L01_SPI_GPIO_APBxClock_FUN(NRF24L01_SPI_GPIO_CLK|NRF24L01_SPI_CS_CLK|
                                  NRF24L01_CE_CLK|NRF24L01_IRQ_CLK,ENABLE);     
  
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_CS_PIN;				 //PB12���� ��ֹW25X�ĸ���
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(NRF24L01_SPI_CS_PORT, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	NRF24L01_SPI_CS_DISABLE();//����				
 	
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE_PIN; 
 	GPIO_Init(NRF24L01_CE_PORT, &GPIO_InitStructure);//��ʼ��ָ��IO
  
	GPIO_InitStructure.GPIO_Pin  = NRF24L01_IRQ_PIN;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PG6 ����  
	GPIO_Init(NRF24L01_IRQ_PORT, &GPIO_InitStructure);
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_SCK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(NRF24L01_SPI_SCK_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_MISO_PIN;
  GPIO_Init(NRF24L01_SPI_MISO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = NRF24L01_SPI_MOSI_PIN;
  GPIO_Init(NRF24L01_SPI_MOSI_PORT, &GPIO_InitStructure);
  
   
	SPI_Cmd(NRF24L01_SPIx, DISABLE); // SPI���費ʹ��
   
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI����
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(NRF24L01_SPIx, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(NRF24L01_SPIx, ENABLE); //ʹ��SPI����
			 
	NRF24L01_CE_LOW(); 			//ʹ��NRF24L01
	SD_SPI_2_CS();
	W25Q64_SPI_2_CS();
	NRF24L01_SPI_CS_DISABLE();			//SPIƬѡȡ��  	 		 	 
}

/**
  * ��������: SPI �ٶ����ú���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������           
  */ 
void SPIx_SetSpeed(SPI_TypeDef* SPIx, uint8_t SPI_BaudRatePrescaler)
{
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIx->CR1&=0XFFC7;
	SPIx->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPIx,ENABLE); 
}

/**
  * ��������: SPIx ��дһ���ֽ�
  * �������: Ҫд����ֽ�
  * �� �� ֵ: ��ȡ�����ֽ�
  * ˵    ������           
  */
uint8_t SPIx_ReadWriteByte(SPI_TypeDef* SPIx, uint8_t TxData)
{		
	uint8_t retry=0;
  /* Check the parameters */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
  {
    retry++;
    if(retry>200)return 0;
  }		
  
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
  {
    retry++;
    if(retry>200)return 0;
  }	  						    
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����					    
}


/**
  * ��������: ���24L01�Ƿ����
  * �������: ��
  * �� �� ֵ: 0���ɹ�;1��ʧ��
  * ˵    ������          
  */ 
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	SPIx_SetSpeed(NRF24L01_SPIx,SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 

/**
  * ��������: SPIд�Ĵ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:ָ���Ĵ�����ַ
  *           
  */ 
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	
  NRF24L01_SPI_CS_ENABLE();                 //ʹ��SPI����
  status =SPIx_ReadWriteByte(NRF24L01_SPIx,reg);//���ͼĴ����� 
  SPIx_ReadWriteByte(NRF24L01_SPIx,value);      //д��Ĵ�����ֵ
  NRF24L01_SPI_CS_DISABLE();                 //��ֹSPI����	   
  return(status);       			//����״ֵ̬
}

/**
  * ��������: ��ȡSPI�Ĵ���ֵ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:Ҫ���ļĴ���
  *           
  */ 
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    
 	NRF24L01_SPI_CS_ENABLE();          //ʹ��SPI����		
  SPIx_ReadWriteByte(NRF24L01_SPIx,reg);   //���ͼĴ�����
  reg_val=SPIx_ReadWriteByte(NRF24L01_SPIx,0XFF);//��ȡ�Ĵ�������
  NRF24L01_SPI_CS_DISABLE();          //��ֹSPI����		    
  return(reg_val);           //����״ֵ̬
}	

/**
  * ��������: ��ָ��λ�ö���ָ�����ȵ�����
  * �������: ��
  * �� �� ֵ: �˴ζ�����״̬�Ĵ���ֵ 
  * ˵    ������
  *           
  */ 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,uint8_t_ctr;	   
  
  NRF24L01_SPI_CS_ENABLE();           //ʹ��SPI����
  status=SPIx_ReadWriteByte(NRF24L01_SPIx,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(uint8_t_ctr=0;uint8_t_ctr<len;uint8_t_ctr++)
  {
    pBuf[uint8_t_ctr]=SPIx_ReadWriteByte(NRF24L01_SPIx,0XFF);//��������
  }
  NRF24L01_SPI_CS_DISABLE();       //�ر�SPI����
  return status;        //���ض�����״ֵ̬
}

/**
  * ��������: ��ָ��λ��дָ�����ȵ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����reg:�Ĵ���(λ��)  *pBuf:����ָ��  len:���ݳ���
  *           
  */ 
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,uint8_t_ctr;	    
 	NRF24L01_SPI_CS_ENABLE();          //ʹ��SPI����
  status = SPIx_ReadWriteByte(NRF24L01_SPIx,reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(uint8_t_ctr=0; uint8_t_ctr<len; uint8_t_ctr++)
  {
    SPIx_ReadWriteByte(NRF24L01_SPIx,*pBuf++); //д������	 
  }
  NRF24L01_SPI_CS_DISABLE();       //�ر�SPI����
  return status;          //���ض�����״ֵ̬
}				   

/**
  * ��������: ����NRF24L01����һ������
  * �������: ��
  * �� �� ֵ: �������״��
  * ˵    ����txbuf:�����������׵�ַ
  *           
  */ 
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	SPIx_SetSpeed(NRF24L01_SPIx,SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE_LOW();
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE_HIGH();//��������	 
  
	while(NRF24L01_IRQ_PIN_READ()!=0);//�ȴ��������
  
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

/**
  * ��������:����NRF24L01����һ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	SPIx_SetSpeed(NRF24L01_SPIx,SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    

/**
  * ��������: �ú�����ʼ��NRF24L01��RXģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_LOW();	  
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  NRF24L01_CE_HIGH(); //CEΪ��,�������ģʽ 
}						 

/**
  * ��������: �ú�����ʼ��NRF24L01��TXģʽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  *           
  */ 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE_LOW();	    
  NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	NRF24L01_CE_HIGH();//CEΪ��,10us����������
}

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
