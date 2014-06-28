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

void blinkenlight(uint8_t times, uint8_t delay);

#endif /* PORTS_H_ */
