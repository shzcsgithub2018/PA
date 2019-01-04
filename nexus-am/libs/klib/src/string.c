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
  if(src==NULL)
    return NULL;

  char *dst_itor=dst;

  do{
    *dst_itor++=*src;
  }while(*src!=0);

  return dst;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
  if(src==NULL)
    return dst;
  
  char *dst_itor=dst;
  while(*dst_itor!=0)dst_itor++;
  while(*src!=0){
    *dst_itor++=*src++;
  }
  *dst_itor=0;

  return dst;
}

int strcmp(const char* s1, const char* s2) {

  if(s1==NULL&&s2==NULL)
    return 0;
  if(s1==NULL)return -1;
  if(s2==NULL)return 1;

  while(*s1!='\0' && *s1==*s2){
    s1++;
    s2++;
  }
  return *s1-*s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* s,int c,size_t n) {
  const unsigned char uc = c;
  unsigned char *su;
  for(su = s;0 < n;++su,--n)
    *su = uc;
  return s;
}

void* memcpy(void* out, const void* in, size_t n) {
  // if(!n)
  //   return 0;

  // while(--n ){
  //   && *(char *)s1 == *(char *)s2
  //   s1 = (char *)s1 + 1;
  //   s2 = (char *)s2 + 1;
  // }
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n){
  if(!n)
    return 0;

  while(--n && *(char *)s1 == *(char *)s2){
    s1 = (char *)s1 + 1;
    s2 = (char *)s2 + 1;
  }
  return (*((unsigned char *)s1) - *((unsigned char *)s2));
}

#endif
