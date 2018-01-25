/*
 * gpio_atmega.h
 *
 * Created: 12/2/2013 7:57:57 PM
 *  Author: Sonny
 */ 


#ifndef GPIO_ATMEGA_H_
#define GPIO_ATMEGA_H_

#include "types_atmega.h"

typedef struct {
	register_t PINx;  // port pin input value
	register_t DDRx;  // port pin direction
	register_t PORTx; // port data values
} gpio_atmega_regs_t;

void gpio_atmega_init(gpio_atmega_regs_t *device, gpio_atmega_regs_t conf);

#endif /* GPIO_ATMEGA_H_ */