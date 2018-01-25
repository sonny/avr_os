/*
 * uart.h
 *
 * Created: 11/30/2013 7:48:54 PM
 *  Author: Greg Cook
 */ 


#ifndef UART_H_
#define UART_H_

#define UART_HW_FLOW_CTL 1

#include <stdio.h>
#include "atmega/usart_atmega.h"
#include "ring_buffer.h"
#include "gpio.h"

typedef struct {
  usart_atmega_regs_t *regs;
  ring_buffer_t *tx_buffer;
  ring_buffer_t *rx_buffer;
  gpio_pin_t * rts;
  gpio_pin_t * cts;
  int(*putc)(char, FILE*);
  int(*getc)(FILE*);	
} uart_t;

void uart_init(uart_t *uart, uint32_t baud);

extern uart_t * const UART0;
extern uart_t * const UART1;

#endif /* UART_H_ */
