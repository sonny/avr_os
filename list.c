/*
 * list.c
 *
 * Created: 10/1/2013 8:08:11 PM
 *  Author: Greg Cook
 */ 

#include <assert.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "list.h"

/**
 * Initialize list 
 *
 * @param head List head
 * @return void
 */
static inline void list_init(list_t *head) {
  assert(head && "List Head is a Null Pointer");
  head->prev = head;
  head->next = head;
}

/**
 * Get size of linked list
 *
 * @param head List head
 * @return size of list
 * @note Not Implemented
 */
static inline int list_size(const list_t *head) {
  assert(0 && "Not implemented");
  return 0;
}

/**
 * Is Empty predicate
 *
 * @param head List head
 * @return true if list is empty
 */
static inline bool list_isEmpty(const list_t *head) {
  assert(head && "List Head is a Null Pointer");
  return (head->next == head);
}

/**
 * Intert node into list
 *
 * For inserting a new node when the next and prev are known
 *
 * @param new Node to insert
 * @param prev Node before new
 * @param next Node to follow new
 * @Note prev and next can be aliased to each other
 *       new should not be, but we aren't going to use restrict
 * @return void
 */
static inline void list_insert(list_t * new, list_t * prev, list_t * next) {
  assert(new && prev && next && "Invalid list pointers");
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

/**
 * Insert node into front of list
 *
 * @param head List head
 * @param new Node to insert
 * @return void
 */
static inline void list_addAtFront(list_t * restrict head, list_t * restrict new) {
  assert(head && new && "List Head is a Null Pointer");
  list_insert(new, head, head->next);
}

/**
 * Insert node into rear of list
 *
 * @param head List head
 * @param new Node to insert
 * @return void
 */
static inline void list_addAtRear(list_t * restrict head, list_t * restrict new) {
  assert(head && new && "List Head is a Null Pointer");
  list_insert(new, head->prev, head);
}

/**
 * Remove node from list
 *
 * @param node Node to remove from list
 * @return void
 * @note This function does not require the node to belong to any particular list. It ensures
 *       that it is removed from any that it might belong to.
 */
static inline void list_remove(list_t *node) {
  assert(node && "List node is a Null Pointer");  
  node->next->prev = node->prev;
  node->prev->next = node->next;
  list_init(node);
}

/**
 * Remove node from list
 *
 * @param head List head that node belongs to
 * @param node Node to remove
 * @return node that was removed
 */
static inline list_t *list_removeNode(list_t * restrict head, list_t * restrict node) {
  assert(head && node && "List Node is a Null Pointer");
  if (list_isEmpty(head)) return NULL;
  list_remove(node);
  return node;
}

/**
 * Remove node from front of list
 *
 * @param head List head
 * @return first node in list
 */
static inline list_t* list_removeFront(list_t *head) {
  assert(head && "List Head is a Null Pointer");
  return list_removeNode(head, head->next);
}

/**
 * Remove node from rear of list
 *
 * @param head List head
 * @return last node in list
 */
static inline list_t* list_removeRear(list_t *head) {
  assert(head && "List Head is a Null Pointer");
  return list_removeNode(head, head->prev);
}

/**
 * Conditionally call function on each element of list 
 *
 * @param head List head
 * @param fnc Function to call on each element if pred is true for that element
 * @param pred Predicate function returns bool for each element
 * @param fdata Data passed to pred and fnc
 * @return void
 */
static inline void list_eachIf(list_t *head, list_iter_callback fnc, list_iter_predicate pred, const void *fdata) {
  list_t *pos, *tmp;
  LIST_FOR_EACH_SAFE(pos, tmp, head) {
    if (!pred || pred(pos, fdata)) // if pred == NULL or pred(node) is true
      fnc(pos, fdata);  // call fnc(node, fdata) on this node
  }
}

/**
 * Call function for each element in list
 *
 * @param head List head
 * @param fnc Function to call on each element
 * @param fdata Data for fnc
 * @return void
 */
static inline void list_each(list_t *head, list_iter_callback fnc, const void *fdata) {
  list_eachIf(head, fnc, NULL, fdata);
}

/**
 * Find the first element in the list for which pred returns true
 *
 * @param head List head
 * @param pred Predicate function which returns for each element
 * @param fdata Data passed to pred
 * @return
 */
static inline list_t* list_find(const list_t *head, list_iter_predicate pred, const void *fdata) {
  list_t *pos, *tmp;
  LIST_FOR_EACH_SAFE(pos, tmp, head) {
    if ((pred)(pos, fdata))
      return pos;        // return the first node that p evals true for
  }
  return NULL;           // return null if we fall through
}

/**
 * Public Interface for List Class
 */
const list_class_t const List = {
  .init = list_init,
  .size = list_size,
  .isEmpty = list_isEmpty,
  .addAtFront = list_addAtFront,
  .addAtRear = list_addAtRear,
  .remove = list_remove,
  .removeFront = list_removeFront,
  .removeRear = list_removeRear,
  .each = list_each,
  .eachIf = list_eachIf,
  .find = list_find
};

