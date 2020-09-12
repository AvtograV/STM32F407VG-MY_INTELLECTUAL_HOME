#ifndef _USART_H
#define _USART_H 

#include <stdint.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "vTask_Create.h"


#define APB1CLK				42000000

#define BAUDRATE_1			9600

void Init_usart_3(void);
void init_usart_2_for_1_wire (void);


/***************************** отправить байт по UART ****************************/
void USART2_send_byte(uint16_t ch);
void USART3_send_byte(char ch);

/********************** отправить число по USART3 *********************/
void USART3_send_data(char data);

/**************************** отправить строку по UART ***************************/
void USART2_send_string(char* str);
void USART3_send_string(char* str);

/***************************** изменить скорость UART ****************************/
void change_speed_USART (uint32_t set_speed);


#endif 			// USART_H
