/*
 * heap.c
 *
 * Created: 10/15/2013 7:41:35 PM
 *  Author: Greg Cook
 *
 * Heap data structure
 */ 

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heap.h"

/**
 * Validation function
 * @param heap Heap opject
 * @return True if heap is a valid structure
 */
static bool heap_valid(const heap_t *heap);

/**
 * Get parent node of current node
 * @param index Index of current node in internal array
 * @return index of parent node in internal array
 */
static inline heap_index_t heap_node_parent(heap_index_t index) {
  return (index - 1) / 2;
}

/**
 * Get left child of current node
 * @param index Index of current node in internal array
 * @return index of left child node in internal array
 */
static inline heap_index_t heap_node_left_child(heap_index_t index) {
  return index * 2 + 1;
}

/**
 * Get right child of current node
 * @param index Index of current node in internal array
 * @return index of right child node in internal array
 */
static inline heap_index_t heap_node_right_child(heap_index_t index) {
  return index * 2 + 2;
}

/**
 * Get key value from heap data
 * @param heap Heap opject
 * @param index Index of node
 * @return key value from data object contained by node
 */
static inline heap_key_t heap_get_key(const heap_t *heap, heap_index_t index) {
  return ( heap->get_key(heap->data[index]) );
}

/**
 * Compare two nodes using max heap compare strategy
 * @param heap Heap opject
 * @param a Index of node a
 * @param b Index of node b
 * @return True if key(a) > key(b)
 */
static inline bool heap_cmp_max(const heap_t *heap, heap_index_t a, heap_index_t b) {
  return ( heap_get_key(heap, a) > heap_get_key(heap, b) );
}

/**
 * Compare two nodes using min heap compare strategy
 * @param heap Heap opject
 * @param a Index of node a
 * @param b Index of node b
 * @return True if key(a) < key(b)
 */
static inline bool heap_cmp_min(const heap_t *heap, heap_index_t a, heap_index_t b) {
  return ( heap_get_key(heap, a) < heap_get_key(heap, b) );
}

/**
 * Swap two nodes in a heap
 * @param heap Heap opject
 * @param a Index of node a
 * @param b Index of node b
 * @return void
 */
static inline void heap_swap(heap_t *heap, heap_index_t a, heap_index_t b) {
  void *temp = heap->data[a];
  heap->data[a] = heap->data[b];
  heap->data[b] = temp;
}

/**
 * Heapify up implementation
 * @param heap Heap opject
 * @param index Index of node to move up
 * @return void
 */
static void heap_heapify_up(heap_t *heap, heap_index_t index) {
  heap_index_t current = index;
  heap_index_t parent = heap_node_parent(current);
  while (current > 0 && !(heap->cmp(heap, parent, current))) {
    heap_swap(heap, parent, current);

    current = parent;
    parent = heap_node_parent(current);
  }
}

/**
 * Heapify node in heap
 * @param heap Heap opject
 * @param index Index of node to heapify
 * @return void
 */
static void heap_heapify(heap_t *heap, heap_index_t index) {
  heap_index_t left  = heap_node_left_child(index);
  heap_index_t right = heap_node_right_child(index);
  heap_index_t minmax = index;

  if (left < heap->size && heap->cmp(heap, left, index))
    minmax = left;
  else
    minmax = index;

  if (right < heap->size && heap->cmp(heap, right, minmax))
    minmax = right;

  if (minmax != index) {
    heap_swap(heap, index, minmax);
    heap_heapify(heap, minmax);
  }
}

/**
 * Get heap head
 * @param heap Heap opject
 * @return Head node of heap
 */
static inline void *heap_head(const heap_t *heap) {
  return heap->data[0];
}

/**
 * Determine if Heap is empty
 * @param heap Heap opject
 * @return True if heap has no elements
 */
static inline bool heap_is_empty(const heap_t *heap) {
  return (heap->size == 0);
}

/**
 * Determine if heap is full
 * @param heap Heap opject
 * @return True if no more elements can be inserted
 */
static inline bool heap_is_full(const heap_t *heap) {
  return (heap->size == heap->max_size);
}

/**
 * Initialize heap structure
 * @param heap Heap opject
 * @param type Type of heap, one of (HEAP_MAX, HEAP_MIN)
 * @param data_size Number of elements in data
 * @param data Heap storage array (you need to allocate this and pass it in)
 * @param get_key Function that returns key from a data element
 * @return void
 */
static void heap_init(heap_t *heap, heap_type_t type, int data_size, void *data, heap_get_key_fp get_key) {	
  heap->size = 0;
  heap->max_size = data_size;
  heap->data = data;

  if (type == HEAP_MAX)
    heap->cmp = heap_cmp_max;
  else
    heap->cmp = heap_cmp_min;
	
  heap->get_key = get_key;

  int i;
  for (i = 0; i < data_size; i++)
    heap->data[i] = NULL;

  assert(heap_valid(heap) && "Heap invalid");
}

