/*
 * bits_atmega.h
 *
 * Created: 10/20/2013 11:38:16 PM
 *  Author: Greg Cook
 */ 


#ifndef BITS_ATMEGA_H_
#define BITS_ATMEGA_H_

#include <avr/sfr_defs.h>

#define REG_SETBIT(r, b)        ((r) |= _BV(b))
#define REG_CLRBIT(r, b)        ((r) &= ~_BV(b))

#define REG_MASK(s, o)          (((1<<s)-1) << o)
#define REG_CLRBITS(r, s, o)    ((r) &= ~REG_MASK(s, o))
#define REG_SETBITS(r, v, s, o) ((r) = (((r) & ~REG_MASK(s, o)) | (v & REG_MASK(s, o))))


#endif /* BITS_ATMEGA_H_ */