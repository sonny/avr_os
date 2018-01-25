/*
 * systick.c
 *
 * Created: 7/21/2013 5:22:19 PM
 *  Author: Greg Cook
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "system.h"
#include "systick.h"
#include "atmega/systick_atmega.h"

/************************************************************************/
/* Systick Interface                                                    */
/************************************************************************/

volatile static tick_t __systick = 0;

ISR(TIMER0_COMPA_vect) {
	__systick++;
	TaskQueue.timer_callback();
}

inline tick_t systick_get(void) { return __systick; }

void systick_init(void) {
	systick_atmega_init();
	timer_set_counter(DEV_TIMER0, MSTICKS);
}

