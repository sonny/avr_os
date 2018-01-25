/*
 * producer_consumer_demo.c
 *
 * Created: 12/6/2013 2:02:15 PM
 *  Author: Greg Cook
 *
 * Demo of the producer consumer problem using tasks/mutexes
 */ 

#include "task.h"
#include "task_mutex.h"
#include "ring_buffer.h"
#include "vt100.h"
#include "producer_consumer_demo.h"

typedef struct {
  uint8_t index;
  uint8_t value;
} pc_data_t;

static ring_buffer_t pc_buffer;
static task_mutex_t pc_mutex;

static task_t *pc_producer0_task;
static task_t *pc_producer1_task;

static task_t *pc_consumer0_task;
static task_t *pc_consumer1_task;
static task_t *pc_consumer2_task;

static task_t *pc_display_task;

static pc_data_t pc_producer0_task_data;
static pc_data_t pc_producer1_task_data;

static pc_data_t pc_consumer0_task_data = {0,0};
static pc_data_t pc_consumer1_task_data = {0,0};
static pc_data_t pc_consumer2_task_data = {0,0};

#define PC_PRODUCER0_TICKS 125
#define PC_PRODUCER1_TICKS 1000

#define PC_CONSUMER0_TICKS 250
#define PC_CONSUMER1_TICKS 750
#define PC_CONSUMER2_TICKS 5000

/************************************************************************
 Function prototypes for task slices
************************************************************************/
static task_slice_result_t producer_init(task_t * task);
static task_slice_result_t producer_produce(task_t * task);
static task_slice_result_t producer_get_control(task_t * task);
static task_slice_result_t producer_finalize(task_t * task);

static task_slice_result_t consumer_init(task_t * task);
static task_slice_result_t consumer_get_control(task_t * task);
static task_slice_result_t consumer_finalize(task_t * task);
static task_slice_result_t consumer_finalize_variable(task_t * task);
static task_slice_result_t consumer_consume(task_t * task);

/************************************************************************
 Task Slice Initialization for producer and consumers
************************************************************************/
static const task_slice_callback_fp const pc_producer_task_slices[] = {
  producer_init,
  producer_produce,
  producer_get_control,
  producer_finalize
};

static const task_slice_callback_fp const pc_consumer_task_slices[] = {
  consumer_init,
  consumer_get_control,
  consumer_finalize,
  consumer_consume
};

// NOTE: consumer2 has a slightly different set of callbacks
static const task_slice_callback_fp const pc_consumer2_task_slices[] = {
  consumer_init,
  consumer_get_control,
  consumer_finalize_variable, // <-- different finalization function
  consumer_consume
};


static task_slice_result_t pc_display(task_t *task);
static const task_slice_callback_fp const pc_display_task_slices[] = {
  pc_display
};

/**
 * Initailize producer_consumer demo
 */ 
void producer_consumer_init(void) {
  Ringbuffer.init(&pc_buffer);
  Mutex.init(&pc_mutex);

  // Note: It is effectively no extra work to add an extra producer
  pc_producer0_task = Task.new(pc_producer_task_slices, &pc_producer0_task_data, PC_PRODUCER0_TICKS, true);
  pc_producer1_task = Task.new(pc_producer_task_slices, &pc_producer1_task_data, PC_PRODUCER1_TICKS, true);

  // Allocate 3 consumers
  pc_consumer0_task = Task.new(pc_consumer_task_slices, &pc_consumer0_task_data, PC_CONSUMER0_TICKS, true);
  pc_consumer1_task = Task.new(pc_consumer_task_slices, &pc_consumer1_task_data, PC_CONSUMER1_TICKS, true);
  pc_consumer2_task = Task.new(pc_consumer2_task_slices, &pc_consumer2_task_data, PC_CONSUMER2_TICKS, true);

  // Schedule 5 tasks
  Task.schedule(pc_producer0_task, TASK_RESCHED);
  Task.schedule(pc_producer1_task, TASK_RESCHED);

  Task.schedule(pc_consumer0_task, TASK_RESCHED);
  Task.schedule(pc_consumer1_task, TASK_RESCHED);
  Task.schedule(pc_consumer2_task, TASK_RESCHED);

  pc_display_task = Task.new(pc_display_task_slices, NULL, 500, true);
  Task.schedule(pc_display_task, TASK_RESCHED);

  // Ultimately, this demo sets up 6 tasks:
  // 2 producer tasks
  // 3 consumer tasks
  // 1 display tasks
  // ALL of these must coordinate the resources in this file
}

/************************************************************************/
/* Producer Task Slice Functions                                        */
/************************************************************************/
#define PC_STATE_PRODUCER_INIT     0
#define PC_STATE_PRODUCER_PRODUCE  1
#define PC_STATE_PRODUCER_CONTROL  2
#define PC_STATE_PRODUCER_FINALIZE 3


static task_slice_result_t producer_init(task_t * task) {
  pc_data_t *data = task->fdata;
  data->index = 255; // effectively -1
  data->value = 255; // initialize value to roll over
  task_slice_result_t result = { PC_STATE_PRODUCER_PRODUCE, TASK_SCHED_IMMED };
  return result;
}

static task_slice_result_t producer_produce(task_t * task) {
  pc_data_t *data = task->fdata;
  data->index++;
  data->value++;

  task_slice_result_t result = { PC_STATE_PRODUCER_CONTROL, TASK_SCHED_IMMED };
  return result;
}

