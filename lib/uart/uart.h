/*
 * uart.h
 *
 *  Created on: 07/06/2014
 *      Author: sebas
 */

#ifndef UART_H_
#define UART_H_

#define USART_BAUD_PRESCALE(BR) ((F_CPU) / ((BR) * 16l) - 1)

#define USART_BUFFER_SIZE			32
#define USART_BUFFER_MASK			(USART_BUFFER_SIZE - 1)
#define	USART_NEXT_BUFFER_IDX(idx)	((idx+1) & USART_BUFFER_MASK)

typedef struct _buffer_t {
	volatile unsigned char* buffer;
	volatile unsigned char push_idx;
	volatile unsigned char pop_idx;
} buffer_t;

void uart_init(uint16_t baud);

void send(uint8_t byte);
uint8_t receive(void);

#endif /* UART_H_ */
