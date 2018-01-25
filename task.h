/*
 * task.h
 *
 * Created: 10/1/2013 8:58:19 PM
 *  Author: Greg Cook
 */ 


#ifndef TASK_H_
#define TASK_H_

#include <stdint.h>
#include "types.h"
#include "heap.h"
#include "list.h"

#define TASK_ALLOC_COUNT 16
#define MAX_QUEUE 32

typedef enum { TASK_END, 
               TASK_WAIT, 
               TASK_SCHED, 
               TASK_RESCHED, 
               TASK_SCHED_IMMED, 
               TASK_ERROR } task_sched_t;

typedef struct task_t task_t;

typedef struct {
  uint8_t next;
  uint8_t sched;
} task_slice_result_t;

typedef task_slice_result_t (*task_slice_callback_fp)(task_t*);

typedef task_sched_t (*task_callback_fp)(task_t*);

struct task_t{
  list_t lnode;
  bool enabled;
  tick_t start_ticks;
  tick_t ticks;
  uint8_t slice_idx;                     // index of next slice to call
  const task_slice_callback_fp * slices; // array of slice callback function pointers
  void *fdata;
};

typedef struct {
  void (* const init)(task_t*, const task_slice_callback_fp * const,void*);
  task_t *(* const new)(const task_slice_callback_fp * const, void*,tick_t,bool);
  void (*delete)(task_t*);
  void (* const set_ticks)(task_t*, tick_t);
  void (* const enable)(task_t*);
  void (* const disable)(task_t*);
  void (* const schedule)(task_t*, task_sched_t);
} task_class_t;

typedef struct {
  void (* const init)(void);
  void (* const enqueue)(task_t*, task_sched_t);
  task_t *(* const dequeue)(void);
  void (* const timer_callback)(void);
  void (* const process_callback)(void);
  void (* const run)(void);
} task_queue_class_t;

void scheduler_init(void);
void scheduler_run(void);

extern const task_class_t const Task;
extern task_queue_class_t TaskQueue;

static inline task_t *task_list_entry(const list_t *lnode) {
  return LIST_ENTRY(lnode, task_t, lnode);
}

static inline list_t *task_list_node(task_t *task) {
  return &task->lnode;
}

#endif /* TASK_H_ */
