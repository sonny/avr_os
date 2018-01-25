/*
 * blinky.c
 *
 * Created: 11/2/2013 2:04:17 PM
 *  Author: Greg Cook
 */

#include <stdlib.h> 
#include "gpio.h"
#include "task.h"

#define BLINK_TICKS 250

static task_t *blink_task;

/**
 * Blinky task callback (only one) -- toggles state of LED
 * @param Task object
 * @return zero index to slice array and reschedule on periodic timer
 */ 
static task_slice_result_t blink_callback(task_t *t) {
  (void)t;
  gpio_pin_toggle(GPIOB0);

  return (task_slice_result_t){0,TASK_RESCHED};
}

static const task_slice_callback_fp const blink_task_slices[] = {
  blink_callback
};

/**
 * Initialize blinky task
 */
void blinky_init(void){
  gpio_pin_set_direction(GPIOB0, out);
  gpio_pin_set_value(GPIOB0, set);

  blink_task = Task.new(blink_task_slices, NULL, BLINK_TICKS, true);
  Task.schedule(blink_task, TASK_RESCHED);
}
