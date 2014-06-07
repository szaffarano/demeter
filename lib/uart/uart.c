/*
 * uart.c
 *
 *  Created on: 07/06/2014
 *      Author: sebas
 */

#include <avr/io.h>
#include <uart/uart.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

buffer_t tx = { .head = 0, .tail = 0 };
buffer_t rx = { .head = 0, .tail = 0 };
FILE file;

int send_stdout(char c, FILE *f);

/**
 * Inicializa la USART para enviar y recibir.
 * Formato: frame de 8 bits con 1 de parada
 */
void uart_init(uint16_t baud) {
	UBRR0H = (USART_BAUD_PRESCALE(baud) >> 8);
	UBRR0L = (0x00FF & USART_BAUD_PRESCALE(baud));

	UCSR0B |= _BV(RXCIE0); // habilitar interrupcion de recepcion de datos
	UCSR0B |= _BV(RXEN0) | _BV(TXEN0); // habilitar tx y rx
	UCSR0C |= _BV(UCSZ00) | _BV(UCSZ01); // frame de 8 bits

	// buffers de transmisión / recepción
	tx.buffer = malloc(USART_BUFFER_SIZE);
	rx.buffer = malloc(USART_BUFFER_SIZE);

	// sobreescribo stdout/stderr para wrapear en printf
	fdev_setup_stream(&file, send_stdout, NULL, _FDEV_SETUP_WRITE);
	stdout = &file;
	stderr = &file;
}

void send(uint8_t byte) {
	unsigned char head = (tx.head + 1) & USART_BUFFER_MASK;

	while (head == tx.tail) {
		// no hay espacio en el buffer
	}

	tx.buffer[head] = byte;
	tx.head = head;

	// habilito interrupción para enviar el dato.
	UCSR0B |= _BV(UDRIE0);
}

uint8_t receive(void) {
	uint8_t tail;
	uint8_t r;

	if (rx.tail == rx.head) {
		return 0;
	}
	tail = (rx.tail + 1) & USART_BUFFER_MASK;
	r = rx.buffer[tail];
	rx.tail = tail;

	return r;
}

ISR(USART_RX_vect) {
	uint8_t head;
	uint8_t data;

	//@TODO manejar estado / errores

	data = UDR0;

	head = (rx.head + 1) & USART_BUFFER_MASK;

	if (head == rx.tail) {
		// overflow
	} else {
		rx.head = head;
		rx.buffer[head] = data;
	}
}

ISR(USART_UDRE_vect) {
	if (tx.head != tx.tail) {
		tx.tail = (tx.tail + 1) & USART_BUFFER_MASK;
		UDR0 = tx.buffer[tx.tail];
	} else {
		// no hay mas datos para enviar, deshabilito la interrupción hasta tener más.
		UCSR0B &= ~_BV(UDRIE0);
	}
}

int send_stdout(char c, FILE *f) {
	send(c);
	return 0;
}
