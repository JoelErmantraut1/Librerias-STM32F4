#include "CE_BT_control.h"

void CE_init_BT(BT bt) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(bt.active_port_periferico, ENABLE);		//Habilitacion de la senal de
																	//reloj para el periferico GPIOD
	//Enmascaramos los pines que usaremos

	GPIO_InitStructure.GPIO_Pin =	bt.active_pin;

	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;					//Los pines seleccionados como salida
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;					//Tipo de salida como push pull
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;				//Velocidad del clock para el GPIO
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;					//Con pull downs

	GPIO_Init(bt.active_port, &GPIO_InitStructure); //Se aplica la configuracion definidas anteriormente

	// Configuracion del pin de alimentacion

	RCC_APB1PeriphClockCmd(bt.USART_periferico, ENABLE);
	RCC_AHB1PeriphClockCmd(bt.USART_port_periferico, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_Pin = bt.tx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(bt.port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = bt.rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(bt.port, &GPIO_InitStructure);

	GPIO_PinAFConfig(bt.port, bt.tx_port_source, bt.alt_fun);
	GPIO_PinAFConfig(bt.port, bt.rx_port_source, bt.alt_fun);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(bt.USART, &USART_InitStructure);
	// Configuracion del USART

	USART_ITConfig(bt.USART, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// Habilito la interrupcion del USART

	GPIO_SetBits(bt.active_port, bt.active_pin);
	// Enciendo el modulo
	USART_Cmd(bt.USART, ENABLE);
	// Habilito el USART
}

void CE_send_BT(BT bt, char *content) {
	/*
	 * Recibe una cadena e itera en cada caracter, enviandolos
	 * ciclicamente. Esta funcion trabaja por polling.
	 */

	int i;
	for (i = 0; i < strlen(content); i++) {

		char dato = content[i];
		USART2->DR = dato;

		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}

uint8_t CE_read_BT(BT bt, char *buffer) {
	/*
	 * Esta funcion debe ser llamada en el handler de la interrupcion
	 * USART que se defina. Cuando recibe el caracter de retorno de carro
	 * (CR) se termina la transmision de datos, y devuelve 1, indicando
	 * que ya se puede utilizar la informacion.
	 */

	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
		*buffer = USART_ReceiveData(USART2);

		return 1;
	}

	return 0;
}

uint8_t CE_read_word_BT(BT bt, char *buffer, int tam) {
	/*
	 * Esta funcion lee una palabra completa por bluetooth.
	 * Para esto, recordara la ultima posicion en la que
	 * escribio en la variable buffer, para guardar el nuevo
	 * caracter en la posicion apropiada.
	 */
	static int pos = 0;

	if (USART_GetFlagStatus(bt.USART, USART_FLAG_RXNE) == RESET) {
		char caracter = USART_ReceiveData(USART2);
		if (caracter == '\n' || caracter == '\r') {
			pos = 0;
			return 0;
		}
		else {
			buffer[pos] = caracter;
			pos++;
		}

		if (pos >= tam) {
			pos = 0;
			return 0;
		}
		return 1;
	}

	return 1;
}

void CE_power_off_BT(BT bt) {
	GPIO_ResetBits(bt.active_port, bt.active_pin);
}
