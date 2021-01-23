#include "stm32f4xx.h"
#include "CE_SD_card_control.h"

void GPIO_config(void);

SD_Card card = {
		"/",
		{"file.txt"},
		1
};

int main(void) {
    //Initialize system
    SystemInit();

    while (1) {

    	CE_mount_SD(card, 1);

    	CE_write_SD(card, card.files[0], "hola", 0);

    	CE_unmount_SD(card, 1);

    }
}
