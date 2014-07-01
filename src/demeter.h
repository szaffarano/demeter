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
#define  RELAY0			PB6
#define  RELAY0_PORT	PORTB
#define  RELAY0_DDR		DDRB

#define  RELAY1			PB7
#define  RELAY1_PORT	PORTB
#define  RELAY1_DDR		DDRB

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

typedef struct _datetime_t {
	int16_t year;
	int16_t month;
	int16_t date;
	int16_t hour;
	int16_t minute;
	int16_t second;
} datetime_t;

typedef struct _state_t {
	int16_t temperature;
	int16_t humidity;
	int16_t light;
} state_t;

typedef struct _instant_t {
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
} instant_t;

// un evento es el cron que indica cuándo activar un relay y por cuánto tiempo.
typedef struct _event_t {
	instant_t start;
	uint16_t duration;
	uint16_t target;
	uint16_t enabled;
} event_t;

#endif /* DEMETER_H_ */
