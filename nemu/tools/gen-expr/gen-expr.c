#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
#define Maxsize_buf 65536
#define Convergence_condition (Maxsize_buf/4/6)
static char buf[Maxsize_buf];
static int p_buf;

static inline void int_toString(int n,char *str){
  char s[6];
  int p_s=0;

  while(n-->0){
      s[p_s++]=n%10+'0';
      n/=10;
  }

  for(int i=0;i<p_s;i++)
    str[i]=s[p_s-1-i];
  str[p_s]='\0';
}

static inline void gen_rand_expr() {
   int rand_num=rand()%3;
   if(p_buf>=Convergence_condition)
      rand_num=0;

   switch (rand_num) {
    case 0: {
      int num=1+rand()%10000;//Don't allow 0;
      char to_str[6];
      int_toString(num,to_str); 
      for(int i=0;i<strlen(to_str);i++)
        buf[p_buf++]=to_str[i];
      break;
    }
    case 1: {
      buf[p_buf++]='(';
      gen_rand_expr(); 
      buf[p_buf++]=')';
      break;
    }
    default: {
      gen_rand_expr(); 
      switch(rand()%4){
        case 0:
          buf[p_buf++]='+';
          break;
        case 1:
          buf[p_buf++]='-';
          break;
        case 2:
          buf[p_buf++]='*';
          break;
        default:
          buf[p_buf++]='/';
          break;
      }
      gen_rand_expr(); 
      break;
    }
  }
  buf[p_buf] = '\0';
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s ;"
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;

  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }

  int i;
  for (i = 0; i < loop; i ++) {
    p_buf=0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
