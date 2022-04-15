#include <stdio.h>
#include <string.h>

#include "emulate8080.h"
#include "opcodeHandlers.h"




int tests_run = 0;

#define FAIL() printf("\nfailure in %s() line %d\n", __func__, __LINE__)
#define _assert(test) do { if (!(test)) {FAIL(); return 1; } } while (0)
#define _verify(test) do {int r=test(); tests_run++; if (r) return r; } while(0)


int mov_reg_test()
{

   State8080 *state = initilizeState();

   state->memory[0] = 0x41;
   state->pc = 0;

   state->b = 0;
   state->c = 1;

   movOpcodeHandler(state);

   uint8_t c = state->c;
   uint8_t b = state->b;

   cleanUpState(state);

   _assert(c == b);
   

   return 0;
}


int mov_to_mem_test()
{

   State8080 *state = initilizeState();

   state->memory[0] = 0x70;
   state->pc = 0;

   state->b = 42;
   state->h = 0x10;
   state->l = 0x24;

   movOpcodeHandler(state);

   uint8_t addr_value = state->memory[0x1024];
   uint8_t b = state->b;

   cleanUpState(state);

   _assert(b == addr_value);
   

   return 0;
}

int mov_from_mem_test() 
{

   State8080 *state = initilizeState();

   state->memory[0] = 0x46;
   state->pc = 0;

   state->b = 0;
   state->h = 0x10;
   state->l = 0x24;

   state->memory[0x1024] = 42;

   movOpcodeHandler(state);

   uint8_t addr_value = state->memory[0x1024];
   uint8_t b = state->b;

   cleanUpState(state);

   _assert(b == addr_value);
   

   return 0;


}

int mvi_to_reg_test() 
{

   State8080 *state = initilizeState();

   state->memory[0] = 0x06;
   state->memory[1] = 42;
   state->pc = 0;


   mviOpcodeHandler(state);

   uint8_t addr_value = state->memory[1];
   uint8_t b = state->b;


   cleanUpState(state);

   _assert(b == addr_value);
   

   return 0;


}


int mvi_to_mem_test() 
{

   State8080 *state = initilizeState();

   state->memory[0] = 0x36;
   state->memory[1] = 42;
   state->pc = 0;

   state->h = 0x10;
   state->l = 0x24;


   mviOpcodeHandler(state);

   uint8_t imm_value = state->memory[1];
   uint8_t addr_value = state->memory[0x1024];


   cleanUpState(state);

   _assert(addr_value == imm_value);
   

   return 0;


}

int lxi_to_reg_test() 
{
   State8080 *state = initilizeState();

   state->pc = 0;
   state->memory[0] = 0x01;
   state->memory[1] = 0x34;
   state->memory[2] = 0x12;

   lxiOpcodeHandler(state);

   
   uint16_t written = (state->b << 8) | state->c;

   uint16_t target_value = 0x1234;


   cleanUpState(state);

   _assert(target_value == written);

   return 0;


}

int lxi_to_sp_test() 
{

   State8080 *state = initilizeState();

   state->pc = 0;
   state->memory[0] = 0x31;
   state->memory[1] = 0x34;
   state->memory[2] = 0x12;

   state->sp = 0x1000;

   lxiOpcodeHandler(state);

   
   uint16_t written;
   memcpy(&written, &state->memory[state->sp], 2);

   uint16_t target_value = 0x1234;


   cleanUpState(state);

   _assert(target_value == written);

   return 0;


}


#define NUM_OF_TESTS sizeof(test_ptrs)/sizeof(*test_ptrs) 

int (*test_ptrs[])() = {
   mov_reg_test,
   mov_to_mem_test,
   mov_from_mem_test,
   mvi_to_reg_test,
   mvi_to_mem_test,
   lxi_to_reg_test,
   lxi_to_sp_test,
};



int all_tests() 
{
   int i;
   for (i = 0; i < NUM_OF_TESTS; i++) {
      printf("test %d\n", i+1);
      _verify(test_ptrs[i]);
      printf("--- PASSED\n");
   }
   return 0;
}


int main() 
{

   int result = all_tests();
   if (result == 0)
      printf("ALL PASSED\n");
   printf("Tests run: %d\n", tests_run);


   return result != 0;

}
