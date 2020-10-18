#ifndef __NRF24l01_H__
#define __NRF24l01_H__	


/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f10x.h"
#include "JIANLAI_.h"
#define SP2_ENABLE 1
#define SP3_ENABLE 0
/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/
//NRF24L01�Ĵ�����������
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  		    0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		    0x20  //TX��������ж�
#define RX_OK   		    0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
#if SP3_ENABLE                              
#define NRF24L01_SPIx                        SPI3
#define NRF24L01_SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define NRF24L01_SPI_CLK                     RCC_APB1Periph_SPI3
#define NRF24L01_SPI_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define NRF24L01_SPI_GPIO_CLK                RCC_APB2Periph_GPIOB  

#define NRF24L01_SPI_SCK_PORT                GPIOB  
#define NRF24L01_SPI_SCK_PIN                 GPIO_Pin_3
   
#define NRF24L01_SPI_MISO_PORT               GPIOB
#define NRF24L01_SPI_MISO_PIN                GPIO_Pin_4

#define NRF24L01_SPI_MOSI_PORT               GPIOB 
#define NRF24L01_SPI_MOSI_PIN                GPIO_Pin_5

#define NRF24L01_SPI_CS_CLK                  RCC_APB2Periph_GPIOF    
#define NRF24L01_SPI_CS_PORT                 GPIOF
#define NRF24L01_SPI_CS_PIN                  GPIO_Pin_11
#define NRF24L01_SPI_CS_ENABLE()             GPIO_ResetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)
#define NRF24L01_SPI_CS_DISABLE()            GPIO_SetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)

#define NRF24L01_CE_CLK                      RCC_APB2Periph_GPIOF    
#define NRF24L01_CE_PORT                     GPIOF
#define NRF24L01_CE_PIN                      GPIO_Pin_9
#define NRF24L01_CE_LOW()                    GPIO_ResetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CE_HIGH()                   GPIO_SetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)

#define NRF24L01_IRQ_CLK                     RCC_APB2Periph_GPIOF    
#define NRF24L01_IRQ_PORT                    GPIOF
#define NRF24L01_IRQ_PIN                     GPIO_Pin_8
#define NRF24L01_IRQ_PIN_READ()              GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)
#elif SP2_ENABLE                              
#define NRF24L01_SPIx                        SPI2
#define NRF24L01_SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define NRF24L01_SPI_CLK                     RCC_APB1Periph_SPI2
#define NRF24L01_SPI_GPIO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define NRF24L01_SPI_GPIO_CLK                RCC_APB2Periph_GPIOB  
#define NRF24L01_SPI_AFIO_CLK				 RCC_APB2Periph_AFIO

#define NRF24L01_SPI_SCK_PORT                GPIOB  
#define NRF24L01_SPI_SCK_PIN                 GPIO_Pin_13
   
#define NRF24L01_SPI_MISO_PORT               GPIOB
#define NRF24L01_SPI_MISO_PIN                GPIO_Pin_14

#define NRF24L01_SPI_MOSI_PORT               GPIOB 
#define NRF24L01_SPI_MOSI_PIN                GPIO_Pin_15

#define NRF24L01_SPI_CS_CLK                  RCC_APB2Periph_GPIOG    
#define NRF24L01_SPI_CS_PORT                 GPIOG
#define NRF24L01_SPI_CS_PIN                  GPIO_Pin_7
#define NRF24L01_SPI_CS_ENABLE()             GPIO_ResetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)
#define NRF24L01_SPI_CS_DISABLE()            GPIO_SetBits(NRF24L01_SPI_CS_PORT, NRF24L01_SPI_CS_PIN)
#define W25Q64_SPI_2_CS()					 GPIO_SetBits(GPIOB,GPIO_Pin_12)//
#define SD_SPI_2_CS()						 GPIO_SetBits(GPIOD,GPIO_Pin_2)
#define NRF24L01_CE_CLK                      RCC_APB2Periph_GPIOF    
#define NRF24L01_CE_PORT                     GPIOG
#define NRF24L01_CE_PIN                      GPIO_Pin_6
#define NRF24L01_CE_LOW()                    GPIO_ResetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define NRF24L01_CE_HIGH()                   GPIO_SetBits(NRF24L01_CE_PORT, NRF24L01_CE_PIN)

#define NRF24L01_IRQ_CLK                     RCC_APB2Periph_GPIOG    
#define NRF24L01_IRQ_PORT                    GPIOG
#define NRF24L01_IRQ_PIN                     GPIO_Pin_8
#define NRF24L01_IRQ_PIN_READ()              GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8)
#endif


//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
									   	   
/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
void NRF24L01_Init(void);						//��ʼ��
void NRF24L01_RX_Mode(void);					//����Ϊ����ģʽ
void NRF24L01_TX_Mode(void);					//����Ϊ����ģʽ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);//д������
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);	//��������		  
uint8_t NRF24L01_Read_Reg(uint8_t reg);					//���Ĵ���
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);		//д�Ĵ���
uint8_t NRF24L01_Check(void);						//���24L01�Ƿ����
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);				//����һ����������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);				//����һ����������
#endif  /* __NRF24l01_H__ */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
