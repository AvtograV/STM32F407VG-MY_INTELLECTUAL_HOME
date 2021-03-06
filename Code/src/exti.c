#include "exti.h"
#include "light.h"
#include "tim.h"

/**************************************************************************************************/
	void EXTI_0_init (void){
		
		// Настраиваем NVIC - контроллер вложенных векторных прерываний
		// Прерывание должно быть разрешено контролером прерываний NVIC
	NVIC_SetPriority(EXTI0_IRQn, 0); 								// делаем прерывания высокоприоритетными.
  NVIC_EnableIRQ(EXTI0_IRQn);											// разрешаем обработку используемых внешних прерываний
		
	
		// включаем тактовые сигналы используемых периферийных устройств.  
		// EXTI не требует включения тактового сигнала 	
		// включить тактирование порта GPIOA
		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// канал от порта ввода-вывода должен быть подключен к контроллеру EXTI (через мультиплексор)
	// EXTI0 configuration
		SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;				// PA[0] pin
												// SYSCFG_EXTICR1_EXTI1_PA           PA[1] pin и т. д.
												// EXTICR[(x 0...5)] - SYSCFG external interrupt configuration registers
		
		
		
	// Выполняем конфигурирование портов ввода-вывода
	// Порт ввода-вывода должен быть настроен на вход
	// Button: PA0: float in (MODER - 00: Input (reset state), PUPDR - 00: No pull-up, pull-down)
		GPIOA -> MODER &= ~GPIO_MODER_MODE0;
   
	// Разрешаем генерацию прерываний для линии EXTI0
    EXTI -> IMR |= EXTI_IMR_MR0;

  // Пользователь должен определить работу детектора фронта: по фронту или по срезу;	
		EXTI -> RTSR |= EXTI_RTSR_TR0;					// Настраиваем линию 0 на обнаружение нарастающего фронта
	//	EXTI -> FTSR |= EXTI_FTSR_TR0;					// Настраиваем линию 0 на обнаружение спадающего фронта
 }

 

 /************************ обработчик внешнего прерывания от линии EXTI0 **************************/																
	void EXTI0_IRQHandler()	{ 
																							// После окончания обработки прерывания, необходимо сбросить
		EXTI -> PR |= EXTI_PR_PR0;								// активность события, вызвавшего прерывание — «очистить прерывание»								 							
																							// cбрасываем нулевой бит регистра EXTI -> PR записывая в бит 1
		

			light_rising_trigger ();								// выполняем какие либо действия
	}

	
	
/**************************************************************************************************/
	void EXTI_4_init (void)	{																// IO - PA4
	
		NVIC_SetPriority(EXTI4_IRQn, 1); 											// делаем прерывания высокоприоритетными.
		NVIC_EnableIRQ(EXTI4_IRQn);														// разрешаем обработку используемых внешних прерываний

		RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;	
		SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR2_EXTI4_PA;				// подключение GPIO к контролеру EXTI - PA4
	
		EXTI -> IMR |= EXTI_IMR_MR4;													// разрешаем генерацию прерываний для линии EXTI4		
		EXTI -> RTSR |= EXTI_RTSR_TR4;												// обнаружение нарастающего фронта	
}	


/************************* обработчик внешнего прерывания от линии EXTI4 ***************************/																	
	void EXTI4_IRQHandler()	{ 
																													// после окончания обработки прерывания, необходимо сбросить
		EXTI -> PR |= EXTI_PR_PR4;														// активность события, вызвавшего прерывание — «очистить прерывание»
		
		GPIOC	-> BSRR |= GPIO_BSRR_BR4;												// отключить с GPIO PC4 - 5 v
		
		TIM6 -> CR1 |= TIM_CR1_CEN;														// каждый раз заново запускаем TIM6
 }
