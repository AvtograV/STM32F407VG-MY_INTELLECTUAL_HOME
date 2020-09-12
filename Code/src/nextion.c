#include "nextion.h"

		/* синий TX --- жёлтый RX */
		
/********************** принять байт от Nextion по USART3 *********************/
void USART3_IRQHandler_Nextion (void) {
	
	USART3 -> CR1 &= ~USART_CR1_RXNEIE;
	
	if (USART3 -> DR == 0x02) {
		GPIOD -> BSRR |= GPIO_BSRR_BS12;
		GPIOD -> BSRR |= GPIO_BSRR_BS13;
		GPIOD -> BSRR |= GPIO_BSRR_BS14;
		GPIOD -> BSRR |= GPIO_BSRR_BS15;
	}
	else if (USART3 -> DR == 0x01) {
		GPIOD -> BSRR |= GPIO_BSRR_BR12;
		GPIOD -> BSRR |= GPIO_BSRR_BR13;
		GPIOD -> BSRR |= GPIO_BSRR_BR14;
		GPIOD -> BSRR |= GPIO_BSRR_BR15;
	}
}


/*******************************************************************************/
void Init_io_led_for_nextion (void) {
	
  RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;											// включить тактирование порта GPIOD
	
	GPIOD -> MODER |= GPIO_MODER_MODER12_0;											// (01) general purpose output mode
	GPIOD -> MODER |= GPIO_MODER_MODER13_0;
	GPIOD -> MODER |= GPIO_MODER_MODER14_0;
	GPIOD -> MODER |= GPIO_MODER_MODER15_0;
}
