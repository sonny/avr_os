/*
 * system.h
 *
 * Created: 7/8/2013 8:13:31 PM
 *  Author: Greg Cook
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/interrupt.h>
#include "systick.h"

#define SYSCLOCK ((uint32_t)8000000)
#define MSTICKS (SYSCLOCK/1000L)

void sys_init(void);

#endif /* SYSTEM_H_ */
