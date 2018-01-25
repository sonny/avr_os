/*
 * device_adc.c
 *
 * Created: 7/18/2013 10:26:40 PM
 *  Author: Greg Cook
 */ 

#include <stdlib.h>
#include <assert.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "adc.h"

// function prototypes for task states
static task_slice_result_t adc_state_init(task_t*);
static task_slice_result_t adc_state_start(task_t*);
static task_slice_result_t adc_state_sample(task_t*);
static task_slice_result_t adc_state_record(task_t*);

static const task_slice_callback_fp const adc_task_slices[] = {
  adc_state_init,
  adc_state_start,
  adc_state_sample,
  adc_state_record
};

static adc_dev_t adc_device = {
  .lock.owner = NULL,
  .current = NULL,
  .regs = (adc_atmega_regs_t*)0x78
};

adc_dev_t * const ADC_DEV = &adc_device; 

static task_t adc_tasks[8];
static adc_task_data_t adc_task_data[8];

const adc_channel_t ADC_CH0 = 0;
const adc_channel_t ADC_CH1 = 1;
const adc_channel_t ADC_CH2 = 2;
const adc_channel_t ADC_CH3 = 3;
const adc_channel_t ADC_CH4 = 4;
const adc_channel_t ADC_CH5 = 5;
const adc_channel_t ADC_CH6 = 6;
const adc_channel_t ADC_CH7 = 7;

#define TASK_STATE_START  1
#define TASK_STATE_SAMPLE 2
#define TASK_STATE_RECORD 3

/**
 * adc task callback to initialize the channel
 *
 * @param task task object pased to the callback by the scheduler
 * @return next state and schedule value
 */
static task_slice_result_t adc_state_init(task_t *task) { return (task_slice_result_t){0,0}; }

/**
 * adc task callback to begin lock the device and set the channel
 *
 * @param task task object pased to the callback by the scheduler
 * @return next state and schedule value
 */
static task_slice_result_t adc_state_start(task_t *task) {
  task_slice_result_t result = { TASK_STATE_START, TASK_SCHED_IMMED };

  if (!task->enabled) {
    result.sched = TASK_END;
  }
  else {
    // try to get lock
    if (Mutex.try_lock(&ADC_DEV->lock, task)) {
      adc_task_data_t *data = task->fdata;
      adc_atmega_change_channel(ADC_DEV->regs, data->channel);

      result.next = TASK_STATE_SAMPLE;
      ADC_DEV->current = task;
    }
  }
  return result;
}

/**
 * adc task callback to start the sampling process
 *
 * @param task task object pased to the callback by the scheduler
 * @return next state and schedule value
 */
static task_slice_result_t adc_state_sample(task_t *task) {
  task_slice_result_t result = { TASK_STATE_RECORD, TASK_END };
  assert(Mutex.have_lock(&ADC_DEV->lock, task) && "Task does not own mutex");
	
  adc_atmega_se_start(ADC_DEV->regs);
	
  return result;
}

/**
 * adc task callback to record the sampled value
 *
 * @param task task object pased to the callback by the scheduler
 * @return next state and schedule value
 */
static task_slice_result_t adc_state_record(task_t *task) {
  task_slice_result_t result = { TASK_STATE_START, TASK_RESCHED };
  assert(Mutex.have_lock(&ADC_DEV->lock, task) && "Task does not own mutex");
  adc_task_data_t *data = task->fdata;
	
  ADC_DEV->data[data->channel] = adc_atmega_se_read(ADC_DEV->regs);
  ADC_DEV->current = NULL;
  if (!task->enabled) result.sched = TASK_END;
  Mutex.unlock(&ADC_DEV->lock, task);
  return result;
}

/**
 * Start sampling an ADC channel
 *
 * @param ch channel
 * @param interval sampling interval in system ticks
 * @return void
 * @note all channels are stopped by default
 */
static void adc_start_channel(adc_channel_t ch, tick_t interval) {
  task_t *task = &adc_tasks[ch];
  Task.set_ticks(task, interval);
  Task.enable(task);
  task->slice_idx = TASK_STATE_START;
  Task.schedule(task, TASK_RESCHED);
}

/**
 * Stop sampling an ADC channel
 *
 * @param ch channel to stop
 * @return void
 * @note the ADC may take one more sample on this channel if it has already
 *       started doing so
 */
static inline void adc_stop_channel(adc_channel_t ch) {
  Task.disable(&adc_tasks[ch]);
}

/**
 * Atomic get most recent value read on ADC channel
 *
 * @param ch channel to get
 * @return adc value 
 */
static inline adc_data_t adc_read_value(adc_channel_t ch) {
  uint16_t result;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    result = ADC_DEV->data[ch];
  }
  return result;
}

/**
 * Initialize adc device, tasks, and all channels
 *
 * @param void
 * @return void
 */
static void adc_init(void) {
  adc_atmega_init(ADC_DEV->regs);
  Mutex.init(&ADC_DEV->lock);
  ADC_DEV->current = NULL;
  int i;
  for (i = 0; i < 8; i++) {
    adc_task_data[i].channel = i;		
    Task.init(&adc_tasks[i], adc_task_slices, &adc_task_data[i]);
    Task.disable(&adc_tasks[i]);
  }
}

ISR(ADC_vect) {
  Task.schedule(ADC_DEV->current, TASK_SCHED_IMMED);
}

/**
 * Public Interface Class for ADC device
 */
const adc_class_t ADC_ = {
  .init = adc_init,
  .start = adc_start_channel,
  .stop = adc_stop_channel,
  .read = adc_read_value
}; 
