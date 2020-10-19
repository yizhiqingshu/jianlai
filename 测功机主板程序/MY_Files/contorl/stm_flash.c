/**
  ******************************************************************************
  * �ļ�����: stm_flash.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: �ڲ�Falsh��дʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm_flash.h"
#include "contorl.h"
#include "gpio.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/*׼��д��Ĳ�������*/


/* Ҫ�����ڲ�FLASH����ʼ��ַ */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_8   
/* Ҫ�����ڲ�FLASH�Ľ�����ַ */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_11  +  GetSectorSize(ADDR_FLASH_SECTOR_11) -1 

/* ˽�б��� ------------------------------------------------------------------*/
uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
uint32_t SectorError = 0;

static FLASH_EraseInitTypeDef EraseInitStruct;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
static uint32_t GetSector(uint32_t Address);
static uint32_t GetSectorSize(uint32_t Sector);

/* ������ --------------------------------------------------------------------*/

void stm_flash_Tx(void)
{
	LED2_ON;
	double Tx_buff[12];
	uint8_t i;
	HAL_FLASH_Unlock();
	Tx_buff[0]=close_T1.Kp*1000;Tx_buff[1]=close_T1.Ki*1000;Tx_buff[2]=close_T1.Kd*1000;
	Tx_buff[3]=close_T2.Kp*1000;Tx_buff[4]=close_T2.Ki*1000;Tx_buff[5]=close_T2.Kd*1000;
	Tx_buff[6]=close_T3.Kp*1000;Tx_buff[7]=close_T3.Ki*1000;Tx_buff[8]=close_T3.Kd*1000;
	Tx_buff[9]=close_S.Kp*1000;Tx_buff[10]=close_S.Ki*1000;Tx_buff[11]=close_S.Kd*1000;
	
  /* ����,ɾ����д������Ƚ��� */
  HAL_FLASH_Unlock();
  
  /* ��ȡҪ�������׸����� */
  FirstSector = GetSector(FLASH_USER_START_ADDR);
  
  /* ��ȡҪ������������Ŀ */
  NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1; 
  
  /* ��ʼ�������ṹ�� */
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
	
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /* �˴�����ʹ��HAL_FLASH_GetError()����ȡ��������������� */
    while (1)
    {
      
    }
  }
  Address = FLASH_USER_START_ADDR;
	for(i=0;i<12;i++)
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Tx_buff[i]) == HAL_OK)
		{
			Address = Address + 4;
		}
	}
	LED2_OFF;
	printf("�����������\n");
}

void stm_flash_Rx(void)
{
	uint8_t i=0;
	double Rx_buff[12];
	HAL_FLASH_Lock(); 
  Address = FLASH_USER_START_ADDR;

	for(i=0;i<12;i++)
	{
		Rx_buff[i] = *(__IO uint32_t*)Address;
    Address = Address + 4;
	} 
	close_T1.Kp=Rx_buff[0]/1000;close_T1.Ki=Rx_buff[1]/1000;close_T1.Kd=Rx_buff[2]/1000;
	close_T2.Kp=Rx_buff[3]/1000;close_T2.Ki=Rx_buff[4]/1000;close_T2.Kd=Rx_buff[5]/1000;
	close_T3.Kp=Rx_buff[6]/1000;close_T3.Ki=Rx_buff[7]/1000;close_T3.Kd=Rx_buff[8]/1000;
	close_S.Kp=Rx_buff[9]/1000;close_S.Ki=Rx_buff[10]/1000;close_S.Kd=Rx_buff[11]/1000;
}

/**
  * ��������: ��������ĵ�ַ���������ڵ�sector
  * �������: Address flash��ַ
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;  
  }

  return sector;
}

/**
  * ��������: ����������Ż�ȡ������С
  * �������: Sector 
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
