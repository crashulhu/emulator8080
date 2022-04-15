#ifndef DISASS8080
#define DISASS8080
#include <stdint.h>

enum InstructionGroup {
   Invalid=0,
   DataTransfer,
   Arithmetic,
   Logical,
   Branch,
   Special,

};

enum RegisterCodes {
   A=7,
   B=0,
   C=1,
   D=2,
   E=3,
   H=4,
   L=5
};

enum RegisterPairs {
   BC=0,
   DE=1,
   HL=2,
   SP=3,
};

typedef struct ConditionsCodes {
   uint8_t       z:1;
   uint8_t       s:1;
   uint8_t       p:1;
   uint8_t       cy:1;
   uint8_t       ac:1;
   uint8_t       pad:3;
} ConditionCodes;

typedef struct State8080 {
   uint16_t  a;
   uint8_t  b;
   uint8_t  c;
   uint8_t  d;
   uint8_t  e;
   uint8_t  h;
   uint8_t  l;
   uint16_t sp;
   uint16_t pc;
   uint8_t *memory;
   struct ConditionsCodes cc;
   uint8_t int_enable;
} State8080;


typedef struct Instruction {
   unsigned char opcode;
   char name[20];
   int size;
   enum InstructionGroup group;
   void (*opcode_handler)(struct State8080*);
} Instruction;


// Globals
Instruction instructions_8080[0x100];



// Function Signatures

State8080 *initilizeState();
int loadIntoMemory(State8080*, unsigned char *);
void disass8080(unsigned char *buf);
void cleanUpState(State8080 *);








#endif
