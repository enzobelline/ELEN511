#include <stdio.h>
#include <stdlib.h>
#include "arch.h"


/*******************************
main
*******************************/
int main()
{
  int i,j;
  int done = 0;
  int cycle = 0;
  int num_issued_inst = 0;
  int num_rob_entries=0;
  int h=0;

  init_inst();
  init_fu();

  printf("============== TEST INSTRUCTION SEQUENCE ===========\n");
  print_program();

  init_rs();	// initialize RS entries
  init_regs();	// initalize registers
  init_mem();	// initialize memory

  printf("* CYCLE %d (initial state)\n",cycle);
  print_rs();	// print initial RS state
  print_regs();	// print initial register state

/*
1:ld1
2:ld2
3:st1
4:st2
5:add1
6:add2
7:add3
8:mul1
9:mul2
*/
  /* simulation loop main */
  while(!done){

    /* increment the cycle */
    cycle++;
    /********************************
     *     Step IV: Commit
     ********************************/    
     
     
     
     
     /********************************
     *     Step III: Write result 
     ********************************/
     for(i=0;i<NUM_RS_ENTRIES;i++) {
       // complete STEP III here
      if(rs_array[i].is_result_ready){//if execution is complete at R
        if((rs_array[i].op==ST)&& (rs_array[i].Qk==0)){ //if station ==store and RS[r].Qk==0
          set_mem(rs_array[i].A,rs_array[i].Vk);
          if(!is_mem_available){
            is_mem_available=true;
          }
          reset_rs_entry(&rs_array[i]);
        }
        else{//if station ==fp or load and cdb is available
          int j;
          for(j=0;j<16 ;j++){//j is num of reg
            if(regs[j].Qi==rs_array[i].id){
              regs[j].val=rs_array[i].result;
              regs[j].Qi=0;
            }
          }
          int k;
          for(k=0;k<NUM_RS_ENTRIES;k++){//j is num of reg
            if(rs_array[k].Qj==rs_array[i].id){
              rs_array[k].Vj=rs_array[i].result;
              rs_array[k].Qj=0;
            }
            if(rs_array[k].Qk==rs_array[i].id){
              rs_array[k].Vk=rs_array[i].result;
              rs_array[k].Qk=0;            
            } 
          }           
          if((rs_array[i].op==ADD)||(rs_array[i].op==SUB)||(rs_array[i].op==BNE)||(rs_array[i].op==ADDI))  {
            if(!is_add_available)
            is_add_available=true;
          }
          if((rs_array[i].op==MUL)||(rs_array[i].op==DIV))  {
            if(!is_mul_available)
            is_mul_available=true;
          }          
          if((rs_array[i].op==LD))  {
            if(!is_mem_available)
            is_mem_available=true;
          }
          reset_rs_entry(&rs_array[i]);
        }
      }
    }

    /********************************
     *     Step II: Execute 
     ********************************/
      for(i=0;i<NUM_RS_ENTRIES;i++) {
       // complete STEP II here
      bool eq0=(rs_array[i].Qj ==0) && (rs_array[i].Qk ==0);
      if(((rs_array[i].op ==ADD)||(rs_array[i].op ==ADDI))&& eq0){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_add_available){
          is_add_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].result = rs_array[i].Vj + rs_array[i].Vk;
          rs_array[i].is_result_ready=true;
        }
       }

      if(((rs_array[i].op ==SUB)||(rs_array[i].op ==BNE))&& eq0){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_add_available){
          is_add_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].result = rs_array[i].Vj - rs_array[i].Vk;
          rs_array[i].is_result_ready=true;
        }
       }
       
      if((rs_array[i].op ==MUL)&& eq0){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_mul_available){
          is_mul_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].result = rs_array[i].Vj * rs_array[i].Vk;
          rs_array[i].is_result_ready=true;
        }
       }
      if((rs_array[i].op ==DIV)&& eq0){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_mul_available){
          is_mul_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].result = rs_array[i].Vj / rs_array[i].Vk;
          rs_array[i].is_result_ready=true;
        }
       }    
      if((rs_array[i].op ==LD)&& (rs_array[i].Qj==0)){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_mem_available){
          is_mem_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].A = rs_array[i].Vj +rs_array[i].A;
          rs_array[i].result = get_mem(rs_array[i].A);
          rs_array[i].is_result_ready=true;
        }      
      }
      if((rs_array[i].op ==ST)&& (rs_array[i].Qj==0)){//if fp rs and RS[r].Qj ==0 and RS[r].Qk ==0 
        if(is_mem_available){
          is_mem_available=false;
          rs_array[i].in_exec=true;
        }
        if(rs_array[i].in_exec)
          rs_array[i].exec_cycles--;
        if(rs_array[i].exec_cycles==0){
          rs_array[i].A = rs_array[i].Vj +rs_array[i].A;
          rs_array[i].is_result_ready=true;
        }      
      } 
      
      }

    /********************************
     *     Step I: Issue 
     ********************************/

    /*  wait if no RS entry is available */
    if(num_issued_inst < NUM_OF_INST) {
      int cand_rs_id; 
      if(inst[num_issued_inst].op==ADD) cand_rs_id = obtain_available_rs(ADD_RS);
      else if(inst[num_issued_inst].op==SUB) cand_rs_id = obtain_available_rs(ADD_RS);
      else if(inst[num_issued_inst].op==MUL) cand_rs_id = obtain_available_rs(MUL_RS);
      else if(inst[num_issued_inst].op==DIV) cand_rs_id = obtain_available_rs(MUL_RS);
      else if(inst[num_issued_inst].op==LD) cand_rs_id = obtain_available_rs(LD_BUF);
      else if(inst[num_issued_inst].op==ST) cand_rs_id = obtain_available_rs(ST_BUF);
      else if(inst[num_issued_inst].op==ADDI) cand_rs_id = obtain_available_rs(ADD_RS);
      else if(inst[num_issued_inst].op==BNE) cand_rs_id = obtain_available_rs(BNE_RS);
      if(num_rob_entries<ROB_SIZE){
        int h=0;
      }

      /* if there is an available RS entry AND ROB ENTRY AVAILABLE*/
      if(cand_rs_id!=-1 && is_rob_available()) {//ROB ARRAY CHEKC
        /* issue the instruction: See Fig. 3.13 */
	      RS * curr_rs = get_rs(cand_rs_id);
        
        if(curr_rs ==NULL) {
          printf("NO RS found with the given id\n");
          exit(1);
        }

        /* normal ALU operations */
        if(inst[num_issued_inst].op==ADD || inst[num_issued_inst].op==SUB || inst[num_issued_inst].op==BNE||
          inst[num_issued_inst].op== MUL ||  inst[num_issued_inst].op==DIV || inst[num_issued_inst].op==ADDI) {
          int rd, rs, rt;
          rd = inst[num_issued_inst].rd;
          rs = inst[num_issued_inst].rs;
          rt = inst[num_issued_inst].rt;

          /* Rs */
          if(regs[rs].Qi!=0) curr_rs->Qj = regs[rs].Qi;
          else curr_rs->Vj = regs[rs].val;

          /* Rt */
          if(regs[rt].Qi!=0) curr_rs->Qk = regs[rt].Qi;
          else curr_rs->Vk = regs[rt].val;

          /* set busy */
          curr_rs->is_busy = true;
          curr_rs->op = inst[num_issued_inst].op;

          /* register update */
          regs[rd].Qi = curr_rs->id;

          /* set exec cycles */
          if(inst[num_issued_inst].op==ADD) curr_rs->exec_cycles=LAT_ADD;
          else if(inst[num_issued_inst].op==SUB) curr_rs->exec_cycles=LAT_SUB;
          else if(inst[num_issued_inst].op==MUL) curr_rs->exec_cycles=LAT_MUL;
          else if(inst[num_issued_inst].op==DIV) curr_rs->exec_cycles=LAT_DIV;
          else if(inst[num_issued_inst].op==ADDI) curr_rs->exec_cycles=LAT_ADDI;
          else if(inst[num_issued_inst].op==BNE) curr_rs->exec_cycles=LAT_BNE;

          /* num issued ++ */
          num_issued_inst++;

        } else if (inst[num_issued_inst].op==LD) {

          int rd, rs, imm;
          rd = inst[num_issued_inst].rd;
          rs = inst[num_issued_inst].rs;
          imm = inst[num_issued_inst].rt;

          /* Rs */
          if(regs[rs].Qi!=0) curr_rs->Qj = regs[rs].Qi;
          else curr_rs->Vj = regs[rs].val;

          /* addr */
          curr_rs->A = imm;

          /* set busy */
          curr_rs->is_busy = true;
          curr_rs->op = inst[num_issued_inst].op;

          /* set exec cycles */
          curr_rs->exec_cycles=LAT_LD;

          /* register update */
          regs[rd].Qi = curr_rs->id;

          /* num issued ++ */
          num_issued_inst++;

        } else if (inst[num_issued_inst].op==ST) {

          int rd, rs, imm;
          rd = inst[num_issued_inst].rd;
          rs = inst[num_issued_inst].rs;
          imm = inst[num_issued_inst].rt;

          /* Rs */
          if(regs[rs].Qi!=0) curr_rs->Qj = regs[rs].Qi;
          else curr_rs->Vj = regs[rs].val;

          /* Rd */
          if(regs[rd].Qi!=0) curr_rs->Qk = regs[rd].Qi;
          else curr_rs->Vk = regs[rd].val;
          
          /* addr */
          curr_rs->A = imm;

          /* set busy */
          curr_rs->is_busy = true;
          curr_rs->op = inst[num_issued_inst].op;

          /* set exec cycles */
          curr_rs->exec_cycles=LAT_ST;

          /* num issued ++ */
          num_issued_inst++;
        }
      }
    }


    /* print out the result */
    printf("* CYCLE %d\n",cycle);
    print_rs();
    print_regs();
   
    /* check the termination condition */ 
    if( (num_issued_inst>=NUM_OF_INST) && !is_rs_active())
      done =1;
  }
  return 0;
}
