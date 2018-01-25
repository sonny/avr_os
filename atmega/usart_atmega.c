/*
 * usart_atmega.c
 *
 * Created: 11/30/2013 7:45:16 PM
 *  Author: Greg Cook
 */ 

#include <util/atomic.h>
#include "usart_atmega.h"

void usart_atmega_init(usart_atmega_regs_t *regs, register16_t baud) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		
		// Set baud rate
		regs->UBRRxH = (uint8_t)(baud >> 8);
		regs->UBRRxL = (uint8_t)baud;
			
		// Control and Status Reg B
		// 7 Rx complete Interrupt enable -- 0b1
		// 6 Tx complete Interrupt enable -- 0b0
		// 5 Data Register Empty Interrupt Enable -- 0b0
		// 4 Rx Enable -- 0b1
		// 3 Tx Enable -- 0b1
		// 2 -- see table 16-7 -- 0b0 (8 bit)
		// 1:0 tx/rx 9th bit 0b00
		// 0b1001 1000 --> 0x98
		regs->UCSRxB = 0b10011000;
			
		// set Frame format -- 8N1
		// 7:6 (0b00) Mode Select -- See table 16-4 pg 191
		// 5:4 (0b00) Parity -- table 16-5 pg 192
		// 3   (0b0)  Stop Bits (1-bit)
		// 2:1 (0b11) Character Size -- See table 16-7 pg 192
		// 0   (0b0)  Clock Polarity
		// 0b0000 0110 -> 0x06
		regs->UCSRxC = 0b00000110;
	}
}