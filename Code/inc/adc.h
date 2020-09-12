#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "vTask_Create.h"

void init_adc1 (void);

uint16_t adc_resalt_send_uart3 (void);

#endif 			// ADC_H
