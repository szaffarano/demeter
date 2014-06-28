/*
 * timer.c
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */
#include <demeter.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include <stdio.h>

static volatile callback_t callback;
static uint8_t counter = 0;

/**
 * Rutina de inicializacion del timer0.
 * Inicializa:
 *  - Normal port operation
 *  - CTC mode
 *  - Preescaler en 1024 (8 mhz/ 256 = 31250 ticks/sec)
 *  - Output Compare Register A en 250: 250 * 125 = 31250 (cada 125 veces pasa 1 segundo, o sea, período de 8 millisegundos)
 */
void timer0_init(callback_t cb) {
	// Seteo del temporizador: preescaler de 256 y el registro
	// TCNT1 = 31250 se cuantifica 1 segundo, que sera la
	// granularidad manejada.

	TCCR0A = 0; /* seteo en Normal Port Operation */
	TCCR0A |= (1 << WGM01); /* modo CTC */
	TCCR0B = (1 << CS02); /* prescaler en 256 */

	OCR0A = 249; /* interrupcion TIMER0_COMPA con frecuencia de 125 hz */
	TIMSK0 |= (1 << OCIE0A); /* habilito interrupcion TIMER0_COMPA*/

	callback = cb;
}

ISR(TIMER0_COMPA_vect) {
	counter++;
	if (counter == 125) {
		if (callback != NULL) {
			callback();
		}
		counter = 0;
	}
}
