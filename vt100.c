/*
 * vt100.c
 *
 * Created: 7/19/2013 7:36:18 PM
 *  Author: Greg Cook
 *
 * Configurable terminal display with default layout
 */ 

#include<stdio.h>
#include <stdarg.h>
#include "vt100.h"

static term_t __term0 = {
  .scroll = {	.start = 7, .end = 13 },
  .region = {	
    { .pos = { .r = 2, .c = 2 },
      .line_count = 4,
      .current_line = 0 }, 
    { .pos = { .r = 2, .c = 41 },
      .line_count = 4,
      .current_line = 0 },
    { .pos = { .r = 16, .c = 2 },
      .line_count = 4,
      .current_line = 0 },
    { .pos = { .r = 16, .c = 41 },
      .line_count = 4,
      .current_line = 0 }
  }
};

term_t * const TERM0 = &__term0;

// Save Cursor	<ESC>[s
static inline void vt100_save_cursor(void) {
  printf( ESC "[s");
}

// Unsave Cursor <ESC>[u
static inline void vt100_unsave_cursor(void) {
  printf( ESC "[u" );
}

// Cursor Down <ESC>[{COUNT}B
static inline void vt100_cursor_down(int count) {
  printf( ESC "[%dB", count);
}


// Erase Screen <ESC>[2J
static inline void vt100_erase_screen(void) {
  printf( ESC "[2J" );
}

// Home <ESC>[{row};{col}H
static inline void vt100_cursor_home(coord_t pos) {
  printf( ESC "[%d;%dH", pos.r, pos.c );
}

// scroll_screen <ESC>[{start};{end}r
static inline void vt100_scroll_screen(scroll_t scroll) {
  printf( ESC "[%d;%dr", scroll.start, scroll.end );
}

/**
 * Initialize Terminal object
 * @param t Terminal object
 * @return void
 */
void term_init(term_t *t) {
  vt100_erase_screen();
  vt100_scroll_screen(t->scroll);
}

/**
 * Print string in terminal line, similar to printf
 * @param t Terminal object 
 * @param r Region index, integer into layout object
 * @param ln Line in region, integer into Region object
 * @param fmt Format string, uses the same format as printf
 * @param ... List of Variables to print in format string, same as printf
 * @return void
 */
void term_display_region(term_t *t, int r, int ln, char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
	
  vt100_save_cursor();
  vt100_cursor_home((coord_t){t->region[r].pos.r + ln, t->region[r].pos.c});
	
  vprintf(fmt, arg);
  va_end(arg);
	
  vt100_unsave_cursor();
}
