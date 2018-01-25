/*
 * types.h
 *
 * Created: 7/21/2013 4:39:43 PM
 *  Author: Greg Cook
 */ 


#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include "atmega/types_atmega.h"

#define TICK_MAX UINT32_MAX

typedef uint32_t tick_t;

typedef struct {
	union {
		register_t *p8;
		register16_t *p16;
		};
} register_ptr_t;

#endif /* TYPES_H_ */