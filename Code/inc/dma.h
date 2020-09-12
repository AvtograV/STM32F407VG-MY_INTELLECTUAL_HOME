#ifndef _DMA_H
#define _DMA_H

#include "stm32f4xx.h"

void USART2_TX_DMA_Init(void);
void USART2_RX_DMA_Init(void);

void exchange_mem_DMA_USART (void);


#endif 			// DMA_H
