/**
 * pwm.c
 *
 * Created: 7/19/2013 10:12:27 PM
 *  Author: Greg Cook
 */ 

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "pwm.h"
#include "timer.h"

/**
 * default pwm config for timer2
 */
static const timer8_atmega_reg_t pwm2_timer_config = {
  // COM2A = 0b10, COM2B = 0b10, WGM2 = 0b011 (pwm fast mode)
  .TCCRxA = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20),
  .TCCRxB = _BV(CS20) // No Prescaler
};

/**
 * default pwm config for timer1
 */
static const timer16_atmega_reg_t pwm1_timer_config = {
  // configure timer 1 as 8 bit fast PWM
  // COM1A = 0b10, COM1B = 0b10, WGM1 = 0b0101 
  .TCCRxA = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10),
  .TCCRxB = _BV(WGM12) | _BV(CS10),
  .TCCRxC = 0,
  .OCRxA = 0,
  .OCRxB = 0
};

static register_t pwm_timer_interrupt_mask = 0;
static register16_t pwm_timer16_top = 0xff;

/**
 * Config for PWM1A driver
 */
static device_pwm_t pwm_1A = { // OC1A --> PD5
  .ddr_pin = DDRB5,
  .ddr = (register_t*)0x2A, // DDRD
  .ocrx.p16 = (register16_t*)0x88, // OCR1A
  .config.t16 = (timer16_atmega_reg_t * const)&pwm1_timer_config,
  .config.TIMSKx = &pwm_timer_interrupt_mask,
  .timer = DEV_TIMER(1),
  .TOP.p16 = (register16_t *)&pwm_timer16_top
};

/**
 * Config for PWM1B driver
 */
static device_pwm_t pwm_1B = { // OC1B --> PD4
  .ddr_pin = DDRB4,
  .ddr = (register_t*)0x2A, // DDRD
  .ocrx.p16 = (register16_t*)0x8A, // OCR1B
  .config.t16 = (timer16_atmega_reg_t * const)&pwm1_timer_config,
  .config.TIMSKx = &pwm_timer_interrupt_mask,	
  .timer = DEV_TIMER(1),
  .TOP.p16 = (register16_t *)&pwm_timer16_top
};

/**
 * Config for PWM2A driver
 */
static device_pwm_t pwm_2A = {
  .ddr_pin = DDRB7,
  .ddr = (register_t*)0x2A, //DDRD
  .ocrx.p8 = (register_t*)0xB3,
  .config.t8 = (timer8_atmega_reg_t * const)&pwm2_timer_config,
  .config.TIMSKx = &pwm_timer_interrupt_mask,
  .timer = DEV_TIMER(2)
};

/**
 * Config for PWM2B driver
 */
static device_pwm_t pwm_2B = {
  .ddr_pin = DDRB6,
  .ddr = (register_t*)0x2A,  //DDRD
  .ocrx.p8 = (register_t*)0xB4, // OCR2B
  .config.t8 = (timer8_atmega_reg_t * const)&pwm2_timer_config,
  .config.TIMSKx = &pwm_timer_interrupt_mask,
  .timer = DEV_TIMER(2)
};

device_pwm_t * const PWM0A = NULL;
device_pwm_t * const PWM0B = NULL;
device_pwm_t * const PWM1A = &pwm_1A;
device_pwm_t * const PWM1B = &pwm_1B;
device_pwm_t * const PWM2A = &pwm_2A;
device_pwm_t * const PWM2B = &pwm_2B;

// global flags to avoid having the timers initialized more than once

/**
 * Initialize PWM driver
 * @param pwm PWM device to initialize
 * @return void
 */
void pwm_init(device_pwm_t *pwm) {
  *(pwm->ddr) |= _BV(pwm->ddr_pin);
  timer_init(pwm->timer, &pwm->config);
}

/**
 * Set output compare register for 8-bit counter 
 * @param pwm PWM device
 * @param val Value to set
 * @return void
 */
static inline void pwm_set_OCRX8(device_pwm_t *pwm, const uint16_t val) {
  *(pwm->ocrx.p8) = (register_t)val;
}

/**
 * Set output compare for 16-bit counter (requires atomic block)
 * @param pwm PWM device
 * @param val Value to set
 * @return void
 */
static inline void pwm_set_OCRX16(device_pwm_t *pwm, const uint16_t val) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    *(pwm->ocrx.p16) = (register16_t)val;
  }
}

/**
 * Set output compare register for counter (8 or 16-bit) 
 * @param pwm PWM device
 * @param val Value to set
 * @return void
 */
static inline void pwm_set_OCRX(device_pwm_t *pwm, const uint16_t val) {
  if (pwm->timer->is16bits)
    pwm_set_OCRX16(pwm, val);
  else
    pwm_set_OCRX8(pwm, val);
}

/**
 * Set raw PWM duty cycle, value is unscaled.
 * @param pwm PWM device
 * @param raw Value to set
 * @return void
 */
void pwm_set_duty_raw(device_pwm_t *pwm, uint16_t raw) {
  pwm_set_OCRX(pwm, raw);
}

/**
 * Set PWM duty cycle, value is scaled from [0..1023] to counter size
 * @param pwm PWM device
 * @param duty Value to set on range [0..1023]
 * @return void
 *
 */
void pwm_set_duty(device_pwm_t *pwm, uint16_t duty) {
  if (pwm->timer->is16bits) {
    uint32_t top = pwm->TOP.p16 ? *(pwm->TOP.p16) : UINT16_MAX;
    uint32_t scaled = ((uint32_t)duty * top) / 1023;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      *(pwm->ocrx.p16) = (register16_t)scaled;
    }
  }
  else {
    uint16_t top = pwm->TOP.p8 ? *(pwm->TOP.p8) : UINT8_MAX;
    uint32_t scaled = ((uint32_t)duty * top) / 1023;
    *(pwm->ocrx.p8) = (register_t)scaled;
  }
}
