/*
 * timer_atmega.h
 *
 * Created: 10/20/2013 11:49:39 PM
 *  Author: Greg Cook
 */ 


#ifndef TIMER_ATMEGA_H_
#define TIMER_ATMEGA_H_

#include <stdbool.h>
#include "types_atmega.h"

typedef struct {
	register_t TCCRxA;
	register_t TCCRxB;
	register_t TCNTx;
	register_t OCRxA;
	register_t OCRxB;
} timer8_atmega_reg_t;

typedef struct {
	register_t TCCRxA;
	register_t TCCRxB;
	register_t TCCRxC;
	register_t reserved00;
	register_t TCNTxL;
	register_t TCNTxH;
	register_t ICRxL;
	register_t ICRxH;
	register_t OCRxAL;
	register_t OCRxAH;
	register_t OCRxBL;
	register_t OCRxBH;
} timer16_atmega_reglh_t;

typedef struct {
	register_t TCCRxA;
	register_t TCCRxB;
	register_t TCCRxC;
	register_t reserved00;
	register16_t TCNTx;
	register16_t ICRx;
	register16_t OCRxA;
	register16_t OCRxB;
} timer16_atmega_reg_t;


typedef struct {
	uint16_t counter;
	char prescaler;
} timer_prescaler_t;

typedef struct timer_atmega_regs_t {
	union {
		timer8_atmega_reg_t *t8;
		timer16_atmega_reg_t *t16;
	};
	register_t *TIMSKx;
} timer_atmega_regs_t;

struct timer_t {
	bool initialized;
	bool is16bits;
//	register_t *TIMSKx;
	void (*timer_set_prescaler)(struct timer_t*, uint32_t);
//	timer_reg_t regs;
	timer_atmega_regs_t regs;
};

typedef struct timer_t timer_atmega_t;

extern timer_atmega_t device_timer0;
extern timer_atmega_t device_timer1;
extern timer_atmega_t device_timer2;

void timer_atmega_init(timer_atmega_t * const dev, const timer_atmega_regs_t *config);


#endif /* TIMER_ATMEGA_H_ */