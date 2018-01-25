/*
 * adc_atmega.h
 *
 * Created: 10/8/2013 11:05:51 PM
 *  Author: Greg Cook
 */ 


#ifndef ADC_ATMEGA_H_
#define ADC_ATMEGA_H_

#include <stdbool.h>
#include "types_atmega.h"

#define ADMUX_SIZE 5
#define ADMUX_OFF  0

typedef uint8_t adc_channel_t;

typedef struct {
	register_t adcl; // data reg l
	register_t adch; // data reg h
	// 7   ADCEN    -- Enable
	// 6   ADSC     -- Start Conversion
	// 5   ADATE    -- Auto Trigger Enable
	// 4   ADIF     -- Interrupt Flag
	// 3   ADIE     -- Interrupt Enable
	// 2:0 ADPS2:0  -- Prescaler Select (see table 20-5 pg 257)  0b110 => 64
	register_t adcsra; // control and status A
	// 7, 5:3 reserved
	// 2:0 ADT2:0 Auto Trigger Source (see table 20-6 pg 259)
	register_t adcsrb; // control and status B
	// 7:6 REFS1:0  -- reference voltage : 00 turned off
	// 5   ADLAR    -- ADC Left Adjust Result (right adjusted by default)
	// 4:0 MUX4:0   -- which Channel is selected (see table 20-4 pg 256)
	register_t admux; // MUX
	register_t reserved00;
	register_t didr0;	// Digital input disable
} adc_atmega_regs_t;

void adc_atmega_init(adc_atmega_regs_t*);
bool adc_atmega_is_busy(adc_atmega_regs_t*);
bool adc_atmega_se_start(adc_atmega_regs_t *regs);
void adc_atmega_change_channel(adc_atmega_regs_t *regs, adc_channel_t ch);
uint16_t adc_atmega_se_read(adc_atmega_regs_t*);

#endif /* ADC_ATMEGA_H_ */