static task_slice_result_t producer_get_control(task_t * task) {
  // the default is to assume that we do not get the lock
  // and that we will be waiting to get rescheduled by the notifier
  task_slice_result_t result = { PC_STATE_PRODUCER_CONTROL, TASK_WAIT };
  if (Mutex.lock(&pc_mutex, task)) {
    result.next = PC_STATE_PRODUCER_FINALIZE;
    result.sched = TASK_SCHED_IMMED;
  }
  return result;
}

static task_slice_result_t producer_finalize(task_t * task) {
  // default result is failure to add new value to queue
  task_slice_result_t result = { PC_STATE_PRODUCER_CONTROL, TASK_SCHED_IMMED };

  if (!Ringbuffer.full(&pc_buffer)) {
    pc_data_t *data = task->fdata;
    Ringbuffer.insert(&pc_buffer, data->value, false);
    result.next = PC_STATE_PRODUCER_PRODUCE;
    result.sched = TASK_RESCHED;
  }

  // assume that we have control of the lock at this point
  Mutex.unlock(&pc_mutex, task);
  return result;
}

/************************************************************************/
/* Consumer Task Slice Functions                                        */
/************************************************************************/
#define PC_STATE_CONSUMER_INIT     0
#define PC_STATE_CONSUMER_CONTROL  1
#define PC_STATE_CONSUMER_FINALIZE 2
#define PC_STATE_CONSUMER_CONSUME  3

static task_slice_result_t consumer_init(task_t * task) {
  pc_data_t *data = task->fdata;
  data->index = 255;
	
  task_slice_result_t result = { PC_STATE_CONSUMER_CONTROL, TASK_SCHED_IMMED };
  return result;
}

static task_slice_result_t consumer_get_control(task_t * task) {
  // the default is to assume that we do not get the lock
  // and that we will be waiting to get rescheduled by the notifier
  task_slice_result_t result = { PC_STATE_CONSUMER_CONTROL, TASK_WAIT };
  if (Mutex.lock(&pc_mutex, task)) {
    result.next = PC_STATE_CONSUMER_FINALIZE;
    result.sched = TASK_SCHED_IMMED;
  }
  return result;	
}

static task_slice_result_t consumer_finalize(task_t * task) {
  // default result is failure to add new value to queue
  task_slice_result_t result = { PC_STATE_CONSUMER_CONTROL, TASK_SCHED_IMMED };

  if (!Ringbuffer.empty(&pc_buffer)) {
    pc_data_t *data = task->fdata;
    data->value = Ringbuffer.remove(&pc_buffer);
    data->index++;
    result.next = PC_STATE_CONSUMER_CONSUME;
    result.sched = TASK_SCHED_IMMED;
  }

  // assume that we have control of the lock at this point
  Mutex.unlock(&pc_mutex, task);
  return result;
}

static task_slice_result_t consumer_finalize_variable(task_t * task) {
  // default result is failure to add new value to queue
  task_slice_result_t result = { PC_STATE_CONSUMER_CONTROL, TASK_SCHED_IMMED };

  if (!Ringbuffer.empty(&pc_buffer)) {
    if (Ringbuffer.size(&pc_buffer) >= RING_BUFFER_SIZE - 2) {
      Task.set_ticks(task, 75);
    }
    else if (Ringbuffer.size(&pc_buffer) < RING_BUFFER_SIZE / 2) {
      Task.set_ticks(task, PC_CONSUMER2_TICKS);
    }

    pc_data_t *data = task->fdata;
    data->value = Ringbuffer.remove(&pc_buffer);
    data->index++;
		
		
    result.next = PC_STATE_CONSUMER_CONSUME;
    result.sched = TASK_SCHED_IMMED;
  }

  // assume that we have control of the lock at this point
  Mutex.unlock(&pc_mutex, task);
  return result;
}

// NOTE: The Consume task is largely a placeholder here to show that
// this is where you might DO SOME WORK on the retrieved value without
// having to worry about coordinating it with the other tasks. But I don't
// have anything to do, so I just use a placeholder.
static task_slice_result_t consumer_consume(task_t * task) {
  task_slice_result_t result = { PC_STATE_CONSUMER_CONTROL, TASK_RESCHED };
  return result;
}

static task_slice_result_t pc_display(task_t *task) {
  task_slice_result_t result = { 0, TASK_RESCHED };
  int rb_size = Ringbuffer.size(&pc_buffer);
	
  term_display_region(TERM0, 0, 0, "Producers       idx  val");
  term_display_region(TERM0, 0, 1, "Producer 0 -- [%3d : %3d]", pc_producer0_task_data.index, pc_producer0_task_data.value);
  term_display_region(TERM0, 0, 2, "Producer 1 -- [%3d : %3d]", pc_producer1_task_data.index, pc_producer1_task_data.value);

  term_display_region(TERM0, 0, 3, "Shared Queue Size [%3d/%3d]", rb_size, RING_BUFFER_SIZE-1);
	
  term_display_region(TERM0, 2, 0, "Consumers      idx   val");
  term_display_region(TERM0, 2, 1, "Consumer 0 -- [%3d : %3d]", pc_consumer0_task_data.index, pc_consumer0_task_data.value);
  term_display_region(TERM0, 2, 2, "Consumer 1 -- [%3d : %3d]", pc_consumer1_task_data.index, pc_consumer1_task_data.value);
  term_display_region(TERM0, 2, 3, "Consumer 2 -- [%3d : %3d]", pc_consumer2_task_data.index, pc_consumer2_task_data.value);

  return result;
}
