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

#include <stdio.h>
#define LED_DDR		DDRB
#define	LED_PORT	PORTB
#define	LED			PB1

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

	while (1) {
		unsigned char r = receive();
		switch (r) {
		case 'a':
			printf("Letra A\r\n");
			break;
		case 'b':
			printf("Letra B\r\n");
			break;
		case 0:
			break;
		default:
			printf("Opción desconocida: %c\r\n", r);
		}
	}

	return (0);
}
