#ifndef __APP_ETHERNET_H
#define __APP_ETHERNET_H

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "lwip/netif.h"
#include "main.h"
   
/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct  
{
	uint8_t MAC[6];      //MAC��ַ
	uint8_t IP[4];       //����IP��ַ
	uint8_t NetMask[4]; 	//��������
	uint8_t GateWay[4]; 	//Ĭ�����ص�IP��ַ
}Lwip_strut;

/* �궨�� --------------------------------------------------------------------*/
#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5
   
/* ��չ���� ------------------------------------------------------------------*/
extern Lwip_strut Lwip_data;

/* �������� ------------------------------------------------------------------*/

void User_notification(struct netif *netif);
#ifdef USE_DHCP
void DHCP_Process(struct netif *netif);
void DHCP_Periodic_Handle(struct netif *netif);
#endif  


#endif /* __APP_ETHERNET_H */


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

