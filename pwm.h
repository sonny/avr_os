/**
 * pwm.h
 *
 * Created: 7/19/2013 10:01:51 PM
 *  Author: Greg Cook
 */ 


#ifndef DEVICE_PWM_H_
#define DEVICE_PWM_H_

#include <stdint.h>
#include "types.h"
#include "timer.h"

typedef struct {
  char ddr_pin;
  register_t *ddr;
  register_ptr_t ocrx;
  register_ptr_t TOP;

  timer_atmega_regs_t config;
  timer_t *timer;
} device_pwm_t;

void pwm_init(device_pwm_t * const);
void pwm_set_duty(device_pwm_t * const pwm, uint16_t duty);
void pwm_set_duty_raw(device_pwm_t * const pwm, uint16_t raw);

extern device_pwm_t * const PWM0A;
extern device_pwm_t * const PWM0B;
extern device_pwm_t * const PWM1A;
extern device_pwm_t * const PWM1B;
extern device_pwm_t * const PWM2A;
extern device_pwm_t * const PWM2B;


#endif /* DEVICE_PWM_H_ */
