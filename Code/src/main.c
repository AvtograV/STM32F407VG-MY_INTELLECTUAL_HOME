#include "main.h"

int main (void) {
	
	RCC_Init ();
	
	init_usart_2_for_1_wire();
	
	USART2_TX_DMA_Init ();
	USART2_RX_DMA_Init ();
				
		while (1) {
			measure_temperature ();
		}
}
