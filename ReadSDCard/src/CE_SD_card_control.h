#ifndef CE_SD_CARD_CONTROL_H
#define CE_SD_CARD_CONTROL_H

#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <string.h>

// Librerias

#define MAX_NUMBER_CHARS	10
// Este identificador se utiliza en la funcion "CE_read_SD"

// Defines

typedef struct {
	char *path;
	char *files; // Lista de ruta de archivos
	int32_t n_files; // Cantidad de archivos (tamaño files)
} SD_Card;

// Estructuras

uint8_t CE_mount_SD(SD_Card card, uint8_t force);
void CE_unmount_SD(SD_Card card, uint8_t force);
uint8_t CE_write_SD(SD_Card card, char *filename, char *text, uint8_t ow);
uint8_t CE_read_SD(SD_Card card, char *filename, char *buffer);

// Prototipos

#endif
