/*
 * gpio_atmega.c
 *
 * Created: 12/2/2013 7:58:23 PM
 *  Author: Sonny
 */ 

#include "gpio_atmega.h"

void gpio_atmega_init(gpio_atmega_regs_t *device, gpio_atmega_regs_t conf) {
	device->PINx = conf.PINx;
	device->DDRx = conf.DDRx;
	device->PORTx = conf.PORTx;
}
