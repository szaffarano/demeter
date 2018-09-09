/*
 * adc.c
 *
 *  Created on: 01/06/2014
 *      Author: sebas
 */

#include <avr/io.h>

void adc_init(void) {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}

uint16_t adc_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | channel;
	ADCSRA |= (1 << ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return ADC;
}
