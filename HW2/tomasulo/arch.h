#include <stdbool.h>
#include "inst.h"
/********************************************
   architecture definition 
*********************************************/
/* num of RS or load/store buffer entries  */
enum rs_type {LD_BUF, ST_BUF, ADD_RS, MUL_RS};
#define NUM_LD_BUF 2
#define NUM_ST_BUF 2
#define NUM_ADD_RS 3
#define NUM_MUL_RS 2
#define NUM_RS_ENTRIES (NUM_LD_BUF+NUM_ST_BUF+NUM_ADD_RS+NUM_MUL_RS)

extern bool is_add_available;
extern bool is_mul_available;
extern bool is_mem_available;

#define MEM_SIZE 1024 /* memory size in word */
extern int mem_arr[MEM_SIZE];

/* registers */
#define NUM_REGS 16

/* execution unit latencies */
#define LAT_ADD 2 /* executed on ADD */
#define LAT_SUB 2 /* executed on ADD */
#define LAT_MUL 2 /* executed on MUL */
#define LAT_DIV 4 /* executed on MUL */
#define LAT_LD 1 /* executed on Memory Unit */
#define LAT_ST 1 /* executed on Memory Unit */

typedef struct a_reg {
  int num; /* register id starting from 0 */
  int val; /* value */
  int Qi; /* the number of the RS entry that contains the operation whose result should be stored into this reg */	
} REG;

extern REG regs[NUM_REGS];

/* data structure for an RS/LB/SB entry */
typedef struct reservation_station {
  int id;		/* RS id starting from 1 */
  bool is_busy;		/* is busy? */
  enum rs_type type; 		/* 0:LD, 1:ST, 2:ADD, 3:MUL */
  enum op_type op; 	/* ADD, SUB, MUL, DIV, LD, ST */
  int Qj, Qk; 		/* ids of the RS entries */
  int Vj, Vk; 		/* values */
  int A; 		/* address for LD/ST */
  int exec_cycles;	/* remaining execution cycles */
  int result;		/* result */
  bool in_exec;		/* is in execution? */
  bool is_result_ready;	/* is result ready? */
  int inst_num;		/* instruction number */
} RS;

extern RS rs_array[NUM_RS_ENTRIES];


void set_mem(int addr, int val);
int get_mem(int addr);
void init_mem();

void init_regs();
void print_regs();

void init_fu();

void init_rs();
void print_rs();

int obtain_available_rs(enum rs_type t);
RS * get_rs(int id);
bool is_rs_active();
void reset_rs_entry(RS * t);
