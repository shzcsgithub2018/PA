#include "klib.h"

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s==NULL)
    return 0;
  int len=0;
  while(*s!='\0'){
    len++;
    s++;
  }
  return len;
}

char *strcpy(char* dst,const char* src) {
  return NULL;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  return NULL;
}

int strcmp(const char* s1, const char* s2) {
  if(s1==NULL&&s2==NULL)
    return 0;
  if(s1==NULL)return 1;
  if(s2==NULL)return -1;

  while(*s1!='\0' && *s1==*s2){
    s1++;
    s2++;
  }
  return *s1-*s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  return 0;
}

#endif
