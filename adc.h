/*
 * device_adc.h
 *
 * Created: 7/18/2013 8:44:14 PM
 *  Author: Greg Cook
 */ 


#ifndef DEVICE_ADC_H_
#define DEVICE_ADC_H_

#include <stdint.h>
#include "atmega/adc_atmega.h"
#include "task.h"
#include "task_mutex.h"

typedef uint16_t adc_data_t;
typedef uint8_t adc_channel_t;

typedef struct {
  task_mutex_t lock;
  task_t *current;
  adc_data_t data[8];
  adc_atmega_regs_t *regs;
} adc_dev_t;

typedef struct {
  adc_channel_t channel;
} adc_task_data_t;

extern adc_dev_t * const ADC_DEV;

extern const adc_channel_t ADC_CH0;
extern const adc_channel_t ADC_CH1;
extern const adc_channel_t ADC_CH2;
extern const adc_channel_t ADC_CH3;
extern const adc_channel_t ADC_CH4;
extern const adc_channel_t ADC_CH5;
extern const adc_channel_t ADC_CH6;
extern const adc_channel_t ADC_CH7;

typedef struct {
  void (* const init)(void);
  void (* const start)(adc_channel_t, tick_t);
  void (* const stop)(adc_channel_t);
  adc_data_t (* const read)(adc_channel_t);
} adc_class_t;

extern const adc_class_t ADC_ ;

#endif /* DEVICE_ADC_H_ */
