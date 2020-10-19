/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: TCP_Client����ʵ��
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
#include "tcp_echoclient.h"

#if LWIP_TCP
/* ˽�����Ͷ��� --------------------------------------------------------------*/

struct echoclient * echoclient_es;
struct tcp_pcb *echoclient_pcb;

/* ˽�к궨�� ----------------------------------------------------------------*/
 

/* ˽�б��� ------------------------------------------------------------------*/
u8_t  recev_buf[TCP_RX_BUFSIZE];
__IO uint32_t message_count=0;
char tcp_SendData[100];
__IO uint8_t rec_flag=0;
uint8_t tcp_flag;


/* ��չ���� ------------------------------------------------------------------*/

extern struct netif gnetif;

/* ˽�к���ԭ�� --------------------------------------------------------------*/
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es);
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
err_t tcp_client_usersent(struct tcp_pcb *tpcb);

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: TCP�����Լ����Ͳ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void YSH7_TCP_SENDData(uint8_t a)
{
  ip_addr_t CLT_ipaddr;  	//Զ��ip��ַ
  uint8_t result=0,i=0;
  i=a;
  /* ����һ���µ�pcb */
	echoclient_pcb=tcp_new();	
	if(echoclient_pcb)			/* �����ɹ� */
	{ 
    printf(" ������TCP�ɹ�\n");  
    /* ������IP��ָ���Ķ˿ںŰ���һ��,IP_ADDR_ANYΪ�󶨱������е�IP��ַ */
		IP4_ADDR(&CLT_ipaddr,Server_IP_ADDR0,Server_IP_ADDR1,Server_IP_ADDR2,Server_IP_ADDR3); 
		tcp_connect(echoclient_pcb,&CLT_ipaddr,TCP_CLIENT_PORT,tcp_echoclient_connected);  //���ӵ�Ŀ�ĵ�ַ��ָ���˿���,�����ӳɹ���ص�tcp_client_connected()����
	}
  else 
  result=1;  
  
//  while(result==0)  
//  {
    ethernetif_input(&gnetif);
		if(i==1)
		{					
			/* ��ֵ����tcp_SendData�� */   
			sprintf(tcp_SendData,"�ͻ��˵�ѹΪ��\n");			
			tcp_client_usersent(echoclient_pcb);   
			printf("���ͳɹ���");
		}
		else if(i==2)
		{
			
		}
//  }  
}

/**
  * ��������: ���ô˺�������������
  * �������: ��
  * �� �� ֵ: 0���ɹ���������ʧ��
  * ˵    ��: ��
  */
err_t tcp_client_usersent(struct tcp_pcb *tpcb)
{
  err_t ret_err;
	struct echoclient *es; 
	es=tpcb->callback_arg;
	if(es!=NULL)  /* ���Ӵ��ڿ��п��Է������� */ 
	{
    es->p_tx=pbuf_alloc(PBUF_TRANSPORT, strlen((char*)tcp_SendData),PBUF_POOL);	/* �����ڴ� */ 

    pbuf_take(es->p_tx,(char*)tcp_SendData,strlen((char*)tcp_SendData));	/* ��tcp_client_sentbuf[]�е����ݿ�����es->p�� */
    tcp_echoclient_send(tpcb,es);/* ��tcp_client_sentbuf[]���渴�Ƹ�pbuf�����ݷ��ͳ�ȥ */
    if(es->p_tx)
    {         
      pbuf_free(es->p_tx);	/* �ͷ��ڴ� */
    }

		ret_err=ERR_OK;
	}
  else
	{ 
		tcp_abort(tpcb);/* ��ֹ����,ɾ��pcb���ƿ� */
		ret_err=ERR_ABRT;
	}
	return ret_err;
}


