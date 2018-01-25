/*
 * task_mutex.c
 *
 * Created: 12/4/2013 1:28:27 PM
 *  Author: Greg Cook
 *
 * Implement mutexes for tasks
 */ 

#include "task_mutex.h"
#include <util/atomic.h>

/**
 * Predicate for use in list find method. Returns true if current node
 * is equal to the original passed in node.
 * @param lnode List node element of a Task object. This is the current 
 * iterated element in the list used by find or for-each.
 * @param v Task node used for comparison
 * @return True if lnode.task == v.task
 */
static bool task_list_cmp(const list_t *lnode, const void *v) {
  const task_t* task = v;
  return (task_list_entry(lnode) == task);
}

/**
 * Iterates through linked list to determine if task is a member
 * @param list Linked list to iterate
 * @param task Task object to check for in list
 * @return True if task is a member of list
 */
static bool task_in_list(const list_t *list, const task_t *task) {
  list_t *lnode = List.find(list, task_list_cmp, task);
  return (lnode != NULL);
}

/**
 * Adds task to mutex's wait list
 * @param mutex Mutex object to wait on
 * @param task Task to add to wait list
 * @return void
 */
static void task_mutex_wait(task_mutex_t *mutex, task_t *task) {
  List.addAtRear(&mutex->waiting, task_list_node(task));
}

/**
 * Notify all waiting mutexes. 
 *
 * In practice, this means that all waiting tasks get rescheduled in the IMMED queue.
 *
 * @param mutex Mutex with waiting tasks
 * @return void
 */
static void task_mutex_notify(task_mutex_t *mutex) {
  list_t *lnode;
  // resched all tasks waiting on this mutex -- see who gets it
  while((lnode = List.removeFront(&mutex->waiting))) {
    Task.schedule(task_list_entry(lnode), TASK_SCHED_IMMED);
  }
}

/**
 * Initialize mutex
 * @param mutex Mutex object
 * @return void
 */
static void mutex_init(task_mutex_t *mutex) {
  mutex->owner = NULL;
  List.init(&mutex->waiting);
}

/**
 * Get lock if it is available 
 * @param mutex Mutex object that task is attempting to lock
 * @param task Task object attempting to get lock
 * @return True if lock is obtained
 */
static bool mutex_try_lock(task_mutex_t *mutex, task_t *task) {
  bool result = false;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if ( mutex->owner == NULL ||
         mutex->owner == task ) {
      mutex->owner = task;
      result = true;
    }
  }
  return result;
}

/**
 * Get lock if it is available, otherwise wait on mutex
 * @param mutex Mutex object that task is attempting to lock
 * @param task Task object attempting to get lock
 * @return True if lock is obtained
 */
static bool mutex_lock(task_mutex_t *mutex, task_t *task) {
  bool result = true;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    result = mutex_try_lock(mutex, task);
    if (!result) {
      task_mutex_wait(mutex, task);
    }
  }
  return result;
}

/**
 * Release lock and notify any waiting tasks
 * @param mutex Mutex that is locked
 * @param task Task that holds lock
 * @return True if unlock is successful
 */
static bool mutex_unlock(task_mutex_t *mutex, task_t *task) {
  bool result = false;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if ( mutex->owner == task ||
         mutex->owner == NULL ) {
      mutex->owner = NULL;
      result = true;
      task_mutex_notify(mutex);
    }
  }
  return result;
}

/**
 * Check to see if task has a lock on a mutex
 * @param mutex Mutex object to check
 * @param task Task objet to check
 * @return True if task owns lock on mutex
 */
static inline bool mutex_have_lock(const task_mutex_t *mutex, const task_t *task) {
  bool result = false;
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    result = ( mutex->owner == task );
  }
  return result;
}

/**
 * Public interface to Mutex class
 */
const task_mutex_class_t const Mutex = {
  .init = mutex_init,
  .try_lock = mutex_try_lock,
  .lock = mutex_lock,
  .unlock = mutex_unlock,
  .have_lock = mutex_have_lock
};	
