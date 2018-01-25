/*
 * heap.h
 *
 * Created: 10/15/2013 7:41:01 PM
 *  Author: Greg Cook
 *
 * heap data structure
 */ 


#ifndef HEAP_H_
#define HEAP_H_

#include <stdbool.h>

#define STATIC_HEAP_SIZE 8

typedef uint32_t heap_key_t;
typedef int8_t heap_index_t;

typedef enum { UNKNOWN, HEAP_MAX, HEAP_MIN } heap_type_t;

typedef struct heap_t heap_t;

typedef heap_key_t (*heap_get_key_fp)(const void*);
typedef bool (*heap_cmp_fp)(const heap_t *, heap_index_t, heap_index_t);

struct heap_t {
  int size;
  int max_size;
  heap_cmp_fp cmp;
  heap_get_key_fp get_key;
  void **data;
};

typedef struct static_heap_t {
  heap_t _structure;
  void  *_data[STATIC_HEAP_SIZE];
} static_heap_t;

typedef struct {
  heap_t *(* const static_init)(static_heap_t*, heap_type_t, heap_get_key_fp);
  void    (* const init)(heap_t*, heap_type_t, int, void*, heap_get_key_fp);
  bool    (* const insert)(heap_t*, void*);
  void   *(* const remove_head)(heap_t*);
  void   *(* const head)(const heap_t*);
  bool    (* const is_empty)(const heap_t*);
  bool    (* const is_full)(const heap_t*);
} heap_class_t;

extern heap_class_t Heap;

#endif /* HEAP_H_ */
