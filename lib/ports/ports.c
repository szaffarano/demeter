/*
 * port.c
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */
#include <sandbox.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void ports_init(void) {
	// interrupciones Pin Change
	PCICR |= (1 << PCIE0); // PCIE3 habilita PCINT[7:0]
	PCMSK0 |= (1 << PCINT1); // Solo dispara la PCINT para PB1 = PCINT1

	// I/O
	// pulsador para activar relay manualmente.
	PUSH_DDR &= ~(1 << PUSH); // setear PUSH como input
	PUSH_PORT |= (1 << PUSH); // pullup en PUSH

	// relays como output, por default estan low, pongo en high antes de habilitar
	// para evitar que se activen por unos ciclos de cpu
	RELAY1_PORT |= (1 << RELAY1); // RELAY1 high para evitar que se active de entrada
	RELAY1_DDR |= (1 << RELAY1); // RELAY1 como output

	RELAY2_PORT |= (1 << RELAY2); // RELAY2 en high para evitar que se active de entrada
	RELAY2_DDR |= (1 << RELAY2); // RELAY2 como output

	// led de status
	LED_DDR |= _BV(LED);
}

// ISRs
ISR(PCINT0_vect) {
	// si el boton esta presionado, el bit en PUSH_PORT es low
	if ((PUSH_PORT & (1 << PUSH)) == 0) {
		RELAY1_PORT &= ~(1 << RELAY1); // bit de relay low lo activa
	} else {
		RELAY1_PORT |= (1 << RELAY1);
	}
}
