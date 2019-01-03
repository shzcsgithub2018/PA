#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val);
  // Log("push 0x%x\n",id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val);
  // Log("pop $ebp=0x%x\n",id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {

  // Log("I'm here_1");
  rtl_sr(4,&cpu.ebp,4);
  rtl_pop(&cpu.ebp);
  // Log("I'm here_1");

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
 // Log("ecx=x%x  0x4(esp)=0x%x",vaddr_read(id_src->addr,4),vaddr_read(cpu.esp+4,4));
  operand_write(id_dest, &id_src->addr);
  //Log("ecx=x%x",cpu.ecx);
  print_asm_template2(lea);
}

make_EHelper(xchg){
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;

  // Log("eax = 0x%x\n",cpu.eax);
  // Log("ebx = 0x%x\n",cpu.ebx);
  // Log("id_dest-val=0x%x",id_dest->val);
  
  t0=id_dest->val;
  operand_write(id_dest, &cpu.eax);
  rtl_sr(0,&t0,id_dest->width);

  // Log("eax = 0x%x\n",cpu.eax);
  // Log("ebx = 0x%x\n",cpu.ebx);

  print_asm_template2(xchg);
}