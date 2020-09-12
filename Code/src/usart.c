#include "usart.h"

										/*** USART2 PA2 - Half duplex mode ***/	
void init_usart_2_for_1_wire(void) {	
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 																// enable RCC for Port A
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 															// enable UART clocking
	
		
		// PA2 alternate function (USART = 0111)
	GPIOA -> AFR[0] |=  GPIO_AFRL_AFSEL2;
	GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
		// PA2 Alternate Push-Pull (10) !!! ALTERNATE !!!
	GPIOA -> MODER |=  GPIO_MODER_MODE2;
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_0;
				
							/************* UART2 *************/
	USART2 -> CR1 &= (uint32_t)~(USART_CR1_TE);
	USART2 -> CR1 &= (uint32_t)~(USART_CR1_RE);
	
	USART2 -> CR3 |= USART_CR3_HDSEL;																		// Half duplex mode
	USART2 -> BRR = ((APB1CLK + BAUDRATE_1) / 2) / BAUDRATE_1;

	USART2 -> CR1 |= (uint32_t)(USART_CR1_TE);													// TX enable
	USART2 -> CR1 |= (uint32_t)(USART_CR1_RE);													// RX enable
	USART2 -> CR1 |= (uint32_t)(USART_CR1_UE);													// USART1 enable	
}


/*********************************************************************/
						/*** USART3 PB10 --> USART3_TX PB11 --> USART3_RX ***/
void Init_usart_3(void) {
		
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; 															// enable RCC for Port B
	RCC -> APB1ENR |= RCC_APB1ENR_USART3EN; 														// enable UART clocking
		
	// PB10 alternate function (USART = 0111)
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL10; 															// set all bits
	GPIOB -> AFR[1] &= ~GPIO_AFRH_AFSEL10_3; 														// clr last bit

		// PB11 alternate function (USART = 0111)
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL11; 															// set all bits
	GPIOB -> AFR[1] &= ~GPIO_AFRH_AFSEL11_3; 														// clr last bit

		// PB10 Alternate Push-Pull (10) !!! ALTERNATE !!!
	GPIOB -> MODER &= ~GPIO_MODER_MODE10;
	GPIOB -> MODER |= GPIO_MODER_MODE10_1;
	
		// PB11 floating input (10)
	GPIOA -> MODER &= ~GPIO_MODER_MODE11;
	GPIOA -> MODER |= GPIO_MODER_MODE11_1;	
			
							/************* UART3 *************/
	USART3 -> BRR =(APB1CLK + BAUDRATE_1 / 2) / BAUDRATE_1;
	USART3 -> CR1 |= USART_CR1_RE; 															// RX enable
	USART3 -> CR1 |= USART_CR1_TE; 															// TX enable
	USART3 -> CR1 |= USART_CR1_UE; 															// USART3 enable
}


/********************** отправить байт по UART3 *********************/
void USART3_send_byte(char ch) {
	while(!(USART3 -> SR & USART_SR_TC)) {};
		USART3 -> DR = ch;
}


/********************** отправить строку по UART *********************/
void USART3_send_string(char* str) {
	uint8_t i = 0;
	while(str[i]) {
		USART3_send_byte(str[i++]);
	}
}

/********************** отправить число по USART3 ********************/
void USART3_send_data(char data) {
	while(!(USART3 -> SR & USART_SR_TC)) {}
			USART3 -> DR = data;
}

/*********************** принять байт по USART3 **********************/
void USART3_IRQHandler (void) {
	USART3 -> CR1 &= ~USART_CR1_RXNEIE;
	// что то делаем
}

/********************** отправить байт по UART2 **********************/
void USART2_send_byte(uint16_t ch) {
	while(!(USART2 -> SR & USART_SR_TC)) {}
			USART2 -> DR = ch;
}


/********************* отправить строку по USART2 ********************/
void USART2_send_string(char* str) {
	uint8_t i = 0;
	while(str[i]) {
		USART2_send_byte(str[i++]);
	}
}

/********************** изменить скорость USART2 *********************/
void change_speed_USART (uint32_t set_speed) {
	USART2 -> CR1 &= (uint32_t)~(USART_CR1_TE);
	USART2 -> CR1 &= (uint32_t)~(USART_CR1_RE);
	
	USART2 -> BRR =(APB1CLK + set_speed / 2) / set_speed;
	
	USART2 -> CR1 |= (uint32_t)(USART_CR1_TE);
	USART2 -> CR1 |= (uint32_t)(USART_CR1_RE);
}
