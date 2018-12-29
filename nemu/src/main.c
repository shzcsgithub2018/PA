#include "nemu.h"
#include "monitor/monitor.h"
#include <unistd.h>
#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {

  FILE *fp=fopen("./tools/gen-expr/input","r");
  if(fp==NULL)
    return 0;
  int result;
  bool success=true;
  char buf[65576];
  printf("true!\n");
  while(!feof(fp)){
    printf("true!\n");
    if(fgets(buf,65576,fp)==NULL)
      return 0;
    sscanf(buf,"%d %s",&result,buf);
    uint32_t val=expr(buf,&success);

    if(val==result)
      printf("true!\n");
    else
      printf("False!\n");
  }
  fclose(fp);
  /* Initialize the monitor. */
  // int is_batch_mode = init_monitor(argc, argv);

  // /* Receive commands from user. */
  // ui_mainloop(is_batch_mode);

  return 0;
}
