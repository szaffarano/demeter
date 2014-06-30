/*
 * port.c
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */
#include <demeter.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdint.h>

static inline void delay_ms(uint16_t);

void ports_init(void) {
	// interrupciones Pin Change
	PCICR |= (1 << PCIE2); // PCIE2 habilita PCINT[23:16]
	PCMSK2 |= (1 << PCINT23); // Solo dispara la PCINT para PD7 = PCINT23

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

void enable_relay(uint8_t number) {
	switch (number) {
	case 1:
		RELAY1_PORT &= ~(1 << RELAY1);
		break;
	case 2:
		RELAY2_PORT &= ~(1 << RELAY2);
		break;
	}
}

void disable_relay(uint8_t number) {
	switch (number) {
	case 1:
		RELAY1_PORT |= (1 << RELAY1);
		break;
	case 2:
		RELAY2_PORT |= (1 << RELAY2);
		break;
	}
}

void blinkenlight(uint8_t times, uint8_t delay) {
	int i = 0;
	for (i = 0; i < times; i++) {
		LED_PORT ^= _BV(LED);
		delay_ms(delay / 2);
		LED_PORT ^= _BV(LED);
		delay_ms(delay);
	}
	LED_PORT &= ~_BV(LED);
}

// ISRs
ISR(PCINT2_vect) {
	// si el boton esta presionado, el bit en PUSH_PORT es low
	if ((PUSH_PORT & (1 << PUSH)) == 0) {
		enable_relay(1);
	} else {
		disable_relay(1);
	}
}

static inline void delay_ms(uint16_t ms) {
	uint16_t i = 0;
	for (; i < ms; i++) {
		_delay_ms(1);
	}
}
