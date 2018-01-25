/*
 * adc_atmega.c
 *
 * Created: 10/8/2013 11:11:59 PM
 *  Author: Greg Cook
 */ 

#include <avr/io.h>
#include "types_atmega.h"
#include "adc_atmega.h"
#include "bits_atmega.h"

void adc_atmega_init_default(adc_atmega_regs_t *regs) {
	regs->didr0 = 0b11111111; // disable digital input
	regs->admux = 0b01000000; // avcc=3.3v, adlar=0, channel 0
	regs->adcsrb = 0; // free running mode -- run, Forrest, run
	// enable, start, auto-trigger, isr flag, isr enable, prescaler = 64
	// 1     , 0    , 0           , 0       , 1         , 110
	regs->adcsra = 0b10001110;
}

void adc_atmega_init(adc_atmega_regs_t *regs) {
	adc_atmega_init_default(regs);
}

inline bool adc_atmega_is_busy(adc_atmega_regs_t *regs) {
	return bit_is_set(regs->adcsra, ADSC);
}

inline bool adc_atmega_se_start(adc_atmega_regs_t *regs) {
	if (adc_atmega_is_busy(regs)) return false;
	REG_SETBIT(regs->adcsra, ADSC);
	return true;
}

inline void adc_atmega_change_channel(adc_atmega_regs_t *regs, adc_channel_t ch) {
	REG_SETBITS(regs->admux, ch, ADMUX_SIZE, ADMUX_OFF);
}

inline uint16_t adc_atmega_se_read(adc_atmega_regs_t *regs) {
	uint16_t result = regs->adcl;
	result |= (regs->adch << 8);
	return result;
}