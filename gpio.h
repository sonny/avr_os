/*
 * device_gpio.h
 *
 * Created: 7/27/2013 12:43:30 PM
 *  Author: Greg Cook
 *
 * Generic GPIO and GPIO pin interface
 */ 

#ifndef DEVICE_GPIO_H_
#define DEVICE_GPIO_H_

#include <stdint.h>
#include "atmega/gpio_atmega.h"

typedef enum { 
  unset = 0,
  set   = 1 
} gpio_pinval;

typedef enum {
  in = 0,
  out = 1	
} gpio_direction;

typedef enum {
  pud = 0,
  puen = 1
} gpio_pullup;

typedef struct {
  gpio_atmega_regs_t *gpio;  // gpio device for pin
  uint8_t pin;		     // pin index (0-7)
  gpio_pinval val;	     // pin output value (shadow or config)
} gpio_pin_t;

void gpio_pin_set_direction(gpio_pin_t*, gpio_direction);
void gpio_pin_set_value(gpio_pin_t*, gpio_pinval);
void gpio_pin_set_pullup(gpio_pin_t*, gpio_pullup);
char gpio_pin_read_pin(const gpio_pin_t*);
void gpio_pin_toggle(gpio_pin_t*);

#define GPIOA_ADDRESS (gpio_atmega_regs_t*)0x20
#define GPIOB_ADDRESS (gpio_atmega_regs_t*)0x23
#define GPIOC_ADDRESS (gpio_atmega_regs_t*)0x26
#define GPIOD_ADDRESS (gpio_atmega_regs_t*)0x29

#define GPIO_PIN(port, pin) &(gpio_pin_ ## port[pin])

extern gpio_pin_t gpio_pin_A[8];
extern gpio_pin_t * const GPIOA0;
extern gpio_pin_t * const GPIOA1;
extern gpio_pin_t * const GPIOA2;
extern gpio_pin_t * const GPIOA3;
extern gpio_pin_t * const GPIOA4;
extern gpio_pin_t * const GPIOA5;
extern gpio_pin_t * const GPIOA6;
extern gpio_pin_t * const GPIOA7;

extern gpio_pin_t gpio_pin_B[8];
extern gpio_pin_t * const GPIOB0;
extern gpio_pin_t * const GPIOB1;
extern gpio_pin_t * const GPIOB2;
extern gpio_pin_t * const GPIOB3;
extern gpio_pin_t * const GPIOB4;
extern gpio_pin_t * const GPIOB5;
extern gpio_pin_t * const GPIOB6;
extern gpio_pin_t * const GPIOB7;

extern gpio_pin_t gpio_pin_C[8];
extern gpio_pin_t gpio_pin_D[8];


#endif /* DEVICE_GPIO_H_ */
