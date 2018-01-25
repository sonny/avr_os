/*
 * task_mutex.h
 *
 * Created: 12/4/2013 1:28:00 PM
 *  Author: Greg Cook
 */ 


#ifndef TASK_MUTEX_H_
#define TASK_MUTEX_H_

#include "task.h"
#include "list.h"

typedef struct {
  task_t *owner;
  list_t waiting;
} task_mutex_t;

typedef struct {
  void (* const init)(task_mutex_t*);
  bool (* const try_lock)(task_mutex_t*, task_t*);
  bool (* const lock)(task_mutex_t*, task_t*);
  bool (* const unlock)(task_mutex_t*, task_t*);
  bool (* const have_lock)(const task_mutex_t*, const task_t*);
} task_mutex_class_t;

task_slice_result_t mutex_task_wait(task_t*, uint8_t);

extern const task_mutex_class_t const Mutex;

#endif /* TASK_MUTEX_H_ */
