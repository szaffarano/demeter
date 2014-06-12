/*
 * config.c
 *
 *  Created on: 11/06/2014
 *      Author: sebas
 */
#include <avr/eeprom.h>
#include "config/config.h"

static event_t EEMEM ee_events[MAX_EVENTS];
static event_t events[MAX_EVENTS];

static uint8_t EEMEM ee_log_interval;
static uint8_t log_interval;

void parameters_init() {
	eeprom_read_block((void*) events, (const void*) ee_events,
			sizeof(event_t) * MAX_EVENTS);

	log_interval = eeprom_read_byte(&ee_log_interval);
}

event_t* get_events(void) {
	return events;
}

uint8_t get_log_interval(void) {
	return log_interval;
}

void set_events(event_t* data) {
	eeprom_update_block(events, ee_events, sizeof(event_t) * MAX_EVENTS);
}

void set_log_interval(uint8_t secs) {
	eeprom_update_byte(&ee_log_interval, secs);
	parameters_init();
}
