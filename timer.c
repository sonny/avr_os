/*
 * timer.c
 *
 * Created: 7/21/2013 5:57:47 PM
 *  Author: Greg Cook
 *
 * Generic timer device interface
 * Mostly a wrapper to the atmega timer
 */

#include <avr/interrupt.h>
#include "types.h"
#include "timer.h"

/**
 * Initialize timer device
 * @param dev Timer object
 * @param config Timer Config object
 * @return void
 */
void timer_init(timer_t * const dev, const timer_config_t *config) {
  timer_atmega_init(dev, config);
}

/**
 * Set timer countdown value along with prescalar if necessary
 * @param dev Timer object
 * @param count Unscaled value to count to
 * @return void
 */
void timer_set_counter(timer_t * const timer, uint32_t count) {
  timer->timer_set_prescaler(timer, count);
}

