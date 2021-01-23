#ifndef CE_BT_ADC_CONTROL_H
#define CE_BT_ADC_CONTROL_H

#include "stm32f4xx.h"

// Librerias

#define MAX_ADC 4095
#define MAX_VOLT 2900

// Defines

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
	uint32_t perif_port;
	uint32_t perif_adc;
	ADC_TypeDef *adc;
	uint8_t injected_channel;
	uint8_t channel;
} ADC_PIN;

// Estructuras

void CE_ADC_init(ADC_PIN adc);
int32_t CE_ADC_read(ADC_PIN adc, uint8_t format);

// Prototipos

#endif
