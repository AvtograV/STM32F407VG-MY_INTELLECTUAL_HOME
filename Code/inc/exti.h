#ifndef _EXTI_H
#define _EXTI_H 

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "vTask_Create.h"

void EXTI_0_init (void);
void EXTI_4_init (void);

#endif 			// EXTI_H
