#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  int fmt_len=strlen(fmt);
  int sign=0,count=0;
  for(int i=0;i<fmt_len;i++){
    switch(fmt[i]){
      case '%':{
        if(sign==1){
          *out++='%';
          sign=0;
        }
        else 
          sign=1;
        break;
      }
      case 'd':{
        if(sign==1){
          int tmp=va_arg(ap,int);
          while(tmp){
            *out++=tmp%10+'0';
            tmp/=10;
          }
          count++;
          sign=0;
        }
        else
          *out++='d';
        break;
      }
      case 's':{
        if(sign==1){
          char *str=va_arg(ap,char*);
          int len=strlen(str);
          while(len--)*out++=*str++;
          *out++=0;
          count++;
          sign=0;
        }
        else
          *out++='s';
        break;
      }
      default:
          *out++=fmt[i];
    }
  }
  va_end(ap);
  return count;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
