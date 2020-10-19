#ifndef __TCP_ECHOCLIENT_H__
#define __TCP_ECHOCLIENT_H__
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "memp.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"

/* 类型定义 ------------------------------------------------------------------*/
/* ECHO protocol states */
/* ECHO protocol states */
enum echoclient_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};

/* structure to be passed as argument to the tcp callbacks */
struct echoclient
{
  enum echoclient_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};


/* 宏定义 --------------------------------------------------------------------*/
#define Server_IP_ADDR0   (uint8_t) 192
#define Server_IP_ADDR1   (uint8_t) 168
#define Server_IP_ADDR2   (uint8_t) 3
#define Server_IP_ADDR3   (uint8_t) 191

#define TCP_RX_BUFSIZE	  1500	//定义tcp client最大接收数据长度
#define TCP_CLIENT_PORT		1234	 //定义tcp server的端口

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void YSH7_TCP_SENDData(uint8_t a);


#endif /* __TCP_ECHOCLIENT_H__ */


/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/

