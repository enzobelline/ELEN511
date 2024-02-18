#include "inst.h"
#include <stdio.h>

INST inst[NUM_OF_INST]; /* instruction array */ 
/******************************
* instruction initialization
******************************/
void init_inst()
{
  inst[0].num=1; inst[0].op=LD; inst[0].rd=0; inst[0].rs=1; inst[0].rt=0; // ld r0,0(r1)
  inst[1].num=2; inst[1].op=MUL; inst[1].rd=4; inst[1].rs=0; inst[1].rt=2; // mul r4,r0, r2
  inst[2].num=3; inst[2].op=ST; inst[2].rd=4; inst[2].rs=1; inst[2].rt=0; // st r4, 0(r1)
  inst[3].num=4; inst[3].op=ADDI; inst[3].rd=1; inst[3].rs=1; inst[3].rt=1; // addi r1, r1, 1
  inst[4].num=5; inst[4].op=BNE; inst[4].rd=1; inst[4].rs=2; inst[4].rt=6; // bne r1, r2, loop
  return;
}

/* print an instruction */
void print_inst(INST ins) {
  printf("I#%d\t",ins.num);
  if(ins.op==ADD) printf("add\tr%d,r%d,r%d\n",ins.rd,ins.rs,ins.rt);
  else if(ins.op==SUB) printf("sub\tr%d,r%d,r%d\n",ins.rd,ins.rs,ins.rt);
  else if(ins.op==MUL) printf("mul\tr%d,r%d,r%d\n",ins.rd,ins.rs,ins.rt);
  else if(ins.op==DIV) printf("div\tr%d,r%d,r%d\n",ins.rd,ins.rs,ins.rt);
  else if(ins.op==LD) printf("ld\tr%d,%d(r%d)\n",ins.rd,ins.rt,ins.rs);
  else if(ins.op==ST) printf("st\tr%d,%d(r%d)\n",ins.rd,ins.rt,ins.rs);
  else if(ins.op==ADDI) printf("addi\tr%d,%d(r%d)\n",ins.rd,ins.rt,ins.rs);
  else if(ins.op==BNE) printf("bne\tr%d,%d(r%d)\n",ins.rd,ins.rt,ins.rs);  
  else printf("unknown\n");
}

void print_program() {
  int i=0;
  for(i=0;i<NUM_OF_INST;i++) print_inst(inst[i]);
}


