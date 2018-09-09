/*
 * config.h
 *
 *  Created on: 11/06/2014
 *      Author: sebas
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <demeter.h>

void parameters_init(void);
event_t* get_events(void);
uint8_t get_log_interval(void);
void set_events(event_t* data);
void set_log_interval(uint8_t secs);

#endif /* CONFIG_H_ */
