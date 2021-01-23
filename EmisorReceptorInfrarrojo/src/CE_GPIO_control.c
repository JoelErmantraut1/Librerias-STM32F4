#include "CE_GPIO_control.h"

uint8_t CE_leer_entrada(Entrada entrada) {
	/*
	 * Lee una entrada de un pin digital.
	 */
	uint8_t state = GPIO_ReadInputDataBit(entrada.port, entrada.pin);

	return state;
}

void CE_escribir_salida(Salida salida, uint8_t state) {
	/*
	 * Escribe en un pin de salida digital.
	 */
	if (!state)	GPIO_ResetBits(salida.port, salida.pin);
	else GPIO_SetBits(salida.port, salida.pin);

	// Si state es 0, apaga
	// Cualquier otro valor, prende
}

void CE_conf_in(Entrada entrada, GPIOPuPd_TypeDef PuPd) {
	GPIO_InitTypeDef GPIO_InitStructure; 					//Estructura de configuracion

	RCC_AHB1PeriphClockCmd(entrada.periferico, ENABLE);		//Habilitacion de la senal de
															//reloj para el periferico GPIOD
	//Enmascaramos los pines que usaremos

	GPIO_InitStructure.GPIO_Pin = entrada.pin;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//Los pines seleccionados como salida
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//Tipo de salida como push pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//Velocidad del clock para el GPIO
	GPIO_InitStructure.GPIO_PuPd = PuPd;					//Con pull downs

	GPIO_Init(entrada.port, &GPIO_InitStructure); 				//Se aplica la configuracion definidas anteriormente
}

void CE_conf_out(Salida salida) {
	GPIO_InitTypeDef GPIO_InitStructure; 					//Estructura de configuracion

	RCC_AHB1PeriphClockCmd(salida.periferico, ENABLE);		//Habilitacion de la senal de
															//reloj para el periferico GPIOD
	//Enmascaramos los pines que usaremos

	GPIO_InitStructure.GPIO_Pin = salida.pin;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//Los pines seleccionados como salida
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;					//Tipo de salida como push pull
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				//Velocidad del clock para el GPIO
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;					//Con pull downs

	GPIO_Init(salida.port, &GPIO_InitStructure); //Se aplica la configuracion definidas anteriormente
}

uint8_t CE_pulsador_presionado(Entrada pulsadores[], uint8_t n) {
	/*
	 * Esta funcion considera un grupo de N pulsadores.
	 *
	 * Cada uno se maneja individualmente, como entradas aisladas.
	 */
	uint8_t botonApretado = NO_BUTTON;
	uint8_t i;

	for(i = 0; i < n; i++) {
		if(CE_leer_entrada(pulsadores[i])) {
			botonApretado = i + 1;
			// Porque el 0 es para NO_BUTTON
			break;
		}
	}

	return botonApretado;
}

uint8_t CE_matriz_presionado(
	Entrada entradas[],
	Salida salidas[],
	int n
) {
	/*
	 * Esta funcion considera un keypad o matriz de pulsadores de 4 botones.
	 */

	uint8_t botonApretado = (uint8_t)NO_BUTTON;
	uint8_t i, k;

	for(k = 0; k < n; k++) {
		for(i = 0; i < n; i++) {
			if(i == k) CE_escribir_salida(salidas[i], Bit_High);
			else CE_escribir_salida(salidas[i], Bit_Low);
			// Coloca todas las salidas en 0, excepto la que corresponda
		}
		// Ahora que tengo la salida correspondiente activada, mido las entradas

		for(i = 0; i < n; i++) {
			if (CE_leer_entrada(entradas[i]) == Bit_High) {
				// Cuando encuentro una entrada activada
				// obtengo el numero de pulsador que le corresponde

				botonApretado = k * 2 + i + 1;
				// k e i representan bit, y aca los convierto a decimal
			}
		}
	}

	return botonApretado;
}
