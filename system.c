/*
 * system.c
 *
 * Created: 7/6/2013 4:58:28 PM
 *  Author: Greg Cook
 * 
 * System tasks
 */ 

#include <util/atomic.h>
#include "system.h"
#include "uart.h"
#include "vt100.h"
#include "task.h"
#include "adc.h"

#define DEFAULT_BAUD 500000
//#define DEFAULT_BAUD 38400


/**
 * Initialize several system devices in one go
 */
void sys_init(void) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    systick_init();
    scheduler_init();
    ADC_.init();
    uart_init(UART0, DEFAULT_BAUD);
    term_init(TERM0);
  }
}
