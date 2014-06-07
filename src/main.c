/*
 * main.c
 *
 *  Created on: 07/06/2014
 *      Author: sebas
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart/uart.h>
#include <string.h>

#include <stdio.h>

#include <ff/ff.h>

#define LED_DDR		DDRB
#define	LED_PORT	PORTB
#define	LED			PB4

FATFS fs;
FIL log_file;

/**
 * MCU: Atmega328
 * Fuses: Oscilador interno a 8 Mhz (sin dividir por 8)
 * 		lfuse: E2
 * 		hfuse: D9
 * 		efuse: 7
 */
int main(void) {
	uart_init(9600);
	sei();

	LED_DDR |= _BV(LED);
	int i = 0;
	for (i = 0; i < 7; i++) {
		LED_PORT ^= _BV(LED);
		_delay_ms(100);
	}
	LED_PORT &= ~_BV(LED);

	UINT bw;

	printf("Montando FS...\r\n");
	f_mount(&fs, "", 0);
	char buff[100];
	for (i = 0; i < 10; i++) {
		if (f_open(&log_file, "datos.log", FA_OPEN_ALWAYS | FA_WRITE | FA_READ)
				== FR_OK) {
			f_lseek(&log_file, f_size(&log_file));
			sprintf(buff, "Hola mundo %d\n", i);
			f_write(&log_file, buff, strlen(buff), &bw);
			f_close(&log_file);
			if (bw == strlen(buff)) {
				LED_PORT |= _BV(LED);
			} else {
				printf("No se pudo escribir en el archivo\r\n");
			}
		} else {
			printf("No se pudo abrir el archivo.\r\n");
		}

	}
	while (1) {
		unsigned char r = uart_getc();
		switch (r) {
		case 'a':
			printf(".Letra A\r\n");
			break;
		case 'b':
			printf(".Letra B\r\n");
			break;
		case 0:
			break;
		default:
			printf("Opción desconocida: %c\r\n", r);
		}
	}

	return (0);
}

DWORD get_fattime(void) {
	/* Returns current time packed into a DWORD variable */
	return ((DWORD) (2013 - 1980) << 25) /* Year 2013 */
	| ((DWORD) 7 << 21) /* Month 7 */
	| ((DWORD) 28 << 16) /* Mday 28 */
	| ((DWORD) 0 << 11) /* Hour 0 */
	| ((DWORD) 0 << 5) /* Min 0 */
	| ((DWORD) 0 >> 1); /* Sec 0 */
}
