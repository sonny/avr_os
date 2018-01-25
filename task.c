/*
 * task.c
 *
 * Created: 10/1/2013 9:08:50 PM
 *  Author: Greg Cook
 */ 

#include <stdlib.h>
#include <util/atomic.h>
#include "task.h"
#include "systick.h"

static task_t * task_timer_array[MAX_QUEUE];
static heap_t task_timer_queue;

static list_t task_process_queue;

static list_t task_dynamic_free;
static task_t task_dynamic_array[TASK_ALLOC_COUNT];
static const uint8_t task_alloc_count = TASK_ALLOC_COUNT;

/************************************************************************/
/* Task/TaskQueue Support Functions                                     */
/************************************************************************/

/**
 * Get the key that the timer queue uses for sorting the heap
 *
 * @param v Data element passed into the heap
 * @return heap key
 */
static inline heap_key_t task_timer_get_key(const void *v) {
  return ((task_t*)v)->ticks;
}

/************************************************************************/
/* Task Interface Functions                                             */
/************************************************************************/

/**
 * Initialize task object
 *
 * @param task Task object to initialize
 * @param callback Array of task slice callback functions
 * @param fdata Data for callback functions to use
 * @return void
 * @note a pointer to the task object is passed to the callback function
 *       and the task object has a pointer to fdata if any is set
 */
static void task_init(task_t *task, const task_slice_callback_fp * const callback, void *fdata) {
  List.init(task_list_node(task));
  task->slices = callback;
  task->fdata = fdata;
  task->enabled = true;
  task->slice_idx = 0;
}

/**
 * Allocate task structure from "dynamic" list
 * 
 * @param void
 * @return empty task struct or NULL if there are none available
 * @note There are a limited number of pre-allocated elements available and it is up to the user
 *       to be aware
 */
static task_t *task_allocate(void) {
  task_t *result = NULL;
  if (!List.isEmpty(&task_dynamic_free)) {
    list_t *lnode = List.removeFront(&task_dynamic_free);
    result = task_list_entry(lnode);
  }
  return result;
}

/**
 * Free a task "dynamically" back to the system for re-allocation
 * 
 * @param task The task to be freed
 * @return void
 * @note It is possible, although unintended to use this interface with statically allocated
 *       nodes.
 */
static void task_delete(task_t *task) {
  list_t *lnode = task_list_node(task);
  List.init(lnode); // make sure task is not currently hanging out in some other list

  // wipe data in task structure
  task->enabled = false;
  task->slices = NULL;
  task->fdata = NULL;
  task->start_ticks = 0;
  task->ticks = 0;
	
  List.addAtRear(&task_dynamic_free, lnode);
}

/**
 * Create a new task, allocating storage from from the pre-allocated storage
 *
 * @param callback Array of task slice callback functions
 * @param fdata Data for callbacks to use
 * @param start_ticks Periodic scheduling interval
 * @param en Enable
 * @return task object or NULL if no storage is available
 */
static task_t *task_new(const task_slice_callback_fp * const callback, void *fdata, tick_t start_ticks, bool en) {
  task_t *t = task_allocate();
  if (t) {
    task_init(t, callback, fdata);
    t->start_ticks = start_ticks;
    t->ticks = 0;
    t->enabled = en;
  }
  return t;
}

/**
 * Set periodic scheduling interval
 *
 * The task timer will count down one tick each systick until it reaches zero
 * and then dispatch the task
 * @param t task object pointer
 * @param ticks tick count
 * @return void
 */
static inline void task_set_ticks(task_t *t, tick_t ticks) {
  t->start_ticks = ticks;
}

/**
 * Enable task
 * @param t Task to enable
 * @return void
 */
static inline void task_enable(task_t *t) { t->enabled = true; }

/**
 * Disable task
 * @param t Task to disable
 * @return void
 */
static inline void task_disable(task_t *t) { t->enabled = false; }

/**
 * Schedule task
 * @param t Task to schedule
 * @return void

 */
static inline void task_schedule(task_t *task, task_sched_t sched) {
  TaskQueue.enqueue(task, sched);
}

