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
  operand_write(id_dest, &id_dest->val);
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
    cpu.gpr[2]._16=rtl_get_sign(&cpu.eax,2)?0xffff:0;
  }
  else {
    cpu.edx=rtl_get_sign(&cpu.eax,4)?0xffffffff:0;
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    rtl_sext(&t0,&cpu.eax,1);
    cpu.gpr[0]._16=t0&0xffff;
  }
  else {
    rtl_sext(&t0,&cpu.eax,2);
    cpu.eax=t0;
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  // Log("movsx  id_src->val=0x%x",id_src->val);
  rtl_sext(&t0, &id_src->val, id_src->width);
  // Log("t0=0x%x",t0);
  // Log("read 0x%x",vaddr_read(0x1001e1,4));
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  
  rtl_zext(&id_dest->val,&id_src->val,id_src->width);
  // Log("movzx  id_src->val=0x%x",id_src->val);
  // Log("movzx  id_dest->val=0x%x",id_dest->val);
  // Log("eip=0x%x",*eip);
  operand_write(id_dest, &id_dest->val);
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