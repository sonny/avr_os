/*
 * systick_atmega.c
 *
 * Created: 10/16/2013 11:16:05 PM
 *  Author: Greg Cook
 */ 

#include <avr/io.h>
#include "systick_atmega.h"
#include "../timer.h"


void systick_atmega_init(void) {
	timer8_atmega_reg_t config8 = {
		.TCCRxA = _BV(WGM01), // WGM0 = 0b010
		.TCCRxB = _BV(FOC0A), // FOC0A = 0b1
		.TCNTx = 0,
		.OCRxA = 0, // (TOP)
		.OCRxB = 0  // (BOTTOM)
	};
	register_t timsk = 0b00000010;
	timer_atmega_regs_t config = { 
		.t8 = &config8, 
		.TIMSKx = &timsk
	};
	timer_init(DEV_TIMER0, &config);
}