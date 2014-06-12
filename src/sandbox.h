/*
 * sandbox.h
 *
 *  Created on: 08/06/2014
 *      Author: sebas
 */

#include <rtc/rtc.h>

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

#define LED_DDR			DDRB
#define	LED_PORT		PORTB
#define	LED				PB0

#define	MAX_EVENTS		10
// datos

typedef struct _event_t {
	rtc_datetime_24h_t start;
	unsigned int duration;
	uint8_t target;
} event_t;

#endif /* SANDBOX_H_ */
