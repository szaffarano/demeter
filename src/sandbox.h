/*
 * sandbox.h
 *
 *  Created on: 08/06/2014
 *      Author: sebas
 */

#ifndef SANDBOX_H_
#define SANDBOX_H_

// Relays
#define  RELAY1			PD3
#define  RELAY1_PORT	PORTD
#define  RELAY1_DDR		DDRD

#define  RELAY2			PD4
#define  RELAY2_PORT	PORTD
#define  RELAY2_DDR		DDRD

// Pulsador
#define  PUSH			PB1
#define  PUSH_PORT		PINB
#define  PUSH_DDR		DDRB

// UART
#define UART_BAUD_RATE	9600

// sensor luminico
#define	PHOTORESISTOR	PC0

#define	LOG				"clima.log"
#define	LOG_INTERVAL	30

#define LED_DDR		DDRB
#define	LED_PORT	PORTB
#define	LED			PB0

#endif /* SANDBOX_H_ */
