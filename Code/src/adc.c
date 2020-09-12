#include "adc.h"


uint16_t res = 0;

const uint16_t  pow10Table16[]=
{
    10000ul,
    1000ul,
    100ul,
    10ul,
    1ul
};

static char *utoa_cycle_sub(uint16_t value, char *buffer) {
   if (value == 0)  {
      buffer[0] = '0';
      buffer[1] = 0;
      return buffer;
   }
	 
   char *ptr = buffer;
   uint8_t i = 0;
	 
   do {
      uint16_t pow10 = pow10Table16[i++];
      uint8_t count = 0;
		 
      while(value >= pow10) {
         count ++;
         value -= pow10;
      }			
     *ptr++ = count + '0';
   }
	 
		while(i < 5);
			*ptr = 0;	 
																					// удаляем ведущие нули
   while(buffer[0] == '0') ++buffer;
   return buffer;
}

/*******************************************************************************************************/

void init_adc1 (void) {
	
   			RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;				// включить тактирование порта GPIOA				
				GPIOA -> MODER |= GPIO_MODER_MODER1;					// (11) режим PA1 - аналоговый вход
				
				RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;					// включить тактирование модуля ADC1	
				ADC -> CCR |= ADC_CCR_ADCPRE;									// 11: PCLK2 divided by 8
	
				ADC1 -> CR1  &= ~ADC_CR1_RES;									// (00) разрешение 12 бит
				ADC1 -> CR2  &= ~ADC_CR2_CONT;								// режим одиночного преобразование	
				ADC1 -> CR2  &= ~ADC_CR2_ALIGN;								// (0) выравнивание по правому краю				
				ADC1 -> SQR1 &= ~ADC_SQR1_L;									// задаем количество каналов для преобразования 
																											// в регулярной группе ADC1 (0000) - один канал	
				ADC1->CR2 |= ADC_CR2_EXTSEL;                                                         
				ADC1->CR2 |= ADC_CR2_EXTEN;
	
				ADC1 -> SQR3 |= ADC_SQR3_SQ1_0;								// (PA1) задаем номер канала (первый канал) первого
																											//  преобразования регулярной группы ADC1
	
				ADC1 -> SMPR2 |= ADC_SMPR2_SMP1;							// (111) длительность выборки для 1-го канала в 480 циклов
	
				ADC1 -> CR2 |= ADC_CR2_ADON;									// Enable ADC					
}

/*******************************************************************************************************/
	
	uint16_t adc_resalt_send_uart3 (void) {
		
				ADC1 -> CR2 |= ADC_CR2_SWSTART;								// запуск преобразований
        while (!(ADC1 -> SR & ADC_SR_EOC));						// ждем пока преобразование завершится
       		
				res = ADC1 -> DR;
		
				char buffer[] = {0,0,0,0};		
				utoa_cycle_sub(res, buffer);
		
				USART3_send_string("n1.val=1234");
				USART3_send_string(buffer);
		
				USART3_send_data(0xFF);
				USART3_send_data(0xFF);
				USART3_send_data(0xFF);
				
				vTaskDelay(1000);
		
	if 			(res > 0  	&& res <= 870)		return 1;
	else if (res > 870  && res <= 1040)		return 100;
	else if (res > 1040 && res <= 1210)		return 200;
	else if (res > 1210 && res <= 1380)		return 300;
	else if (res > 1380 && res <= 1550)		return 400;
	else if (res > 1550 && res <= 1720)		return 500;
	else if (res > 1720 && res <= 1890) 	return 600;
	else if (res > 1890 && res <= 2060) 	return 700;	
	else if (res > 2060 && res <= 2230)		return 800;	
	else if (res > 2230 && res <= 2400) 	return 900;	
	else if (res > 2400 && res <= 2570) 	return 1000;	
	else if (res > 2570 && res <= 2740)		return 1100;	
	else if (res > 2740 && res <= 2910) 	return 1200;	
	else if (res > 2910 && res <= 3080) 	return 1300;	
	else if (res > 3080 && res <= 3250) 	return 1400;	
	else if (res > 3250 && res <= 3420) 	return 1500;	
	else if (res > 3420 && res <= 3590) 	return 1600;	
	else if (res > 3590 && res <= 3760) 	return 1700;
	else if (res > 3760 && res <= 3930) 	return 1800;	
	else																	return 2000;
}



/*******************************************************************************************************/
uint16_t adc_resalt_for_motor (void) {
	
		ADC1 -> CR2 |= ADC_CR2_SWSTART;								// запуск преобразований
    while (!(ADC1 -> SR & ADC_SR_EOC));						// ждем пока преобразование завершится
       		
			res = ADC1 -> DR;	
	
	if 			(res > 0  	&& res <= 870)		return 1;
	else if (res > 870  && res <= 1040)		return 100;
	else if (res > 1040 && res <= 1210)		return 200;
	else if (res > 1210 && res <= 1380)		return 300;
	else if (res > 1380 && res <= 1550)		return 400;
	else if (res > 1550 && res <= 1720)		return 500;
	else if (res > 1720 && res <= 1890) 	return 600;
	else if (res > 1890 && res <= 2060) 	return 700;	
	else if (res > 2060 && res <= 2230)		return 800;	
	else if (res > 2230 && res <= 2400) 	return 900;	
	else if (res > 2400 && res <= 2570) 	return 1000;	
	else if (res > 2570 && res <= 2740)		return 1100;	
	else if (res > 2740 && res <= 2910) 	return 1200;	
	else if (res > 2910 && res <= 3080) 	return 1300;	
	else if (res > 3080 && res <= 3250) 	return 1400;	
	else if (res > 3250 && res <= 3420) 	return 1500;	
	else if (res > 3420 && res <= 3590) 	return 1600;	
	else if (res > 3590 && res <= 3760) 	return 1700;
	else if (res > 3760 && res <= 3930) 	return 1800;	
	else																	return 2000;
}
