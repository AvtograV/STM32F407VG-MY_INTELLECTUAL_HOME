#include "tim.h"


/*************************** настройка TIM4 на выдачу ШИМ (не используется)**********************************/
void TIM4_Init (void){
	
	// Включить тактирование порта GPIOD
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	// Включить тактирование модуля TIM4
    RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	
	// 10: режим: альтернативная функция
    GPIOD -> MODER |= GPIO_MODER_MODER12_1;									// выход 12 порта D (PD12)
		GPIOD -> MODER |= GPIO_MODER_MODER13_1;									// выход 13 порта D (PD13)
		GPIOD -> MODER |= GPIO_MODER_MODER14_1;									// выход 14 порта D (PD14)
		GPIOD -> MODER |= GPIO_MODER_MODER15_1;									// выход 15 порта D (PD15)
	
	// PD12 - PD15 альтернативная функция - таймер (TIM - 0010: AF2)
		GPIOD -> AFR[1] |= GPIO_AFRH_AFSEL12_1;
		GPIOD -> AFR[1] |= GPIO_AFRH_AFSEL13_1;
		GPIOD -> AFR[1] |= GPIO_AFRH_AFSEL14_1;
		GPIOD -> AFR[1] |= GPIO_AFRH_AFSEL15_1;

																							/* TIM4 - ШИМ генератор	*/
// 1 канал TIM4 - ШИМ, прямая полярность
	TIM4 -> CCMR1 |= TIM_CCMR1_OC1M;							// 110 – режим ШИМа №1 (прямая полярность PWM).
	TIM4 -> CCMR1 &= ~TIM_CCMR1_OC1M_0;						// В режиме №1 - единица соответствует положительному уровню на выходе,
// 2 канал TIM4 - ШИМ, прямая полярность				// в режиме №2 - нулевому уровню.
	TIM4 -> CCMR1 |= TIM_CCMR1_OC2M;							// т.е. в 1-м режиме пока значение таймера < значения CCR, на выходе будет 1.	
	TIM4 -> CCMR1 &= ~TIM_CCMR1_OC2M_0;						// Во 2-м режиме наоборот.
// 3 канал TIM4 - ШИМ, прямая полярность			
	TIM4 -> CCMR2 |= TIM_CCMR2_OC3M;					
	TIM4 -> CCMR2 &= ~TIM_CCMR2_OC3M_0;
// 4 канал TIM4 - ШИМ, прямая полярность
	TIM4 -> CCMR2 |= TIM_CCMR2_OC4M;
	TIM4 -> CCMR2 &= ~TIM_CCMR2_OC4M_0;
	
	
	TIM4 -> CR1 &= ~TIM_CR1_DIR;									// направление счёта (здесь прямой счёт от 0 до max 65535)
																								// В таймерах, где можно выбрать направление счёта
																								// (даже во время счёта), по умолчанию, после сброса - счёт вверх,
																								// для счёта вниз надо установить бит DIR в регистре TIMx -> CR1.
																								// TIMx -> CR1 |= TIM_CR1_DIR
	
  TIM4 -> EGR = TIM_EGR_UG;											// передадим сигнал о сбросе счётчика, чтоб все 
																								// внутренние регистры точно обновились																													
		
  TIM4 -> CCER |= TIM_CCER_CC1E;								// включить выход 1 канала TIM4		
	TIM4 -> CCER |= TIM_CCER_CC2E;								// включить выход 2 канала TIM4		
	TIM4 -> CCER |= TIM_CCER_CC3E;								// включить выход 3 канала TIM4		
	TIM4 -> CCER |= TIM_CCER_CC4E;								// включить выход 4 канала TIM4 
																													
	
	TIM4 -> PSC = 1000;														// предделитель - делит частоту системной шины (AHB1ENR)
																								// (здесь частота AHB1ENR == 42 МГц) (частота счёта - 42 КГц)
																						
	TIM4 -> ARR = 2000;														// Период счёта - значение регистра автоперезагрузки
																								// (число, до которого будем считать таймер и обнуляться).
																						
	TIM4 -> CCR1 = 1;															// регистр захвата / сравнения - начальный коэффициент заполнения.
	TIM4 -> CCR2 = 1;															// (таймер считает до этого числа и инвертируется)
	TIM4 -> CCR3 = 1;															// и получается пульсацмя (ШИМ).
	TIM4 -> CCR4 = 1;

	TIM4 -> CR1 |= TIM_CR1_CEN;										// запустить генерацию - counter enable   
}




