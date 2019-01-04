#include "cpu/exec.h"

make_EHelper(add) {
  // Log("ebx=0x%x",cpu.ebx);
  // Log("fasfdfaff");
  Log("id_dest->val=0x%x id_src->val=0x%x",id_dest->val,id_src->val);
  rtl_add(&t1,&id_dest->val,&id_src->val);
  operand_write(id_dest,&t1);
  rtl_update_ZFSF(&t1,id_dest->width);
  // if(t1<id_src->val)
  //   rtl_li(&t0,1);
  // else
  //   rtl_li(&t0,0);
  // rtl_set_CF(&t0);

  t3=rtl_get_sign(&id_src->val,4);
  at=rtl_get_sign(&id_dest->val,4);
  t2=rtl_get_sign(&t1,id_dest->width);
  Log("result=0x%x   id_dest->val=0x%x id_src->val=0x%x",t1,id_dest->val,id_src->val);
  Log("sign:  src=0x%x  dest=0x%x   t1=0x%x",t3,at,t2);
  if(at&t3)
    rtl_li(&t2,1);
  else
    rtl_li(&t2,0);
  rtl_set_CF(&t2);
  Log("CF=%d",cpu.eflages.CF);
  // Log("ebx=0x%x",cpu.ebx);
  print_asm_template2(add);
}

make_EHelper(sub) {
  // Log("sub   esp=0x%x\n",cpu.esp);
  // Log("dest= 0x%x src=0x%x\n",id_dest->val,id_src->val);
  rtl_sext(&t0,&id_src->val,id_src->width);
  rtl_sub(&t1,&id_dest->val,&t0);
  operand_write(id_dest,&t1);

  t3=rtl_get_sign(&t0,id_dest->width);
  at=rtl_get_sign(&id_dest->val,id_dest->width);
  t2=rtl_get_sign(&t1,id_dest->width);
  rtl_update_ZF(&t1,id_dest->width);
  rtl_update_SF(&t1,id_dest->width);
  if((!at&&t3&&t2)|| (at&&!t3&&!t2))
    rtl_li(&t2,1);
  else
    rtl_li(&t2,0);
  rtl_set_CF(&t2);
  // Log("dest= 0x%x src=0x%x\n",id_dest->val,id_src->val);
  // Log("sub   esp=0x%x\n",cpu.esp);
  print_asm_template2(sub);
}

make_EHelper(cmp) {
  rtl_sext(&t0,&id_src->val,id_dest->width);
  rtl_sub(&t1,&id_dest->val,&t0);
  // Log("t1=0x%x",t1);

  t3=rtl_get_sign(&t0,id_dest->width);
  at=rtl_get_sign(&id_dest->val,id_dest->width);
  t2=rtl_get_sign(&t1,id_dest->width);

  rtl_update_ZF(&t1,id_dest->width);
  rtl_update_SF(&t1,id_dest->width);
  if((!at&&t3&&t2)|| (at&&!t3&&!t2))
    rtl_li(&t2,1);
  else
    rtl_li(&t2,0);
  rtl_set_OF(&t2);

  print_asm_template2(cmp);
}

make_EHelper(inc) {
  // TODO();
  rtl_addi(&id_dest->val,&id_dest->val,1);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZF(&id_dest->val,id_dest->width);
  print_asm_template1(inc);
}

make_EHelper(dec) {
  // TODO();
  rtl_subi(&id_dest->val,&id_dest->val,1);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZF(&id_dest->val,id_dest->width);
  print_asm_template1(dec);
}

make_EHelper(neg) {
  // TODO();
  Log("id_dest->val=0x%x",id_dest->val);
  rtl_not(&t0,&id_dest->val);
  rtl_addi(&id_dest->val,&t0,1);
  Log("id_dest->val=0x%x\n",id_dest->val);
  operand_write(id_dest,&id_dest->val);
  print_asm_template1(neg);
}

make_EHelper(adc) {
  rtl_add(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &t2, &id_dest->val);
  rtl_get_CF(&t1);
  rtl_add(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &t2, &id_dest->val);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_not(&t0, &t0);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(adc);
}

make_EHelper(sbb) {
  rtl_sub(&t2, &id_dest->val, &id_src->val);
  rtl_setrelop(RELOP_LTU, &t3, &id_dest->val, &t2);
  rtl_get_CF(&t1);
  rtl_sub(&t2, &t2, &t1);
  operand_write(id_dest, &t2);

  rtl_update_ZFSF(&t2, id_dest->width);

  rtl_setrelop(RELOP_LTU, &t0, &id_dest->val, &t2);
  rtl_or(&t0, &t3, &t0);
  rtl_set_CF(&t0);

  rtl_xor(&t0, &id_dest->val, &id_src->val);
  rtl_xor(&t1, &id_dest->val, &t2);
  rtl_and(&t0, &t0, &t1);
  rtl_msb(&t0, &t0, id_dest->width);
  rtl_set_OF(&t0);

  print_asm_template2(sbb);
}

make_EHelper(mul) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_mul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_mul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(mul);
}

// imul with one operand
make_EHelper(imul1) {
  rtl_lr(&t0, R_EAX, id_dest->width);
  rtl_imul_lo(&t1, &id_dest->val, &t0);

  switch (id_dest->width) {
    case 1:
      rtl_sr(R_AX, &t1, 2);
      break;
    case 2:
      rtl_sr(R_AX, &t1, 2);
      rtl_shri(&t1, &t1, 16);
      rtl_sr(R_DX, &t1, 2);
      break;
    case 4:
      rtl_imul_hi(&t2, &id_dest->val, &t0);
      rtl_sr(R_EDX, &t2, 4);
      rtl_sr(R_EAX, &t1, 4);
      break;
    default: assert(0);
  }

  print_asm_template1(imul);
}

// imul with two operands
make_EHelper(imul2) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template2(imul);
}

// imul with three operands
make_EHelper(imul3) {
  rtl_sext(&t0, &id_src->val, id_src->width);
  rtl_sext(&t1, &id_src2->val, id_src->width);
  rtl_sext(&id_dest->val, &id_dest->val, id_dest->width);

  rtl_imul_lo(&t2, &t1, &t0);
  operand_write(id_dest, &t2);

  print_asm_template3(imul);
}

make_EHelper(div) {
  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_div_q(&t2, &t0, &id_dest->val);
      rtl_div_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_div64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_div64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(div);
}

make_EHelper(idiv) {
  switch (id_dest->width) {
    case 1:
      rtl_lr(&t0, R_AX, 2);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AL, &t2, 1);
      rtl_sr(R_AH, &t3, 1);
      break;
    case 2:
      rtl_lr(&t0, R_AX, 2);
      rtl_lr(&t1, R_DX, 2);
      rtl_shli(&t1, &t1, 16);
      rtl_or(&t0, &t0, &t1);
      rtl_idiv_q(&t2, &t0, &id_dest->val);
      rtl_idiv_r(&t3, &t0, &id_dest->val);
      rtl_sr(R_AX, &t2, 2);
      rtl_sr(R_DX, &t3, 2);
      break;
    case 4:
      rtl_lr(&t0, R_EAX, 4);
      rtl_lr(&t1, R_EDX, 4);
      rtl_idiv64_q(&cpu.eax, &t1, &t0, &id_dest->val);
      rtl_idiv64_r(&cpu.edx, &t1, &t0, &id_dest->val);
      break;
    default: assert(0);
  }

  print_asm_template1(idiv);
}
