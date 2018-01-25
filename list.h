/*
 * list.h
 *
 * Created: 10/1/2013 8:08:39 PM
 *  Author: Greg Cook
 */ 


#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct list_t list_t;
struct list_t {
  list_t *prev;
  list_t *next;
};

typedef void (*list_iter_callback)(const list_t*, const void*);
typedef bool (*list_iter_predicate)(const list_t*, const void*);

typedef struct {
  void (*const init)(list_t*);
  int  (*const size)(const list_t*);
  bool (*const isEmpty)(const list_t*);
  void (*const addAtFront)(list_t*, list_t*);
  void (*const addAtRear)(list_t*, list_t*);
  void (*const remove)(list_t*);
  list_t* (*const removeFront)(list_t*);
  list_t* (*const removeRear)(list_t*);
  void (*const each)(list_t*, list_iter_callback, const void*);
  list_t* (*const find)(const list_t*, list_iter_predicate, const void*);
  void (*const eachIf)(list_t*, list_iter_callback, list_iter_predicate, const void*);
} list_class_t;

extern const list_class_t const List;


#define LIST_FOR_EACH_SAFE(pos, tmp, head)                      \
  for (pos = (head)->next, tmp = pos->next; pos != (head);      \
       pos = tmp, tmp = pos->next)

#define LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)   \

/**
 * This macro is via the linux kernal and Greg Kroah-Hartman. See:
 * http://www.kroah.com/log/linux/container_of.html
 */
#define CONTAINER_OF(ptr, type, member) ({                      \
      const typeof( ((type *)0)->member ) *__mptr = (ptr);      \
      (type *)( (char *)__mptr - offsetof(type,member) );})


#endif /* LIST_H_ */
