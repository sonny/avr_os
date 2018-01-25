/*
 * systick.h
 *
 * Created: 7/21/2013 5:19:59 PM
 *  Author: Greg Cook
 */ 


#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "task.h"
#include "types.h"
#include "timer.h"

void systick_init(void);
tick_t systick_get(void);
void timed_task_queue_init(void);

#endif /* SYSTICK_H_ */
