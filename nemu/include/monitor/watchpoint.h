#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[1000];
  uint32_t last_result;

} WP;

WP* result_change();
void new_wp(char *);
void free_wp(int);
void printf_wp();

#endif
