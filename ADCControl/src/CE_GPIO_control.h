#ifndef CE_GPIO_CONTROL_H
#define CE_GPIO_CONTROL_H

/*
 * ABSTRACT:
 *
 * Esta libreria facilita el control de entradas y salidas.
 * Incluye:
 * 	 - Control de una entrada y salida individual.
 * 	 - Control de una matriz de pulsadores.
 * 	 - Control de pulsadores tactiles.
 *
 */

#include "stm32f4xx.h"
#include <stm32f4xx_gpio.h>

// Librerias

typedef struct {
	uint32_t periferico; // RCC_AHB1Periph_GPIOA
	GPIO_TypeDef* port;	 // GPIOA
	uint16_t pin;		 // GPIO_Pin_0
} Entrada;

typedef struct {
	uint32_t periferico; // RCC_AHB1Periph_GPIOA
	GPIO_TypeDef* port;	 // GPIOA
	uint16_t pin;		 // GPIO_Pin_0
	uint8_t pin_source;  // GPIO_PinSource0
} Salida;

// Estructuras

enum {
	Bit_Low = 0,
	Bit_High
} Bit_State;

// Enumeraciones

enum {
	NO_BUTTON = 0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5,
	BUTTON_6,
	BUTTON_7,
	BUTTON_8,
	BUTTON_9,
	BUTTON_10,
	BUTTON_11,
	BUTTON_12,
	BUTTON_13,
	BUTTON_14,
	BUTTON_15,
	BUTTON_16
};
// Soporta teclados de hasta 16 teclas

// Enumeraciones

void CE_conf_in(Entrada entrada, GPIOPuPd_TypeDef PuPd);
void CE_conf_out(Salida salida);
uint8_t CE_leer_entrada(Entrada entrada);
void CE_escribir_salida(Salida salida, uint8_t state);
uint8_t CE_pulsador_presionado(Entrada pulsadores[], uint8_t n);
uint8_t CE_matriz_presionado(
	Entrada entradas[],
	Salida salidas[],
	int n
);

// Prototipos

#endif
