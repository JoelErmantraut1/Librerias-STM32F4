#include "CE_SD_card_control.h"

// Librerias

uint8_t sd_mounted = 0;
FATFS FatFs = {0};

// Variables

uint8_t CE_mount_SD(SD_Card card, uint8_t force) {
	/*
	 * Esta funcion monta la tarjeta, solo si ya no esta montada.
	 *
	 * Force se usa cuando se llama a esta funcion
	 * explicitamente.
	 */
	if (sd_mounted | (f_mount(&FatFs, card.path, 1) == FR_OK)) {
		if (force) sd_mounted = 1;
		return 1;
	}
	else
		return 0;
}

void CE_unmount_SD(SD_Card card, uint8_t force) {
	/*
	 * Esta funcion desmonta la tarjeta, si esta montada.
	 *
	 * Force se usa cuando se llama a esta funcion
	 * explicitamente.
	 */
	if (force | !sd_mounted) {
		f_mount(0, "/", 1);
		sd_mounted = 0;
	}
}

uint8_t CE_write_SD(SD_Card card, char *filename, char *text, uint8_t ow) {
	/*
	 * La funcion recibe la estructura de la tarjeta de la cual se va a obtener
	 * el directorio raiz, recibe el nombre del archivo que se quiere escribir,
	 * el texto que se quiere escribir, y un parametro adicional que permite
	 * decidir que borrar el contenido antes de escribir (sobreescribir) o agregar
	 * contenido.
	 *
	 * Retorna 1 si tuvo exito en la escritura, o 0 en cualquier otro caso.
	 */

    FIL fil;
    uint8_t success = 0;
    // Esta variable se emplea para devolver 0 si fallo o 1 si tuvo exito

    // Inicializa retardo
    TM_DELAY_Init();
    // El autor de la libreria recomienda usar este conjunto de timer para
    // asegurar la precision de los temporizados

	if (CE_mount_SD(card, 0)) {

		// Intentar abrir el archivo
		if (f_open(&fil, filename, FA_OPEN_ALWAYS  | FA_WRITE) == FR_OK) {

			if (!ow) f_lseek(&fil, f_size(&fil));
			// Si ow == 0: Mueve el puntero al final de archivo para evitar sobreescritura
			// Si ow == 1: Comienza a escribir desde el principio, sobreescribiendo

			if (f_puts(text, &fil) > 0) {
				success = 1;
				// if (TM_FATFS_DriveSize(&total, &free) == FR_OK) {;}
				// Funcion que incorpora el autor para conocer el espacio total
				// y libre de la SD. DEMANDA MUCHO TIEMPO.
			};

			f_close(&fil);
		};

		CE_unmount_SD(card, 0);
		// IMPORTANTE: NO OLVIDAR DESMONTAR LA TARJETA
	};

	if (success) return 1;
	else return 0;
}

uint8_t CE_read_SD(SD_Card card, char *filename, char *buffer) {
	/*
	 * La funcion recibe la estructura de la tarjeta de la cual se va a obtener
	 * el directorio raiz, recibe el nombre del archivo que se quiere leer,
	 * y la variable en donde se almacenara el contenido.
	 *
	 * Esta funcion esta pensada para leer LINEA POR LINEA. Dado que los
	 * archivos que se leeran seran archivos de configuracion, que tendran
	 * un parametro a configurar, diferente en cada linea.
	 * Se supone, entonces, que la variable "buffer" es lo suficientemente
	 * grande como para almacenar el contenido.
	 *
	 * Retorna 1 si tuvo exito en la escritura, o 0 en cualquier otro caso.
	 */

    FIL fil;
    uint8_t success = 0;
    // Esta variable se emplea para devolver 0 si fallo o 1 si tuvo exito

    // Inicializa retardo
    TM_DELAY_Init();
    // El autor de la libreria recomienda usar este conjunto de timer para
    // asegurar la precision de los temporizados

	if (CE_mount_SD(card, 0)) {

		// Intentar abrir el archivo
		if (f_open(&fil, filename, FA_OPEN_EXISTING | FA_READ) == FR_OK) {
			/*
			 * Se seleccionar "FA_OPEN_EXISTING" porque no tiene sentido leer
			 * un archivo recien creado. El archivo de configuracion debio haber sido
			 * creado y almacenado en la tarjeta de antemano. Si no es asi, la
			 * funcion retornara error.
			 */

			f_gets(buffer, MAX_NUMBER_CHARS, &fil);

			if (strlen(buffer) > 0) success = 1;
			// Si se escribio algo en la variable

			f_close(&fil);
		};

		CE_unmount_SD(card, 0);
		// IMPORTANTE: NO OLVIDAR DESMONTAR LA TARJETA
	};

	if (success) return 1;
	else return 0;
}
