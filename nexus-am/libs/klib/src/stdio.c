#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  va_list ap;
  char out[128];
  va_start(ap,fmt);
  int n=vsprintf(out,fmt,ap);
  int len=strlen(out);
  for(int i=0;i<len;i++)
    _putc(out[i]);
  va_end(ap);
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
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
          char buf[128];
          int buf_len=0;
          while(tmp){
            buf[buf_len++]=tmp%10+'0';
            tmp/=10;
          }
          while(--buf_len>=0)*out++=buf[buf_len];
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
  *out++=0;
  return count;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap,fmt);
  int n=vsprintf(out,fmt,ap);
  va_end(ap);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
