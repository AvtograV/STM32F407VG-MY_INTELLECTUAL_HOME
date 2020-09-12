#include "rcc_Init.h"

void RCC_Init (void) {
	RCC -> CR |= ((uint32_t)RCC_CR_HSEON); 											// enable HSE
		while( !(RCC->CR & RCC_CR_HSERDY) ) {}; 									// ready to start HSE	
			
	// Настраиваем Flash prefetch, instruction cache, data cache и wait state
	FLASH -> ACR |= FLASH_ACR_ICEN;															// Instruction cache enable
	FLASH -> ACR |= FLASH_ACR_DCEN;															// Data cache enable
	FLASH -> ACR |= FLASH_ACR_LATENCY_5WS;
	FLASH -> ACR |= FLASH_ACR_PRFTEN;
		
			// определяем делители шин
	RCC -> CFGR &= ~(RCC_CFGR_HPRE);
	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;													// AHB prescaler - not divided (не используется, деление на 1)
	RCC -> CFGR &= ~(RCC_CFGR_PPRE1);
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV4;													// APB Low speed prescaler (APB1) (делитель - 4)
	RCC -> CFGR &= ~(RCC_CFGR_PPRE2);
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV2;													// APB high-speed prescaler (APB2) (делитель - 2)	
	
			// PLL - HSE
	RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;										// set HSE as PLL source - тактирование PLL от HSE
	RCC -> CR &= ~(RCC_CR_PLLON); 															// disable PLL before changes			
			// PLL M
	RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLM); 											// clear all PLLM bits
	RCC -> PLLCFGR |= RCC_PLLCFGR_PLLM_2; 											// set PLLM = 4 (100)			
			// PLL P
	RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLP); 											// main PLL division PLLP = 2 (00)			
			// PLL N
	RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLN); 											// clear all PLLN bits with mask
	RCC -> PLLCFGR |= (RCC_PLLCFGR_PLLN_3 											// set PLLN = 168 (1010 1000)
	| RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_7);									// for 8 or 16 MHz HSE
			// PLL Q 7 (0111)
	RCC -> PLLCFGR &= ~(RCC_PLLCFGR_PLLQ);
	RCC -> PLLCFGR |= (RCC_PLLCFGR_PLLQ_0	| RCC_PLLCFGR_PLLQ_2 | RCC_PLLCFGR_PLLQ_2);
		
			// включаем PLL и дожидаемся готовности PLL
	RCC -> CR |= RCC_CR_PLLON;
		while (!(RCC -> CR & RCC_CR_PLLRDY)){};			
			
			// переключаем системное тактирование на PLL
	RCC -> CFGR &= ~RCC_CFGR_SW;
  RCC -> CFGR |= RCC_CFGR_SW_PLL;			
			// дожидаемся переключения системного тактированиея на PLL
	while (!(RCC -> CFGR & RCC_CFGR_SWS ) ){};
}

/************* для контроля выводим SYSCLK на MCO2 (PC9), частота на MCO2 =  SYSCLK/5 *************/
void mco2_Init(void) {
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC -> MODER &= ~GPIO_MODER_MODE9;
	GPIOC -> MODER |= GPIO_MODER_MODE9_1;
	GPIOC -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	RCC -> CFGR |= RCC_CFGR_MCO2PRE;
}
