#ifndef _TIM_H
#define _TIM_H 

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"


/********************** настройка TIM4 на выдачу PWM *****************************/
void TIM4_Init (void);


/************* TIM6 - отсчёт времения для сетевого диммера 220 V ******************/
void TIM6_Init (void);


/************************** TIM7 - системный таймер *******************************/
void TIM7_Init (void);


/**********************	функция задержка времени на TIM7	*************************/
void delay_ms(uint16_t value);									// argument  длительность задержки
																								// от 1 до 65535 миллисекунд

#endif 			// TIM_H