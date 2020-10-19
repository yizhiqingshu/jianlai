/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: LwIPЭ���ʼ��
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
#include "stm32f4xx_hal.h"
#include "lwip/dhcp.h"
#include "app_ethernet.h"
#include "usart.h"

extern uint8_t TxBuf1[256];
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
#ifdef USE_DHCP
#define MAX_DHCP_TRIES  4
uint32_t DHCPfineTimer = 0;
__IO uint8_t DHCP_state = DHCP_OFF;
__IO uint8_t DHCP_flag=0;
Lwip_strut Lwip_data;
#endif

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: TCP����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void User_notification(struct netif *netif) 
{
  if (netif_is_up(netif))
 {
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#else
    uint8_t iptxt[20];
    sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
    printf ("Static IP address: %s\n", iptxt);

#endif /* USE_DHCP */
 }
 else
  {  
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif  /* USE_DHCP */
   //printf ("The network cable is not connected \n");
	 sprintf((char*)&TxBuf1,"The network cable is not connected \n");    
	 HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
  } 
}

/**
  * ��������: ������״̬����֪ͨ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
#ifndef USE_DHCP
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
#endif
  
  if(netif_is_link_up(netif))
  {
		sprintf((char*)&TxBuf1,"The network cable is now connected \n");    
	  HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
    //printf ("The network cable is now connected \n");
    
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#else
    IP_ADDR4(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);  
    
    netif_set_addr(netif, &ipaddr , &netmask, &gw);  
     
    uint8_t iptxt[20];
    sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
    printf ("Static IP address: %s\n", iptxt);
#endif /* USE_DHCP */   
    
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);     
  }
  else
  {
#ifdef USE_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif /* USE_DHCP */
    
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
    sprintf((char*)&TxBuf1,"The network cable is not connected \n");    
	  HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
    //printf ("The network cable is not connected \n");
    
  }
}

#ifdef USE_DHCP
/**
  * ��������: DHCP��ȡ����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void DHCP_Process(struct netif *netif)
{
  uint32_t ipaddr=0,netmask=0,gw=0;
  struct dhcp *dhcp;   

  uint8_t iptxt[20];
  
  switch (DHCP_state)
  {
    case DHCP_START:
    {
      DHCP_state = DHCP_WAIT_ADDRESS;
      dhcp_start(netif);
//			sprintf((char*)&TxBuf1,"The network cable is not connected \n");    
//			HAL_UART_Transmit(&husart1,TxBuf1,strlen((char *)TxBuf1),1000);
      printf ("  State: Looking for DHCP server ...\n");
    }
    break;
    
  case DHCP_WAIT_ADDRESS:
    {
      printf ("DHCP_state=%d\n",DHCP_state);
      if (dhcp_supplied_address(netif)) 
      {
        DHCP_state = DHCP_ADDRESS_ASSIGNED; 
        /* ��ȡIP��ַ */
        ipaddr=netif->ip_addr.addr;
        
        Lwip_data.MAC[3]=MAC_ADDR3;
        Lwip_data.MAC[2]=MAC_ADDR2;
        Lwip_data.MAC[1]=MAC_ADDR1;
        Lwip_data.MAC[0]=MAC_ADDR0;
        printf (" MAC  ��ַ:         %d %d %d %d\n", Lwip_data.MAC[0],Lwip_data.MAC[1],Lwip_data.MAC[2],Lwip_data.MAC[3]);  
               
        Lwip_data.IP[3]=(uint8_t)(ipaddr>>24);
        Lwip_data.IP[2]=(uint8_t)(ipaddr>>16);
        Lwip_data.IP[1]=(uint8_t)(ipaddr>>8);
        Lwip_data.IP[0]=(uint8_t)(ipaddr);
        printf (" DHCP GET IP:      %d %d %d %d\n", Lwip_data.IP[0],Lwip_data.IP[1],Lwip_data.IP[2],Lwip_data.IP[3]);  
        /* ��ȡ���������ַ */
        netmask=netif->netmask.addr;
        Lwip_data.NetMask[3]=(uint8_t)(netmask>>24);
        Lwip_data.NetMask[2]=(uint8_t)(netmask>>16);
        Lwip_data.NetMask[1]=(uint8_t)(netmask>>8);
        Lwip_data.NetMask[0]=(uint8_t)(netmask);
        printf (" DHCP GET ��������: %d %d %d %d\n", Lwip_data.NetMask[0],Lwip_data.NetMask[1],Lwip_data.NetMask[2],Lwip_data.NetMask[3]);  
        /* ��ȡĬ�����ص�ַ */
        gw=netif->gw.addr;
        Lwip_data.GateWay[3]=(uint8_t)(gw>>24);
        Lwip_data.GateWay[2]=(uint8_t)(gw>>16);
        Lwip_data.GateWay[1]=(uint8_t)(gw>>8);
        Lwip_data.GateWay[0]=(uint8_t)(gw);
        printf (" DHCP GET Ĭ������: %d %d %d %d\n", Lwip_data.GateWay[0],Lwip_data.GateWay[1],Lwip_data.GateWay[2],Lwip_data.GateWay[3]);  
        
        
//        sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));        
//        printf (" IP address assigned by a DHCP server: %s\n", iptxt);        
				printf (" ���Խ�������TCP����������������...\n"); 		//��YSH7_TCP_SENDData()����
      }
      else
      {
        dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    
        /* DHCP timeout */
        if (dhcp->tries > MAX_DHCP_TRIES)
        {
          DHCP_state = DHCP_TIMEOUT;
          
          /* Stop DHCP */
          dhcp_stop(netif);
          
          /* Static address used */
          IP_ADDR4(&(netif->ip_addr), IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
          IP_ADDR4(&(netif->netmask), NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
          IP_ADDR4(&(netif->gw), GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
      
          sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
          printf ("DHCP Timeout !! \n");
          printf ("Static IP address: %s\n", iptxt);   
        }
      }
    }
    break;
  case DHCP_LINK_DOWN:
    {
      /* Stop DHCP */
      dhcp_stop(netif);
      DHCP_state = DHCP_OFF; 
    }
    break;
  default: break;
  }
}

/**
  * ��������: DHCP��ѯ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void DHCP_Periodic_Handle(struct netif *netif)
{  
  /* Fine DHCP periodic process every 500ms */
  if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  HAL_GetTick();
    /* process DHCP state machine */
    DHCP_Process(netif);    
  }
}
#endif

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
