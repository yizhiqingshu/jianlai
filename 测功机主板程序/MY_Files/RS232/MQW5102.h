#ifndef _MQW5102_H
#define _MQW5102_H

#include "main.h"


typedef struct _MQW5102_test_data{
	double qiluya;
	double youwen;
	uint16_t zhuansu;
	uint16_t NOx;
	double CO2;
	double N;
	double Ns;
	double K;
	uint16_t yanqi_T;
}MQW5102_test_data;

extern MQW5102_test_data MQW5102;

void Tx_MQW5102(void);
void Rx_MQW5102(void);
#endif

