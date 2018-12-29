//#define TEST
#ifdef TEST
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
#endif
int init_monitor(int, char *[]);
void ui_mainloop(int);

int main(int argc, char *argv[]) {
#ifdef TEST
  FILE *fp=fopen("./tools/gen-expr/input","r");
  if(fp==NULL)
    return 0;
  int result;
  char buf[65576];

  while(!feof(fp)){
    if(fgets(buf,65576,fp)==NULL)
      return 0;

    sscanf(buf,"%d %s",&result,buf);
    printf("%d\t %s\n",result,buf);

    bool success=true;
    uint32_t val=expr(buf,&success);

    if(val==result)
      printf("true!\n");
    else
      printf("False!\n");
  }
  fclose(fp);
#else
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  // /* Receive commands from user. */
  ui_mainloop(is_batch_mode);
#endif
  return 0;
}
