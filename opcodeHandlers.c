#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emulate8080.h"
#include "opcodeHandlers.h"

uint8_t *getRegisterFromCode(uint8_t code, struct State8080* state) 
{

   uint8_t *reg = NULL;
   switch (code) {
      case A:
         reg = &state->a;
         break;
      case B: 
         reg = &state->b;
         break;
      case C:
         reg = &state->c;
         break;
      case D:
         reg = &state->d;
         break;
      case E:
         reg = &state->e;
         break;
      case H:
         reg = &state->h;
         break;
      case L:
         reg = &state->l;
         
   }

   return reg;
}

void translate_reg_pair(uint8_t code, struct State8080 *state, uint8_t **high, uint8_t **low) 
{
   *high = NULL;
   *low = NULL;
   switch (code) {
      case BC:
         *high = &state->b;
         *low = &state->c;
         break;
      case DE:
         *high = &state->d;
         *low = &state->c;
         break;
      case HL:
         *high = &state->h;
         *low = &state->l;
   }
}


void unimplementedInstruction(struct State8080 *state)
{
   printf("invalid instruction\n");
   exit(1);
}

void nopOpcodeHandler(struct State8080 *state)
{
   return;
}


// Data Transfer Group


void movOpcodeHandler(struct State8080 *state)
{

   unsigned int r1, r2;
   unsigned int opcode;
   uint8_t *src, *dst;
   unsigned short mem_ptr = 0;

   opcode = state->memory[state->pc];

   r1 = (opcode & 0b00111000) >> 3;
   r2 = (opcode & 0b00000111);

   if (r1 == 6) {
      mem_ptr = ((state->h << 8) | state->l);
      src = getRegisterFromCode(r2, state);
      state->memory[mem_ptr] = *src;
   } else if (r2 == 6) {
      dst = getRegisterFromCode(r1, state);
      mem_ptr = ((state->h <<8) | state->l);
      *dst = state->memory[mem_ptr];
   } else {
      dst = getRegisterFromCode(r1, state);
      src = getRegisterFromCode(r2, state);
      *dst = *src;
   }
}

void mviOpcodeHandler(struct State8080 *state)
{
   unsigned int r;
   unsigned int opcode;
   unsigned int immediate;
   uint8_t *dst;
   uint16_t mem_ptr;

   opcode = state->memory[state->pc];

   immediate = state->memory[state->pc+1];

   r = (opcode & 0b00111000)>>3;
   if (r == 6){
      mem_ptr = ((state->h << 8) | state->l);
      state->memory[mem_ptr] = immediate;
   } else {
      dst = getRegisterFromCode(r, state);
      *dst = immediate;
   }

}

void lxiOpcodeHandler(struct State8080 *state)
{

   unsigned int rp;
   unsigned int opcode;
   unsigned int high_order_byte, low_order_byte;
   unsigned int immediate;
   uint8_t *high_reg=NULL; 
   uint8_t *low_reg=NULL;

   opcode = state->memory[state->pc];
   rp = (opcode & 0b00110000) >> 4;
   low_order_byte  = state->memory[state->pc+1];
   high_order_byte = state->memory[state->pc+2];
   
   translate_reg_pair(rp, state, &high_reg, &low_reg);

   if (rp == 3) {
     state->memory[state->sp] = ( high_order_byte << 8 ) + low_order_byte;
     immediate = ( high_order_byte << 8 ) + low_order_byte;
     memcpy(&state->memory[state->sp], &immediate, 2);
   } else {
      *high_reg = (uint8_t )high_order_byte;
      *low_reg  = (uint8_t )low_order_byte;
   }

   return;
}

