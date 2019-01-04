#include <am.h>

void print(const char *s) {
  for (; *s; s ++) {
    _putc(*s);
  }
}
int main() {
  for (int i = 0; i < 1000; i ++) {
    print("Hello World!\n");
  }
  return 0;
}
