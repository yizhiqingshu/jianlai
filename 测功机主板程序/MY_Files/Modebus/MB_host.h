#ifndef MB_HOST_H__
#define MB_HOST_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "main.h"
/* ���Ͷ��� ------------------------------------------------------------------*/

/* �궨�� --------------------------------------------------------------------*/
#define MB_SLAVEADDR        0x0001
#define MB_REG_ADDR         0x0000//�Ĵ�����ַ
/* ��չ���� ------------------------------------------------------------------*/

/* �������� ------------------------------------------------------------------*/
uint16_t MB_CRC16(uint8_t *pushMsg,uint8_t usDataLen);
void MB_ReadCoil_01H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteCoil_05H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInput_02H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadHoldingReg_03H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_ReadInputReg_04H(uint8_t _addr, uint16_t _reg, uint16_t _num);
void MB_WriteHoldingReg_06H(uint8_t _addr, uint16_t _reg, uint16_t _data);
void MB_WriteNumHoldingReg_10H(uint8_t _addr, uint16_t _reg, uint16_t _num,uint8_t *_databuf);
#endif /* __BSP_MB_HOST_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
