#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
    static char *line_read = NULL;

    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(nemu) ");

    if (line_read && *line_read) {
        add_history(line_read);
    }

    return line_read;
}

static int cmd_c(char *args) {
    cpu_exec(-1);
    return 0;
}

static int cmd_q(char *args) {
    return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args){
    char *arg=strtok(NULL," ");
    int n=atoi(arg);
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args){
    char *arg=strtok(NULL," ");
    
    if(arg==NULL)
        return 0;

    if(strcmp(arg,"r")==0){
        printf("eax\t%-15d0x%-15x  \nebx\t%-15d0x%-15x  \necx\t%-15d0x%-15x  \nedx\t%-15d0x%-15x\n",
                    cpu.eax,cpu.eax,cpu.ebx,cpu.ebx,cpu.ecx,cpu.ecx,cpu.edx,cpu.edx);
        printf("edi\t%-15d0x%-15x  \nesi\t%-15d0x%-15x  \nesp\t%-15d0x%-15x  \nebp\t%-15d0x%-15x\n",
                    cpu.edi,cpu.edi,cpu.esi,cpu.esi,cpu.esp,cpu.esp,cpu.ebp,cpu.ebp);
        printf("eip\t%-15d0x%-15x\n",cpu.eip,cpu.eip);
    }else if(strcmp(arg,"w")==0){

    }

    return 0;
}

static int cmd_x(char *args){
    // char *arg1=strtok(NULL," ");
    // char *arg2=strtok(NULL," ");
    // Log("arg1 = %s", arg1);
    // Log("arg2 = %s", arg2);
    // if(!arg1 || !arg2)
    //     return 0;

    // int num=atoi(arg1);
    //uint32_t *addr=strtol(arg2,NULL,0);
    vaddr_t addr;
    int num;
    if(sscanf(args,"%d%x",&num,&addr)==EOF)
        return 0;
    printf("%-15s\t      %-15s\n","addr","val");
    for(int i=0;i<num;i++,addr+=4)
        printf("0x%-15x\t0x%-15x\n",addr,vaddr_read(addr,4));

    return 0;
}

static int cmd_p(char *args){
    return 0;
}

static int cmd_w(char *args){
    return 0;
}

static int cmd_d(char *args){
    return 0;
}
static struct {
    char *name;
    char *description;
    int (*handler) (char *);
} cmd_table [] = {
    { "help", "Display informations about all supported commands", cmd_help },
    { "c", "Continue the execution of the program", cmd_c },
    { "q", "Exit NEMU", cmd_q },
    { "si", "Step Num instructions or step one instructions",cmd_si},
    { "info", "print Register status or Monitoring Point information",cmd_info},
    { "x", "Scan memory",cmd_x},
    { "p", "Expression evaluation",cmd_p},
    { "w", "Setting up monitoring points",cmd_w},
    { "d", "Delete monitoring points",cmd_d},
    /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if (arg == NULL) {
        /* no argument given */
        for (i = 0; i < NR_CMD; i ++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else {
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(arg, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}

void ui_mainloop(int is_batch_mode) {
    if (is_batch_mode) {
        cmd_c(NULL);
        return;
    }

    for (char *str; (str = rl_gets()) != NULL; ) {
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL) { continue; }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }

#ifdef HAS_IOE
        extern void sdl_clear_event_queue(void);
        sdl_clear_event_queue();
#endif

        int i;
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) { return; }
                break;
            }
        }

        if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
    }
}