/**
  * ��������: TCP���ӽ���ʱ���õĺ��� 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static err_t tcp_echoclient_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  struct echoclient *es = NULL;
  
  if (err == ERR_OK)   
  {
    /* allocate structure es to maintain tcp connection informations */
    es = (struct echoclient *)mem_malloc(sizeof(struct echoclient));
   // echoclient_es=es;
    if (es != NULL)
    {
      es->state = ES_CONNECTED;
      es->pcb = tpcb;
      /* allocate pbuf */
      es->p_tx = pbuf_alloc(PBUF_TRANSPORT, strlen((char*)tcp_SendData) , PBUF_POOL);
         
      if (es->p_tx)
      {       

        /* pass newly allocated es structure as argument to tpcb */
        tcp_arg(tpcb, es);
  
        /* initialize LwIP tcp_recv callback function */ 
        tcp_recv(tpcb, tcp_echoclient_recv);
  
        /* initialize LwIP tcp_sent callback function */
        tcp_sent(tpcb, tcp_echoclient_sent);
  
        /* initialize LwIP tcp_poll callback function */
        tcp_poll(tpcb, tcp_echoclient_poll, 1);
           
        return ERR_OK;
      }
    }
    else
    {
      /* close connection */
      tcp_echoclient_connection_close(tpcb, es);
      
      /* return memory allocation error */
      return ERR_MEM;  
    }
  }
  else
  {
    /* close connection */
    tcp_echoclient_connection_close(tpcb, es);
  }
  return err;
}
    
/**
  * ��������: TCP�������ݻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static err_t tcp_echoclient_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{ 
  char *recdata=0;
  struct echoclient *es;
  err_t ret_err;
  

  LWIP_ASSERT("arg != NULL",arg != NULL);
  
  es = (struct echoclient *)arg;
  
  /* if we receive an empty tcp frame from server => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p_tx == NULL)
    {
       /* we're done sending, close connection */
       tcp_echoclient_connection_close(tpcb, es);
    }
    else
    {    
      /* send remaining data*/
      tcp_echoclient_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }   
  /* else : a non empty frame was received from echo server but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_CONNECTED)
  {
    /* increment message count */
    message_count++;
         
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);  
    
		recdata=(char *)malloc(p->len*sizeof(char));
		if(recdata!=NULL)
		{
			memcpy(recdata,p->payload,p->len);
			printf("upd_rec:%s",recdata);
		}
		free(recdata);    
    
    pbuf_free(p);
    //tcp_echoclient_connection_close(tpcb, es);
    ret_err = ERR_OK;
  }

  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    
    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * ��������: TCP�������ݺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void tcp_echoclient_send(struct tcp_pcb *tpcb, struct echoclient * es)
{
  struct pbuf *ptr;
  err_t wr_err = ERR_OK;
 
  while ((wr_err == ERR_OK) &&
         (es->p_tx != NULL) && 
         (es->p_tx->len <= tcp_sndbuf(tpcb)))
  {
    
    /* get pointer on pbuf from es structure */
    ptr = es->p_tx;

    /* enqueue data for transmission */
    wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);
    
    if (wr_err == ERR_OK)
    { 
      /* continue with next pbuf in chain (if any) */
      es->p_tx = ptr->next;
      
      if(es->p_tx != NULL)
      {
        /* increment reference count for es->p */
        pbuf_ref(es->p_tx);
      }
      tcp_output(tpcb);
      /* free pbuf: will free pbufs up to es->p (because es->p has a reference count > 0) */
      pbuf_free(ptr);
   }
   else if(wr_err == ERR_MEM)
   {
      /* we are low on memory, try later, defer to poll */
     es->p_tx = ptr;
   }
   else
   {
     /* other problem ?? */
   }
  }
}

/**
  * ��������: TCP_poll�ص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static err_t tcp_echoclient_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct echoclient *es;

  es = (struct echoclient*)arg;
  if (es != NULL)
  {
    if (es->p_tx != NULL)
    {
      /* there is a remaining pbuf (chain) , try to send data */
      tcp_echoclient_send(tpcb, es);
    }
    else
    {
      /* no remaining pbuf (chain)  */
      if(es->state == ES_CLOSING)
      {
        /* close tcp connection */
        tcp_echoclient_connection_close(tpcb, es);
      }
    }
    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * ��������: TCP���ͻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static err_t tcp_echoclient_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct echoclient *es;

  LWIP_UNUSED_ARG(len);

  es = (struct echoclient *)arg;
  
  if(es->p_tx != NULL)
  {
    /* still got pbufs to send */
    tcp_echoclient_send(tpcb, es);
  }

  return ERR_OK;
}

/**
  * ��������: �ر�TCP���Ӻ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void tcp_echoclient_connection_close(struct tcp_pcb *tpcb, struct echoclient * es )
{
  /* remove callbacks */
  tcp_recv(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_poll(tpcb, NULL,0);

  if (es != NULL)
  {
    mem_free(es);
  }

  /* close tcp connection */
  tcp_close(tpcb);
  
}

#endif /* LWIP_TCP */

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
