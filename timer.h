/*
 * timer.h
 *
 * Created: 7/21/2013 4:52:54 PM
 *  Author: Greg Cook
 *
 * Generic timer device interface
 */ 


#ifndef DEVICE_TIMER_H_
#define DEVICE_TIMER_H_

#include <stdbool.h>
#include "types.h"
#include "atmega/timer_atmega.h"

typedef struct timer_t timer_t;
typedef struct timer_atmega_regs_t timer_config_t;

void timer_init(timer_t * const, const timer_config_t*);
void timer_set_counter(timer_t * const timer, uint32_t count);

extern timer_t * const DEV_TIMER0;
extern timer_t * const DEV_TIMER1;
extern timer_t * const DEV_TIMER2;

#define DEV_TIMER(t) (&device_timer ## t)

#endif /* DEVICE_TIMER_H_ */
