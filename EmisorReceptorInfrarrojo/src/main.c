/*
 * Archivo: 		main.c
 * Info: 			Generated by Atollic TrueSTUDIO(R) 9.3.0
 * Autor:			Joel Ermantraut
 * Ult. Mod.:		06/01/2021
 *
 * POSIBLES PROBLEMAS PUEDEN SER POR EL REBOTE PRODUCIDO
 * POR EL ENTORNO. SE RECOMIENDA ENCERRAR A LOS SENSORES EN
 * UN ESPACIO AISLADO DE LA LUZ.
*/

/* Includes */
#include "stm32f4xx.h"
#include "CE_EXTI_control.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */

void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);

/* Private functions */

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/

Entrada infrarrojo1 = {
	RCC_AHB1Periph_GPIOB,
	GPIOB,
	GPIO_Pin_4
};

Entrada infrarrojo2 = {
	RCC_AHB1Periph_GPIOB,
	GPIOB,
	GPIO_Pin_5
};

// Infrarrojos para detectar entrada o salida de abejas

EXTI_entrada int_infrarrojo1 = {
	EXTI_PortSourceGPIOB,
	EXTI_PinSource4,
	EXTI4_IRQn,
	EXTI_Line4,
	EXTI_Trigger_Rising
};
EXTI_entrada int_infrarrojo2 = {
	EXTI_PortSourceGPIOB,
	EXTI_PinSource5,
	EXTI9_5_IRQn,
	EXTI_Line5,
	EXTI_Trigger_Rising
};

// Lineas de interrupcion por entradas

int main(void)
{
 	SystemInit();

	CE_conf_in(infrarrojo1, GPIO_PuPd_NOPULL);
	CE_conf_in(infrarrojo2, GPIO_PuPd_NOPULL);
	CE_EXTI_config(infrarrojo2, int_infrarrojo2);
	CE_EXTI_config(infrarrojo1, int_infrarrojo1);

	while (1) {

	}
}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(int_infrarrojo1.int_line) != RESET)
	{

		EXTI_ClearITPendingBit(int_infrarrojo1.int_line);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(int_infrarrojo2.int_line) != RESET)
	{

		EXTI_ClearITPendingBit(int_infrarrojo2.int_line);
	}
}
