/*
 * main.c
 *
 * Created: 10/1/2013 7:21:50 PM
 *  Author: Greg Cook
 */ 

#include "system.h"
#include "task.h"
#include "systick.h"
#include "blinky.h"
#include "display.h"
#include "echo.h"
#include "adc.h"
#include "producer_consumer_demo.h"

int main(void) {
  sys_init();

  blinky_init();
  display_init();
  echo_init();
  producer_consumer_init();
	
  ADC_.start(ADC_CH0, 250);
  ADC_.start(ADC_CH1, 125);
  ADC_.start(ADC_CH2, 125);

  scheduler_run();
}
