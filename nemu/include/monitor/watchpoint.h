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
WP* result_change(WP* wp);
WP* new_wp(char *args);
void free_wp(WP* wp);

#endif
