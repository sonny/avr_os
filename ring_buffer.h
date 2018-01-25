/*
 * ring_buffer.h
 *
 * Created: 7/8/2013 8:51:41 PM
 *  Author: Greg Cook
 */ 

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdbool.h>
#include <stdint.h>

#define RING_BUFFER_SIZE ((uint8_t)128)
#define RING_BUFFER_TOO_CLOSE ((uint8_t)16)

// Careful there, optimizing cowboy....
// If you use any bigger size, you are
// going to have troubles unless you use bigger
// data types.
typedef struct {
  uint8_t start;
  uint8_t end;
  uint8_t buffer[RING_BUFFER_SIZE];
} ring_buffer_t;

typedef struct {
  void (* const init)(ring_buffer_t*);
  bool (* const full)(const ring_buffer_t*);
  bool (* const empty)(const ring_buffer_t*);
  bool (* const almost_full)(const ring_buffer_t*);
  void (* const insert)(ring_buffer_t*, char, bool);
  void (* const insert_string)(ring_buffer_t*, const char*);
  char (* const remove)(ring_buffer_t*);
  uint8_t (* const size)(const ring_buffer_t *);
} ringbuffer_class_t;

extern const ringbuffer_class_t const Ringbuffer;

#endif /* RING_BUFFER_H_ */
