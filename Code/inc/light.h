#ifndef _LIGHT_H
#define _LIGHT_H 

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "vTask_Create.h"
#include "tim.h"

/**********	функция плавного увеличения яркости свечения	*************/
		void light_rising_trigger (void);

#endif 			// LIGHT_H
