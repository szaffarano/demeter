/*
 * timer.h
 *
 *  Created on: 03/06/2014
 *      Author: sebas
 */

#ifndef TIMER_H_
#define TIMER_H_

typedef void(*callback_t)(void);

void timer0_init(callback_t callback);

#endif /* TIMER_H_ */
