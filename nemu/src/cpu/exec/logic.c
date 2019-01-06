#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  // Log("eax=0x%x",cpu.eax);
  // Log("now $eip=0x%x",*eip);
  rtl_and(&t0,&id_dest->val,&id_src->val);
  // Log("dest=0x%x   src=0x%x",id_dest->val,id_src->val);
  rtl_li(&t1, 0);
  rtl_set_CF(&t1);
  rtl_set_OF(&t1);
  // Log("ZF=%d",cpu.eflages.ZF);
  rtl_update_ZFSF(&t0,id_dest->width);
  //  Log("ZF=%d",cpu.eflages.ZF);
  print_asm_template2(test);
}

make_EHelper(and) {

  // Log("esp=0x%x\n",cpu.esp);
  // Log("dest= 0x%x src=0x%x\n",id_dest->val,id_src->val);
  // Log("dest= 0x%d src=0x%x\n",id_dest->width,id_src->width);

  rtl_sext(&t0,&id_src->val,id_src->width);
  rtl_and(&id_dest->val,&id_dest->val,&t0);

  // Log("dest= 0x%x src=0x%x\n",id_dest->val,t0);

  operand_write(id_dest, &id_dest->val);

  rtl_update_ZF(&id_dest->val,id_dest->width);
  // Log("esp=0x%x\n",cpu.esp);
  // Log("dest= 0x%x src=0x%x\n",id_dest->val,id_src->val);

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&id_dest->val,&id_dest->val,&id_src->val);
  // Log("id_dest->val=%x id_src->val=%x\n",id_dest->val,id_src->val);

  
  operand_write(id_dest, &id_dest->val);
  // Log("eax=%u\n",cpu.eax);

  rtl_li(&t0,0);
  rtl_set_CF(&t0);
  rtl_set_OF(&t0);

  rtl_update_ZF(&id_dest->val,id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  // Log("edx=0x%x   eax=0x%x\n",cpu.edx,cpu.eax);
  // Log("dest->val=0x%x   src->val=0x%x",id_dest->val,id_src->val);
  rtl_or(&id_dest->val,&id_dest->val,&id_src->val);
  // Log("dest->val=0x%x   src->val=0x%x",id_dest->val,id_src->val);
  operand_write(id_dest,&id_dest->val);
  
  rtl_li(&t0,0);
  rtl_set_CF(&t0);
  rtl_set_OF(&t0);

  rtl_update_ZF(&id_dest->val,id_dest->width);
  // Log("ZF=0x%x",cpu.eflages.ZF);
  // Log("dest->val=0x%x   src->val=0x%x",id_dest->val,id_src->val);
  // Log("edx=0x%x   eax=0x%x\n",cpu.edx,cpu.eax);
  print_asm_template2(or);
}

make_EHelper(sar) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_sari(&id_dest->val,&id_dest->val,id_src->val);
  operand_write(id_dest,&id_dest->val);
  print_asm_template2(sar);
}

make_EHelper(shl) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest,&id_dest->val);
  print_asm_template2(shl);
}

make_EHelper(shr) {
  // TODO();
  // unnecessary to update CF and OF in NEMU
  rtl_shri(&id_dest->val,&id_dest->val,id_src->val);
  operand_write(id_dest,&id_dest->val);
  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);
  if(*eip==0x100bc8)
    Log("movzx  id_dest->val=0x%x",id_dest->val);
  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&id_dest->val,&id_dest->val);
  operand_write(id_dest,&id_dest->val);

  print_asm_template1(not);
}

make_EHelper(rol){
    int tmp=id_src->val;
    t3=id_dest->val;
    while(tmp-->0){
      t0=rtl_get_sign(&t3,id_dest->width);
      rtl_shli(&t1,&t3,1);
      t3=t0;
    }
    // rtl_set_CF(&t0);
    operand_write(id_dest,&t3);
    // if(id_src->val==1){
    //   rtl_get_CF(&t0);
    //   if(rtl_get_sign(&t3,id_dest->width)!=t0)
    //     t0=1;
    //   else
    //     t0=0;
    //   rtl_set_OF(&t0);
    // }

    print_asm_template1(rol);
}

make_EHelper(ror){
  int tmp=id_src->val;
    t3=id_dest->val;
    while(tmp-->0){
      t0=t3&0x1;
      rtl_sari(&t1,&t3,1);
      t3=t1|(t0<<31);
    }
    // rtl_set_CF(&t0);
    operand_write(id_dest,&t3);
    // if(id_src->val==1){
    //   rtl_get_CF(&t0);
    //   if(rtl_get_sign(&t3,id_dest->width)!=t0)
    //     t0=1;
    //   else
    //     t0=0;
    //   rtl_set_OF(&t0);
    // }

    print_asm_template1(ror);
}