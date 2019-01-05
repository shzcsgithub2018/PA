#include "cpu/exec.h"
#include "device/port-io.h"

void difftest_skip_ref();
void difftest_skip_dut();

make_EHelper(lidt) {
  TODO();

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  TODO();

  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  TODO();

  print_asm("iret");
}

make_EHelper(in) {
  // TODO();
  switch(id_dest->width){
    case 1:{
              // Log("i'm here");
              rtl_li(&id_dest->val,pio_read_l(id_src->val));
              Log("in 0x%x",id_dest->val);
              operand_write(id_dest,&id_dest->val);
              break;
           }
    case 2:{  
              rtl_li(&id_dest->val,pio_read_b(id_src->val));
              operand_write(id_dest,&id_dest->val);
              break;
           }
    case 4:{
              rtl_li(&id_dest->val,pio_read_w(id_src->val));
              operand_write(id_dest,&id_dest->val);
              break;
           }
  }

  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  // TODO();
  Log("out 0x%x",id_dest->val);
  switch(id_src->width){
    case 1:{
              pio_write_l(id_dest->val,id_src->val);
              break;
           }
    case 2:{  
              pio_write_b(id_dest->val,id_src->val);
              break;
           }
    case 4:{
              pio_write_w(id_dest->val,id_src->val);
              break;
           }
  }

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
