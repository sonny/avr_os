/*
 * timer_atmega.c
 *
 * Created: 10/21/2013 12:00:29 AM
 *  Author: Greg Cook
 */ 
#include <util/atomic.h>
#include "timer_atmega.h"

static void timer_set_prescaler0(timer_atmega_t * const timer, const uint32_t count);
static void timer_set_prescaler1(timer_atmega_t * const timer, const uint32_t count);
static void timer_set_prescaler2(timer_atmega_t * const timer, const uint32_t count);

timer_atmega_t device_timer0 = {
	.is16bits = false,
	.regs.TIMSKx = (register_t *)0x6E,
	.regs.t8 = (timer8_atmega_reg_t *)0x44,
	.timer_set_prescaler = timer_set_prescaler0
};

timer_atmega_t device_timer1 = {
	.is16bits = true,
	.regs.TIMSKx = (register_t *)0x6F,
	.regs.t16 = (timer16_atmega_reg_t *)0x80,
	.timer_set_prescaler = timer_set_prescaler1
};

timer_atmega_t device_timer2 = {
	.is16bits = false,
	.regs.TIMSKx = (register_t *)0x70,
	.regs.t8 = (timer8_atmega_reg_t *)0xB0,
	.timer_set_prescaler = timer_set_prescaler2
};

 
static void timer_atmega_init8(timer_atmega_t * const timer, const timer_atmega_regs_t *config) {
	if (timer->initialized) return;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*(timer->regs.TIMSKx) = *(config->TIMSKx);
		timer->regs.t8->OCRxA = config->t8->OCRxA;
		timer->regs.t8->OCRxB = config->t8->OCRxB;
		timer->regs.t8->TCNTx = config->t8->TCNTx;
		timer->regs.t8->TCCRxA = config->t8->TCCRxA;
		timer->regs.t8->TCCRxB = config->t8->TCCRxB;
		timer->initialized = true;
	}
}

static void timer_atmega_init16(timer_atmega_t * const timer, const timer_atmega_regs_t *config) {
	if (timer->initialized) return;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		*(timer->regs.TIMSKx) = *(config->TIMSKx);
		timer->regs.t16->TCCRxA = config->t16->TCCRxA;
		timer->regs.t16->TCCRxB = config->t16->TCCRxB;
		timer->regs.t16->TCCRxC = config->t16->TCCRxC;
		timer->regs.t16->TCNTx = config->t16->TCNTx;
		timer->regs.t16->ICRx = config->t16->ICRx;
		timer->regs.t16->OCRxA = config->t16->OCRxA;
		timer->regs.t16->OCRxB = config->t16->OCRxB;
		timer->initialized = true;
	}
}

void timer_atmega_init(timer_atmega_t * const dev, const timer_atmega_regs_t *config) {
	if (dev->is16bits)
		timer_atmega_init16(dev, config);
	else
		timer_atmega_init8(dev, config);
}

static void timer_set_prescaler0(timer_atmega_t * const timer, const uint32_t count) {
	// calculate prescaler and counter TOP
	uint32_t counter = count  - 1;
	char prescaler = 0b001;

	// determine closest reasonable prescaler
	if (counter < UINT8_MAX) {
		prescaler = 0b001;
	}
	else if (counter / 8 < UINT8_MAX) {
		prescaler = 0b010;
		counter = counter / 8;
	}
	else if (counter / 64 < UINT8_MAX) {
		prescaler = 0b011;
		counter = counter / 64;
	}
	else if (counter / 256 < UINT8_MAX) {
		prescaler = 0b100;
		counter = counter / 256;
	}
	else {
		prescaler = 0b101;
		counter = counter / 1024;
	}
	timer->regs.t8->TCCRxB |= prescaler;
	timer->regs.t8->OCRxA = (register_t)counter;
}

static void timer_set_prescaler1(timer_atmega_t * const timer, const uint32_t count) {
       uint32_t counter = count  - 1;
       char prescaler = 0b001;

       // determine closest reasonable prescaler
       // for 16-bit counter 1
       if (counter < UINT16_MAX) {
	       prescaler = 0b001;
       }
       else if (counter / 8 < UINT16_MAX) {
	       prescaler = 0b010;
	       counter = counter / 8;
       }
       else if (counter / 64 < UINT16_MAX) {
	       prescaler = 0b011;
	       counter = counter / 64;
       }
       else if (counter / 256 < UINT16_MAX) {
	       prescaler = 0b100;
	       counter = counter / 256;
       }
       else {
	       prescaler = 0b101;
	       counter = counter / 1024;
       }

	// use mode 14 with ICR1 as TOP for fast PWM
	timer->regs.t16->TCCRxB |= prescaler;
	timer->regs.t16->ICRx = (register16_t)counter;
}

static void timer_set_prescaler2(timer_atmega_t * const timer, const uint32_t count) {
	// calculate prescaler and counter TOP
	uint32_t counter = count  - 1;
	char prescaler = 0b001;

	// determine closest reasonable prescaler
	if (counter < UINT8_MAX) {
		prescaler = 0b001;
	}
	else if (counter / 8 < UINT8_MAX) {
		prescaler = 0b010;
		counter = counter / 8;
	}
	else if (counter / 32 < UINT8_MAX) {
		prescaler = 0b011;
		counter = counter / 32;
	}
	else if (counter / 64 < UINT8_MAX) {
		prescaler = 0b100;
		counter = counter / 64;
	}
	else if (counter / 128 < UINT8_MAX) {
		prescaler = 0b101;
		counter = counter / 128;
	}
	else if (counter / 256 < UINT8_MAX) {
		prescaler = 0b110;
		counter = counter / 256;
	}
	else {
		prescaler = 0b111;
		counter = counter / 1024;
	}
	timer->regs.t8->TCCRxB |= prescaler;
	timer->regs.t8->OCRxA = (register_t)counter;
}

timer_atmega_t * const DEV_TIMER0 = &device_timer0;
timer_atmega_t * const DEV_TIMER1 = &device_timer1;
timer_atmega_t * const DEV_TIMER2 = &device_timer2;
