/*
 * echo.c
 *
 * Created: 12/3/2013 8:09:45 PM
 *  Author: Greg Cook
 */ 

#include "echo.h"
#include "task.h"
#include "vt100.h"
#include <stdio.h>
#include <ctype.h>

static task_t * echo_task;

static task_slice_result_t echo_task_callback(task_t *t) {
  int c = fgetc(stdin);
  while (c != EOF) {
    if (c == '\r' || c== '\n')
      printf("\n\r");
    else
      fputc(toupper(c), stdout);
    c = fgetc(stdin);
  }

  return (task_slice_result_t){0, TASK_RESCHED};
}

static const task_slice_callback_fp const echo_task_slices[] = {
  echo_task_callback
};

void echo_init(void) {
  echo_task = Task.new(echo_task_slices, NULL, 125, true);
  Task.schedule(echo_task, TASK_RESCHED);
}
