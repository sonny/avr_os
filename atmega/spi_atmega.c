/*
 * spi_atmega.c
 *
 * Created: 11/2/2013 11:13:27 PM
 *  Author: Greg Cook
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "spi_atmega.h"
#include "bits_atmega.h"

#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA PB5           //MOSI (SI)
#define LATCH PB4          //SS   (RCK)
#define CLOCK PB7          //SCK  (SCK)

void spi_atmega_init(spi_mode_t mode) {
	if (mode == SPI_MASTER) {
		//Setup IO
		SHIFT_REGISTER |= (_BV(DATA) | _BV(LATCH) | _BV(CLOCK)); //Set control pins as outputs
		SHIFT_PORT &= ~(_BV(DATA) | _BV(LATCH) | _BV(CLOCK));        //Set control pins low
			
		//Setup SPI
		SPCR0 = _BV(SPIE0) | _BV(SPE0) | _BV(MSTR0);  //Start SPI as Master
	}
}

void spi_atmega_set_data(uint8_t data) {
	SPDR0 = data;
}

void spi_atmega_ss(spi_ss_t ss) {
	if (ss == SPI_SS_SELECT)
		REG_CLRBIT(SHIFT_PORT, LATCH);
	else
		REG_SETBIT(SHIFT_PORT, LATCH);
}

void __attribute__ ((weak)) spi_stc_isr(void) {
	// does nothing ... override
}

ISR(SPI_STC_vect) {
	spi_stc_isr();
}