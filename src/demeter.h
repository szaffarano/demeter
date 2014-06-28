/*
 * demeter.h
 *
 *  Created on: 08/06/2014
 *      Author: sebas
 */

#ifndef SANDBOX_H_
#define SANDBOX_H_

#include <rtc/rtc.h>

// Relays
#define  RELAY1			PB6
#define  RELAY1_PORT	PORTB
#define  RELAY1_DDR		DDRB

#define  RELAY2			PB7
#define  RELAY2_PORT	PORTB
#define  RELAY2_DDR		DDRB

// Pulsador
#define  PUSH			PD7
#define  PUSH_PORT		PIND
#define  PUSH_DDR		DDRD

// UART
#define UART_BAUD_RATE	9600

// sensor luminico
#define	PHOTORESISTOR	PC0

#define	LOG				"clima.log"

#define	LED				PB0
#define	LED_PORT		PORTB
#define LED_DDR			DDRB

// cantidad de eventos que permite parametrizar.
#define	MAX_EVENTS		10

typedef struct _instant_t {
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} instant_t;

// un evento es el cron que indica cuándo activar un relay y por cuánto tiempo.
typedef struct _event_t {
	instant_t start;
	unsigned int duration;
	uint8_t target;
} event_t;

#endif /* DEMETER_H_ */
