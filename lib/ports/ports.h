/*
 * ports.h
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */

#ifndef PORTS_H_
#define PORTS_H_

void ports_init(void);

void enable_relay(uint8_t number);
void disable_relay(uint8_t number);
uint8_t is_pushed(void);
void blinkenlight(uint8_t times, uint8_t delay);
uint8_t is_relay_enabled(uint8_t relay_number);

#endif /* PORTS_H_ */