/************************** TIM6 - отсчёт времения для сетевого диммера 220 V *******************************/
void TIM6_Init (void){																		// GPIO - PC4
	 
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;									// включить тактирование порта GPIOC		
  RCC -> APB1ENR |= RCC_APB1ENR_TIM6EN;										// включить тактирование модуля TIM6		
																													
  GPIOC -> MODER 	|= GPIO_MODER_MODER4_0;									// 01 - out - GPIO PC4
	
	TIM6 -> PSC = 2100 - 1;																	// (частота AHB1ENR == 21 МГц) (частота счёта - 10 КГц)																						
	TIM6 -> ARR = 75;                                 			// настройка прерываний (есть эффект от 50 до 75)
	
	TIM6 -> CR1 |= TIM_CR1_ARPE;														// запись без регистра предварительной загрузки	
	TIM6 -> CR1 |= TIM_CR1_OPM;															// one-pulse mode	
	TIM6 -> CR1 |= TIM_CR1_URS;															// генерирует прерывание только счетчик переполнения	
	TIM6 -> CR1 &= ~TIM_CR1_UDIS;														// генерировать событие UEV	
	
	TIM6 -> EGR |= TIM_EGR_UG;															// обновление регистров (update event)
	
	TIM6 -> DIER |= TIM_DIER_UIE; 													// разрешить прерывания от таймера	
//	TIM6 -> CR1 |= TIM_CR1_CEN;														// запустить таймер - counter enable	
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 													// разрешение TIM6_DAC_IRQn прерываний	
}



/************************** TIM7 - системный таймер *******************************/
void TIM7_Init (void){																		// GPIO - PC4
	 
  RCC -> APB1ENR |= RCC_APB1ENR_TIM7EN;										// включить тактирование модуля TIM7
	
	TIM7 -> PSC = 21000 - 1;																// настроить делитель для формирования миллисекунд																						
	TIM7 -> ARR = 0;
	
	TIM7 -> CR1 |= TIM_CR1_ARPE;														// запись без регистра предварительной загрузки	
	TIM7 -> CR1 |= TIM_CR1_OPM;															// one-pulse mode	
	TIM7 -> CR1 |= TIM_CR1_URS;															// генерирует прерывание только счетчик переполнения	
	TIM7 -> CR1 &= ~TIM_CR1_UDIS;														// генерировать событие UEV	
	
	TIM7 -> EGR |= TIM_EGR_UG;															// обновление регистров (update event)
	
	TIM7 -> DIER |= TIM_DIER_UIE; 												// разрешить прерывания от таймера	
//	TIM7 -> CR1 |= TIM_CR1_CEN;														// запустить таймер - counter enable	
	NVIC_EnableIRQ(TIM6_DAC_IRQn); 												// разрешение TIM6_DAC_IRQn прерываний	
}



/************************ обработка прерываний от TIM6 для для сетевого диммера 220 V ***********************/
void TIM6_DAC_IRQHandler(void) {
	
    TIM6 -> SR &= ~TIM_SR_UIF; 											  		// cбрасываем флаг прерывания	
	GPIOC -> BSRR |= GPIO_BSRR_BS4;													// на GPIO PC4 - 5 v
}



/********************	функция задержка времени на TIM7	***********************/
void delay_ms(uint16_t value)	{									// argument  длительность задержки от 1 до 65535 миллисекунд

  TIM7 -> ARR = value;                  				// загрузить значение задержки
  TIM7 -> CNT = 0;
  TIM7 -> CR1 |= TIM_CR1_CEN;	      						// запустить таймер
	
  while((TIM7 -> SR & TIM_SR_UIF) == 0){} 			// дождаться конца задержки
			TIM7 -> SR &= ~TIM_SR_UIF;	      				// сбросить флаг
}
