/**
 * display.c
 *
 * Created: 10/23/2013 11:37:59 PM
 *  Author: Greg Cook
 *
 * Display ADC values to terminal asyncronously
 * Example of Single Task Slice with statically declared task
 */ 
#include <stdlib.h>
#include <util/atomic.h>
#include "systick.h"
#include "adc.h"
#include "task.h"
#include "vt100.h"

/**
 * Statically declared task example
 */
static task_t display_task = {
  .start_ticks = 200 // 5 times/sec
};

static void display_time(void) {
  tick_t systicks = systick_get();
  int s = systicks / 1000;
  int ms = systicks % 1000;
  term_display_region(TERM0, 3, 3, "Runtime %d.%03d Seconds", s, ms);
}

static task_slice_result_t term_display_callback(task_t *t) {
  term_display_region(TERM0, 1, 0, "ADC Raw Values");
  int val = ADC_.read(ADC_CH0);
  term_display_region(TERM0, 1, 1, "ADC 0: Val [%4d]", val);
	
  val = ADC_.read(ADC_CH1);
  term_display_region(TERM0, 1, 2, "ADC 1: Val [%4d]", val);

  val = ADC_.read(ADC_CH2);
  term_display_region(TERM0, 1, 3, "ADC 2: Val [%4d]", val);

  display_time();
  return (task_slice_result_t){0,TASK_RESCHED};
}

/**
 * Single Task Slice
 */
static const task_slice_callback_fp term_display_slices[] = {
  term_display_callback
};

void display_init(void) {
  Task.init(&display_task, term_display_slices, NULL);
  Task.schedule(&display_task, TASK_RESCHED);
}
