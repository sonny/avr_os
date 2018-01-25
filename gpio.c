/*
 * device_gpio.c
 *
 * Created: 7/27/2013 1:15:02 PM
 *  Author: Greg Cook
 *
 * Generic GPIO and GPIO pin interface
 */ 

#include "gpio.h"
#include "atmega/bits_atmega.h"

gpio_pin_t gpio_pin_A[8] = {
  { .gpio = GPIOA_ADDRESS, .pin = 0, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 1, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 2, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 3, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 4, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 5, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 6, .val = unset	},
  { .gpio = GPIOA_ADDRESS, .pin = 7, .val = unset	},
};

gpio_pin_t * const GPIOA0 = &gpio_pin_A[0];
gpio_pin_t * const GPIOA1 = &gpio_pin_A[1];
gpio_pin_t * const GPIOA2 = &gpio_pin_A[2];
gpio_pin_t * const GPIOA3 = &gpio_pin_A[3];
gpio_pin_t * const GPIOA4 = &gpio_pin_A[4];
gpio_pin_t * const GPIOA5 = &gpio_pin_A[5];
gpio_pin_t * const GPIOA6 = &gpio_pin_A[6];
gpio_pin_t * const GPIOA7 = &gpio_pin_A[7];

gpio_pin_t gpio_pin_B[8] = {
  { .gpio = GPIOB_ADDRESS, .pin = 0, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 1, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 2, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 3, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 4, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 5, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 6, .val = unset	},
  { .gpio = GPIOB_ADDRESS, .pin = 7, .val = unset	},
};

gpio_pin_t * const GPIOB0 = &gpio_pin_B[0];
gpio_pin_t * const GPIOB1 = &gpio_pin_B[1];
gpio_pin_t * const GPIOB2 = &gpio_pin_B[2];
gpio_pin_t * const GPIOB3 = &gpio_pin_B[3];
gpio_pin_t * const GPIOB4 = &gpio_pin_B[4];
gpio_pin_t * const GPIOB5 = &gpio_pin_B[5];
gpio_pin_t * const GPIOB6 = &gpio_pin_B[6];
gpio_pin_t * const GPIOB7 = &gpio_pin_B[7];

gpio_pin_t gpio_pin_C[8] = {
  { .gpio = GPIOC_ADDRESS, .pin = 0, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 1, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 2, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 3, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 4, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 5, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 6, .val = unset	},
  { .gpio = GPIOC_ADDRESS, .pin = 7, .val = unset	},
};

gpio_pin_t * const GPIOC0 = &gpio_pin_C[0];
gpio_pin_t * const GPIOC1 = &gpio_pin_C[1];
gpio_pin_t * const GPIOC2 = &gpio_pin_C[2];
gpio_pin_t * const GPIOC3 = &gpio_pin_C[3];
gpio_pin_t * const GPIOC4 = &gpio_pin_C[4];
gpio_pin_t * const GPIOC5 = &gpio_pin_C[5];
gpio_pin_t * const GPIOC6 = &gpio_pin_C[6];
gpio_pin_t * const GPIOC7 = &gpio_pin_C[7];

gpio_pin_t gpio_pin_D[8] = {
  { .gpio = GPIOD_ADDRESS, .pin = 0, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 1, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 2, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 3, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 4, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 5, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 6, .val = unset	},
  { .gpio = GPIOD_ADDRESS, .pin = 7, .val = unset	},
};

gpio_pin_t * const GPIOD0 = &gpio_pin_D[0];
gpio_pin_t * const GPIOD1 = &gpio_pin_D[1];
gpio_pin_t * const GPIOD2 = &gpio_pin_D[2];
gpio_pin_t * const GPIOD3 = &gpio_pin_D[3];
gpio_pin_t * const GPIOD4 = &gpio_pin_D[4];
gpio_pin_t * const GPIOD5 = &gpio_pin_D[5];
gpio_pin_t * const GPIOD6 = &gpio_pin_D[6];
gpio_pin_t * const GPIOD7 = &gpio_pin_D[7];

/**
 * Set gpio pin to 1
 * @param p GPIO_pin object
 * @return void
 */
inline void gpio_pin_set_pin(gpio_pin_t *p) {
  p->gpio->PORTx |= (1 << p->pin);
}

/**
 * Set gpio pin to 0
 * @param p GPIO_pin object
 * @return void
 */
inline void gpio_pin_unset_pin(gpio_pin_t *p) {
  p->gpio->PORTx &= ~(1 << p->pin);
}

/**
 * Toggle gpio pin value
 * @param p GPIO_pin object
 * @return void
 */
inline void gpio_pin_toggle(gpio_pin_t *p) {
  p->gpio->PINx |= (1 << p->pin);	
}

/**
 * Set direction of GPIO pin to input or output
 * @param p GPIO_pin object
 * @param d Direction, one of (in, out)
 * @return void
 */
void gpio_pin_set_direction(gpio_pin_t *p, gpio_direction d) {
  if (d) {
    REG_SETBIT(p->gpio->DDRx, p->pin);
  }
  else
    REG_CLRBIT(p->gpio->DDRx, p->pin);
}

/**
 * Set GPIO pin to value
 * @param p GPIO_pin object
 * @param v Value to set, one of (set, unset)
 * @return void
 */
void gpio_pin_set_value(gpio_pin_t *p, gpio_pinval v) {
  if (v)
    REG_SETBIT(p->gpio->PORTx, p->pin);
  else
    REG_CLRBIT(p->gpio->PORTx, p->pin);
}

/**
 * Set GPIO pin pullup to value
 * @param p GPIO_pin object
 * @param v Value to set, one of (pud, puen)
 * @return void
 */
void gpio_pin_set_pullup(gpio_pin_t *p, gpio_pullup v) {
  if (v)
    REG_SETBIT(p->gpio->PORTx, p->pin);
  else
    REG_CLRBIT(p->gpio->PORTx, p->pin);
}

/**
 * Read GPIO pin value
 * @param p GPIO_pin object
 * @return bit value from pin
 */
char gpio_pin_read_pin(const gpio_pin_t *p) {
  uint8_t mask = (1 << p->pin);
  char val = (p->gpio->PINx & mask) >> p->pin;
  return val;
}
