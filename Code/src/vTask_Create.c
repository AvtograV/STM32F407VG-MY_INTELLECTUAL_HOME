#include "vTask_Create.h"


void vTack_ADC1_send_USART3 (void *argument) {
	while (1) {
//		adc_resalt_send_uart3 ();
//		USART3_IRQHandler_Nextion ();
		USART3_send_byte('1');
		vTaskDelay(1000);
	}
}

void vTack_USART_send (void *argument) {
	while (1) {
		USART3_send_string("000\r\n");
		vTaskDelay(1000);
	}
}




/***************************** задача - подключение 1-wire *****************************/
void vTack_one_wire_conect (void *argument) {
	while (1) {
//		measure_temperature ();
//		USART3_IRQHandler_Nextion ();
		USART3_send_byte('1');
	}
}
