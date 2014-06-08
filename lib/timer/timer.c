/*
 * timer.c
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */
#include <sandbox.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include <stdio.h>

static volatile callback_t callback;

void timer1_init(callback_t cb) {
	// Seteo del temporizador: preescaler de 256 y el registro
	// TCNT1 = 31250 se cuantifica 1 segundo, que sera la
	// granularidad manejada.
	TCCR1A = 0;    // Timer1 en normal port operation.
	TCCR1B = (1 << CS12); // prescaler en 256
	TCCR1B |= (1 << WGM12); // seteo para CTC (clear time on compare match)
	OCR1A = 31249; // tics para que pase un segundo segun el prescaler en 1024
	TIMSK1 |= (1 << OCIE1A); // bit para habilitar CTC interrupt

	callback = cb;
}

ISR(TIMER1_COMPA_vect) {
	if (callback != NULL) {
		callback();
	}
}

