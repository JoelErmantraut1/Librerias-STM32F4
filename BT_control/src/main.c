/*
 * Archivo: 		main.c
 * Info: 			Generated by Atollic TrueSTUDIO(R) 9.3.0
 * Autor:			Joel Ermantraut
 * Ult. Mod.:		06/01/2021
 *
*/

#include <stdio.h>
#include "CE_BT_control.h"
#include <string.h>

// Librerias

int ready = 0;

// Variables

void USART2_IRQHandler(void);

BT bt = {
	GPIOD,
	GPIO_Pin_14,
	RCC_AHB1Periph_GPIOD,
	GPIOA,
	GPIO_Pin_2,
	GPIO_Pin_3,
	USART2,
	GPIO_AF_USART2,
	RCC_APB1Periph_USART2,
	RCC_AHB1Periph_GPIOA,
	GPIO_PinSource2,
	GPIO_PinSource3
};

// Prototipos

int main(void) {

	CE_init_BT(bt);

	char buffer[20];
	int i;
	int response;

	for (i = 0; i < 20; i++) {
		buffer[i] = '\0';
	}

	while(1) {

		if (ready) {
			response = CE_read_word_BT(bt, buffer, 3);
			ready = 0;
		}

		if (!response) {
			if (!strcmp(buffer, "zzz")) {
				CE_send_BT(bt, "x112");
			}
			response = 1;
			int i;
			for (i = 0; i < 20; i++) {
				buffer[i] = '\0';
			}
		}

	}

}

void USART2_IRQHandler(void) {
	if(USART_GetITStatus(bt.USART, USART_IT_RXNE) != RESET)
	{
		ready = 1;
		USART_ClearITPendingBit(bt.USART, USART_IT_RXNE);
	}
}
