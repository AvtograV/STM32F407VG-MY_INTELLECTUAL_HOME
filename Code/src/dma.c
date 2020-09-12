#include "dma.h"


uint8_t ow_t_buf[8];																								// Буфер для передачи по 1-wire
uint8_t ow_r_buf[8];																								// Буфер для приёма по 1-wire

/*****************************************************************************************************************/
void USART2_TX_DMA_Init(void)	{
	// Включить тактирование DMA1
	if ((RCC -> AHB1ENR & RCC_AHB1ENR_DMA1EN) != RCC_AHB1ENR_DMA1EN)
		RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		
	DMA1_Stream6 -> CR &= ~DMA_SxCR_EN;
	DMA1_Stream6 -> PAR = (uint32_t)&(USART2 -> DR);
	DMA1_Stream6 -> M0AR = (uint32_t) ow_t_buf;												// адрес буфера в памяти 
	
	//----------------- Манипуляции с регистром конфигурации  ----------------
	DMA1_Stream6 -> CR |= DMA_SxCR_CHSEL_2;														// Выбрать 4 канал (100)
	DMA1_Stream6 -> CR |= DMA_SxCR_DIR_0;															// Направление - чтение из памяти в переферию (01)
	DMA1_Stream6 -> CR |= DMA_SxCR_MINC;															// Инкремент указателя памяти включен
	DMA1_Stream6 -> CR |= DMA_SxCR_TCIE;															// Transfer complete interrupt enable

	USART2 -> CR3 |= USART_CR3_DMAT;
}

/*****************************************************************************************************************/
void USART2_RX_DMA_Init(void)	{
	// Включить тактирование DMA1
	if ((RCC -> AHB1ENR & RCC_AHB1ENR_DMA1EN) != RCC_AHB1ENR_DMA1EN)
		RCC -> AHB1ENR |= RCC_AHB1ENR_DMA1EN;
		
	DMA1_Stream5 -> CR &= ~DMA_SxCR_EN;
	DMA1_Stream5 -> PAR = (uint32_t)&(USART2 -> DR);
	DMA1_Stream5 -> M0AR = (uint32_t)&ow_r_buf[0];
	//----------------- Манипуляции с регистром конфигурации  ----------------
	DMA1_Stream5 -> CR |= DMA_SxCR_CHSEL_2;														// Выбрать 4 канал (100)
	DMA1_Stream5 -> CR &= ~DMA_SxCR_DIR;															// Направление - чтение из перефbрии в память  (00)
	DMA1_Stream5 -> CR |= DMA_SxCR_MINC;															// Инкремент указателя памяти включен
	
	USART2 -> CR3 |= USART_CR3_DMAR;
}

/******************************* обмен в канале "память-DMA1-USART2-DMA1-память *********************************/
void exchange_mem_DMA_USART (void) {
		
	DMA1_Stream6 -> NDTR = sizeof(ow_t_buf); 												// указать количество передаваемых -
	DMA1_Stream5 -> NDTR = sizeof(ow_t_buf);												// принимаемых данных		
		
	// Перед пересылкой данных через USART с использованием DMA, требуется
	// сбросить флаг TC записью в него 0.
	USART2 -> SR &= ~USART_SR_TC;

	// Datasheet требует сброса этого бита после завершения передачи блока данных
	// с помощью DMA; здесь сброс и установка бита выполняются последовательно.
	USART2 -> CR3 &= ~USART_CR3_DMAR;
	USART2 -> CR3 |= USART_CR3_DMAR;

	// Включаем каналы DMA (первым - канал для получения данных от USART).
	DMA1_Stream5 -> CR |= DMA_SxCR_EN;
	DMA1_Stream6 -> CR |= DMA_SxCR_EN;

	// Ждём полного завершения пересылки данных
	// (что сильно снижает полезность применения DMA в этом примере).
	while((DMA1 -> HISR & (DMA_HISR_TCIF5 | DMA_HISR_TCIF6))!=
							(DMA_HISR_TCIF5 | DMA_HISR_TCIF6)) {}

	DMA1_Stream5 -> CR |= DMA_SxCR_EN;
	DMA1_Stream6 -> CR |= DMA_SxCR_EN;
				
	DMA1 -> HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CTCIF6;
				
	// Ждём завершения передачи данных через USART
	while(!(USART2 -> SR & USART_SR_TC)) {}
}
