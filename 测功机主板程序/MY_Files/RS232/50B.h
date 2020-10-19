#ifndef _50B_H
#define _50B_H

#include "main.h"

typedef struct _MQ50B_test_data{
	double CO2;
	double CO;
	uint16_t HC;
	uint16_t NO;
	double O2;
}MQ50B_test_data;

extern MQ50B_test_data MQ50B;

void Tx_50B(void);
void Rx_50B(void);

#endif

