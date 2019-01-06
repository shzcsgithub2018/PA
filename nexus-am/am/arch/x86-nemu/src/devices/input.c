#include <am.h>
#include <x86.h>
#include <amdev.h>

#define I8042_DATA_PORT 0x60
// static int keydown=0;
// static int keycode=0;
size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _KbdReg *kbd = (_KbdReg *)buf;
      uint16_t keycode=inw(I8042_DATA_PORT);
      if(keycode==_KEY_NONE)
        kbd->keycode = _KEY_NONE;
      else
        kbd->keycode = keycode;
      kbd->keydown = 0; 
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