/**
 * Statically define the data array in a data structure
 * @note this is dumb...don't use this, but its the way I wrote it
 * initialially and it is still used somewhere.
 */
static heap_t *heap_static_init(static_heap_t *st_heap, heap_type_t type, heap_get_key_fp get_key) {
  heap_t *structure = &st_heap->_structure;
  void *data = &st_heap->_data;
  heap_init(structure, type, STATIC_HEAP_SIZE, data, get_key);
  return structure;
}

/**
 * Insert new element into heap
 * @param heap Heap opject
 * @param new Object to insert
 * @return True if object was inserted, false if heap was full
 */
static bool heap_insert(heap_t * restrict heap, void * restrict new) {
  if (!heap_is_full(heap)) {
    int index = heap->size;
    heap->size++;
    heap->data[index] = new;
    heap_heapify_up(heap, index);

    assert(heap_valid(heap) && "Heap invalid");

    return true;
  }
  return false;
}

/**
 * Remove and return object from head of heap
 * @param heap Heap opject
 * @return Object from head of heap, or NULL if heap is empty
 */
static void *heap_remove_head(heap_t *heap) {
  void *result = NULL;
  if (!heap_is_empty(heap)) {
    result = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1]; // last element
    heap->data[heap->size - 1] = NULL;
    heap->size--;
    heap_heapify(heap, 0);
  }

  assert(heap_valid(heap) && "Heap invalid");
  return result;
}

/**
 * Heap class interface
 */
heap_class_t Heap = {
  .static_init = heap_static_init,
  .init = heap_init,
  .insert = heap_insert,
  .remove_head = heap_remove_head,
  .head = heap_head,
  .is_empty = heap_is_empty,
  .is_full = heap_is_full
};

/************************************************************************/
/*  Debug/Validation Code                                               */
/************************************************************************/

/**
 * Does the heap maintain the max heap property
 * @param heap Heap opject
 * @param index Index of node
 * @return true if max heap property holds for node
 */
static inline bool heap_max_heap_property(const heap_t *heap, int index) {
  int parent = heap_node_parent(index);
  return (heap_get_key(heap, parent) >= heap_get_key(heap, index));
}

/**
 * Does the heap maintain the min heap property
 * @param heap Heap opject
 * @param index Index of node
 * @return true if max heap property holds for node
 */
static inline bool heap_min_heap_property(const heap_t *heap, int index) {
  int parent = heap_node_parent(index);
  return (heap_get_key(heap, parent) <= heap_get_key(heap, index));
}

/**
 * Is the heap valid
 * @param heap Heap opject
 * @return True if the heap is valid
 */
static bool heap_valid(const heap_t *heap) {
  bool error = false;

  // heap is not null
  error |= (heap == NULL);
  assert(!error && "heap ptr is null");

  // cannot continue if error
  if (error) return !error;

  // size >= 0
  error |= (heap->size < 0);
  assert(!error && "heap size is negative");

  // size <= max_size
  error |= (heap->size > heap->max_size);
  assert(!error && "heap size is larger than max size");
	
  // cmp is not null
  error |= (heap->cmp == NULL);
  assert(!error && "heap cmp function ptr is null");

  // get_key is not null
  error |= (heap->get_key == NULL);
  assert(!error && "heap get_key function ptr is null");

  int i;
  for (i = 0; i < heap->max_size; i++) {
    if (i < heap->size) {
      error |= (heap->data[i] == NULL);
      assert(!error && "valid data space is NULL");
    }
    else {
      // data entry must be NULL
      error |= (heap->data[i] != NULL);
      assert(!error && "empty data space not NULL");
    }
  }

  // cannot continue if error
  if (error) return !error;

  // determine heap type
  bool (*heap_property)(const heap_t*, int);
	
  if (heap->cmp == heap_cmp_max)
    heap_property = heap_max_heap_property;
  else
    heap_property = heap_min_heap_property;

  for (i = 1; i < heap->size; i++) {
    error |= (!heap_property(heap, i));
    assert(!error && "heap property not maintained");
  }

  return !error;
}

/**
 * Generic heap dispaly function
 * @param heap Heap opject
 * @return void
 */
static void heap_display(const heap_t *heap) {
  int i;
  for (i = 0; i < heap->max_size; i++) {
    if (i < heap->size) {
      printf("%d:%d ", i, (int)(heap->get_key(heap->data[i])));
    }
    else {
      printf("%d:%p ", i, heap->data[i]);
    }
  }
  printf("\n");
}
