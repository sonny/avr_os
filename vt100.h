/*
 * vt100.h
 *
 * Created: 7/19/2013 7:32:46 PM
 *  Author: Greg Cook
 *
 * Configurable terminal display with default layout
 */ 


#ifndef VT100_H_
#define VT100_H_

typedef struct {
  char r;
  char c;
} coord_t;

typedef struct {
  char start;
  char end;
} scroll_t;

typedef struct {
  coord_t pos;
  char line_count;
  char current_line;	
} region_t;

typedef struct {
  coord_t cursor;
  scroll_t scroll;
  region_t region[4];
} term_t;

extern term_t * const TERM0;

#define ESC "\x1b"

void term_init(term_t *t);
void term_display_region(term_t *t, int r, int ln, char *fmt, ...);
#endif /* VT100_H_ */
