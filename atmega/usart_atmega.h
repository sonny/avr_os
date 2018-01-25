/*
 * usart_atmega.h
 *
 * Created: 11/30/2013 7:39:26 PM
 *  Author: Greg Cook
 */ 


#ifndef USART_ATMEGA_H_
#define USART_ATMEGA_H_

#include "types_atmega.h"

typedef struct {
	register_t UCSRxA;
	register_t UCSRxB;
	register_t UCSRxC;
	register_t reserved00;
	register_t UBRRxL;
	register_t UBRRxH;
	register_t UDRx;	
} usart_atmega_regs_t;

void usart_atmega_init(usart_atmega_regs_t *regs, register16_t baud);

#endif /* USART_ATMEGA_H_ */