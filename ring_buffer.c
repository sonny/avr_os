/*
 * ring_bufffer.c
 *
 * Created: 7/8/2013 8:55:45 PM
 *  Author: Greg Cook
 *
 * This implementation maintains an empty slot between end and start,
 * so there are only SIZE-1 buffer slots.
 */ 

#include "ring_buffer.h"

/**
 * Initialize Ring Buffer
 * @param rb Ring Buffer object
 * @return void
 */
static void ringbuffer_init(ring_buffer_t *rb) {
  rb->start = 0;
  rb->end = 0;
}

/**
 * Returns true if Ring Buffer is full
 * @param rb Ring Buffer object
 * @return true if rb is full
 */
static inline bool ringbuffer_isFull(const ring_buffer_t *rb) {
  return (((rb->end + 1) % RING_BUFFER_SIZE) == rb->start);
}

/**
 * Return true if Ring Buffer is empty
 * @param rb Ring Buffer object
 * @return true if rb is empty
 */
static inline bool ringbuffer_isEmpty(const ring_buffer_t *rb) {
  return (rb->start == rb->end);
}

/**
 * Returns number of elements left in Ring Buffer
 * @param rb Ring Buffer object
 * @return count of elements remaining in rb
 */
static inline uint8_t ringbuffer_remainder(const ring_buffer_t *rb) {
  return (rb->start + RING_BUFFER_SIZE - rb->end) % RING_BUFFER_SIZE;
}

/**
 * Returns true if the remainder count is less than RING_BUFFER_TOO_CLOSE
 *
 * This function is used for serial comms to trigger hardware flow control
 * before the buffer overruns
 *
 * @param rb Ring Buffer object
 * @return true if rb remainder is too close
 */
static inline bool ringbuffer_almost_full(const ring_buffer_t *rb) {
  return (ringbuffer_remainder(rb) >= RING_BUFFER_TOO_CLOSE);
}

/**
 * Inserts new element into the Ring Buffer. May block if Ring buffer is full.
 *
 * Blocking depends on some other asynchronous event to take control of the
 * processor and empty the Ring Buffer. Be careful about using this in anger.
 *
 * @param rb Ring Buffer object
 * @param c Element to insert
 * @param block If true, block when Ring Buffer is full, otherwise overwrite
 * @return void
 */
static inline void ringbuffer_insert_element(ring_buffer_t *rb, char c, bool block) {
  while (block && ringbuffer_isFull(rb)) ; // block until ring buffer is not full

  rb->buffer[rb->end] = c;
  // do something convoluted to avoid concurrency problems
  // avoid possibility of ISR erroneously finding buffer empty when full
  if (ringbuffer_isFull(rb))
    rb->start = (rb->start + 1 ) % RING_BUFFER_SIZE;

  rb->end = (rb->end + 1 ) % RING_BUFFER_SIZE;
}

/**
 * Copy entire string into Ring buffer
 * Not actually used anywhere, as far as I know
 * @param rb Ring Buffer object
 * @param s String to copy
 * @return void
 */
static void ringbuffer_insert_string(ring_buffer_t * restrict rb, const char * restrict s) {
  const char *p = s;
  while (*p) {
    ringbuffer_insert_element(rb, *p, true);
    p++;
  }
}

/**
 * Remove next element from Ring Buffer
 * @param rb Ring Buffer object
 * @return next element from ring buffer
 * @Note This does not account for the possibility that the Ring Buffer may be empty.
 *       Check First.
 */
static char ringbuffer_extract_element(ring_buffer_t *rb) {
  char c = rb->buffer[rb->start];
  rb->start = (rb->start + 1 ) % RING_BUFFER_SIZE;
  return c;	
}

/**
 * Calculates the used size of the ring buffer. 
 *
 * This is the inverse operation of remainder. That is 
 * Size + Remainder = RING_BUFFER_SIZE.
 *
 * @param rb Ring Buffer object
 * @return number of elements occupied in rb
 */
static uint8_t ringbuffer_size(const ring_buffer_t *rb) {
  return (RING_BUFFER_SIZE + rb->end - rb->start) % RING_BUFFER_SIZE;
}

const ringbuffer_class_t const Ringbuffer = {
  .init = ringbuffer_init,
  .full = ringbuffer_isFull,
  .empty = ringbuffer_isEmpty,
  .almost_full = ringbuffer_almost_full,
  .insert = ringbuffer_insert_element,
  .insert_string = ringbuffer_insert_string,
  .remove = ringbuffer_extract_element,
  .size = ringbuffer_size
};
