#ifndef _NEXTION_H
#define _NEXTION_H 

#include "stm32f4xx.h"
#include "usart.h"

void Init_io_led_for_nextion (void);

/********************** принять байт от Nextion по USART3 *********************/
void USART3_IRQHandler_Nextion (void);

#endif 			// NEXTION_H
