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
static uint8_t pushed;
static uint8_t relays;

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
	RELAY0_PORT |= (1 << RELAY0); // RELAY0 high para evitar que se active de entrada
	RELAY0_DDR |= (1 << RELAY0); // RELAY0 como output

	RELAY1_PORT |= (1 << RELAY1); // RELAY1 en high para evitar que se active de entrada
	RELAY1_DDR |= (1 << RELAY1); // RELAY1 como output

	// led de status
	LED_DDR |= _BV(LED);

	relays = 0;
}

void enable_relay(uint8_t number) {
	switch (number) {
	case 0:
		RELAY0_PORT &= ~(1 << RELAY0);
		relays |= (1 << number);
		break;
	case 1:
		RELAY1_PORT &= ~(1 << RELAY1);
		relays |= (1 << number);
		break;
	}
}

void disable_relay(uint8_t number) {
	switch (number) {
	case 0:
		RELAY0_PORT |= (1 << RELAY0);
		relays &= ~(1 << number);
		break;
	case 1:
		RELAY1_PORT |= (1 << RELAY1);
		relays &= ~(1 << number);
		break;
	}
}

uint8_t is_pushed(void) {
	return pushed;
}

uint8_t is_relay_enabled(uint8_t relay_number) {
	return (relays & (1 << relay_number)) ? 1 : 0;
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
		pushed = 1;
	} else {
		pushed = 0;
		disable_relay(1);
	}
}

static inline void delay_ms(uint16_t ms) {
	uint16_t i = 0;
	for (; i < ms; i++) {
		_delay_ms(1);
	}
}