/**
 * Public Interface for Task Class
 */
const task_class_t const Task = {
  .init = task_init,
  .new = task_new,
  .delete = task_delete,
  .set_ticks = task_set_ticks,
  .enable = task_enable,
  .disable = task_disable,
  .schedule = task_schedule
};

/************************************************************************/
/* Task Queue Interface Functions                                       */
/************************************************************************/

/**
 * Initialize pre-allocated task objects 
 * @param void
 * @return void
 */
static inline void task_queue_init(void) {
  static uint8_t task_initialized = 0;
  if (task_initialized) return;
	
  int8_t i;
  for (i = 0; i < task_alloc_count; ++i) {
    list_t *lnode = task_list_node(&task_dynamic_array[i]);
    List.addAtRear(&task_dynamic_free, lnode);
  }
}

/**
 * Schedule task
 *
 * A task is either scheduled immediately, or on the timer queue
 *
 * @param t Task to schedule
 * @param sched Scheduling algorithm to use
 */
static inline void task_queue_enqueue(task_t *t, task_sched_t sched) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if (sched == TASK_SCHED_IMMED) 
      List.addAtRear(&task_process_queue, task_list_node(t));
    else {
      t->ticks = t->start_ticks + systick_get();
      Heap.insert(&task_timer_queue, t);
    }
  }
}

/**
 * Get the next task atomically from the immediate queue
 *
 * @param void
 * @retval next task to run or NULL if none are available
 */
static inline task_t *task_queue_dequeue(void) {
  task_t *result = NULL;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    result = task_list_entry(List.removeFront(&task_process_queue));
  }
  return result;
}

/**
 * Callback for use in the systick ISR
 *
 * This function decrements tick counts and moves tasks from the timer queue
 * to the process queue when appropriate.
 * @param void
 * @return void
 */
static void task_queue_timer_callback(void) {
  task_t *task = Heap.head(&task_timer_queue);
  tick_t systicks = systick_get();
  while (task && (systicks > task->ticks)) {
    task = Heap.remove_head(&task_timer_queue);

    List.addAtRear(&task_process_queue, task_list_node(task));
    task = Heap.head(&task_timer_queue);
  }
}

/**
 * Dispatch the next task from the scheduler and reschedule it
 *
 * This is the main callback handler for the task scheduler
 *
 * @param void
 * @return void
 */
static void task_queue_process_callback(void) {
  task_t *next_task = TaskQueue.dequeue();
  if (next_task) {
    task_slice_result_t result = {0, TASK_END};

    if (next_task->enabled) {
      NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE) {
        result = next_task->slices[next_task->slice_idx](next_task);
      }
      next_task->slice_idx = result.next;
    }
    switch(result.sched) {
    case TASK_SCHED_IMMED: 
      /* fallthrough */
    case TASK_RESCHED:
      TaskQueue.enqueue(next_task, result.sched);
      break;
    case TASK_SCHED: // Not a valid return value, use RESCHED
      /* fallthrough */
    case TASK_END:
      /* fallthrough */
    case TASK_WAIT:
      break;
    default:
      // error of some sort
      break;
    }
  }
}

/**
 * Public Interface for the TaskQueue Class
 */
task_queue_class_t TaskQueue = {
  .init = task_queue_init,
  .enqueue = task_queue_enqueue,
  .dequeue = task_queue_dequeue,
  .timer_callback = task_queue_timer_callback,
  .process_callback = task_queue_process_callback
};

/**
 * Initialize the scheduler queues
 * @param void
 * @return void 
 */
void scheduler_init(void) {
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    Heap.init(&task_timer_queue, HEAP_MIN, MAX_QUEUE, task_timer_array, task_timer_get_key);

    List.init(&task_process_queue);
    List.init(&task_dynamic_free);
  }
  task_queue_init();
}

/**
 * Scheduler main event loop
 * @param void
 * @return never returns
 */
void scheduler_run(void) {
  sei();
  while(true) {
    task_queue_process_callback();
  }
}

