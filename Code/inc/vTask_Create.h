#ifndef _VTASK_CREATE_H
#define _VTASK_CREATE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "dma.h"
#include "adc.h"
#include "motor.h"
#include "one_wire.h"
#include "nextion.h"



/**************** отправить результат измерения ADC1 через UART2 + FreeRTOS ***************/
void vTack_ADC1_send_USART3 (void *argument);

/****************************** задача - подключение 1-wire ******************************/
void vTack_one_wire_conect (void *argument);

void vTack_USART_send (void *argument);


#endif 			// VTASK_CREATE
