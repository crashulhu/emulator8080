#include <sys/types.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "emulate8080.h"


#include "opcodeHandlers.h"

#define MEM_SIZE 0x4000



State8080 *initilizeState() 
{
   State8080 *state = malloc(sizeof(State8080));
   memset(state, 0, sizeof(State8080));

   unsigned char *memory = malloc(MEM_SIZE);
   memset(memory, 0, MEM_SIZE);

   state->memory = memory;

   return state;
}


int loadIntoMemory(State8080 *state, unsigned char *buf)
{

   size_t buf_size = malloc_usable_size(buf);

   if (buf_size > MEM_SIZE) {
      printf("Error, not enough memory\n");
      return -1;
   }

   memcpy(state->memory, buf, MEM_SIZE);
   return 0;
}


void cleanUpState(State8080 *state)
{
   if (state == NULL){
      return;
   }
   if (state->memory != NULL) {
      free(state->memory);
   }
   free(state);

}


void disass8080(unsigned char *buf) 
{
   int pc = 0;
   int opcode;
   unsigned int addr;
   Instruction inst;

   size_t buf_size = malloc_usable_size(buf);

   while (pc <= buf_size) {

      opcode = buf[pc];
      inst = instructions_8080[opcode];

      printf("%04x   ", pc);

      if (inst.size == 3) {
         addr = (buf[pc+2] << 8) | buf[pc+1];
         printf("%02x   %s $%04x\n", inst.opcode, inst.name, addr);
      } else if (inst.size == 2) {
         printf("%02x   %s #$%02x\n", inst.opcode, inst.name, buf[pc+1]);
      } else {
         printf("%02x   %s\n", inst.opcode, inst.name);
      }

      pc += inst.size;
   }
}


Instruction instructions_8080[0x100] = 
{
   {0x00, "NOP", 1},
   {0x01, "LXI B,", 3},
   {0x02, "STAX B", 1},
   {0x03, "INX B", 1},
   {0x04, "INR B", 1},
   {0x05, "DCR B", 1},
   {0x06, "MVI B,", 2, DataTransfer, mviOpcodeHandler},
   {0x07, "RLC", 1},
   {0x08, "-", 1},
   {0x09, "DAD B", 1},
   {0x0a, "LDAX B", 1},
   {0x0b, "DCX B", 1},
   {0x0c, "INR C", 1},
   {0x0d, "DCR C", 1},
   {0x0e, "MVI C,", 2, DataTransfer, mviOpcodeHandler},
   {0x0f, "RRC", 1},
   {0x10, "-", 1},
   {0x11, "LXI D,", 3},
   {0x12, "STAX D", 1},
   {0x13, "INX D", 1},
   {0x14, "INR D", 1},
   {0x15, "DCR D", 1},
   {0x16, "MVI D,", 2, DataTransfer, mviOpcodeHandler},
   {0x17, "RAL", 1},
   {0x18, "-", 1},
   {0x19, "DAD D", 1},
   {0x1a, "LDAX D", 1},
   {0x1b, "DCX D", 1},
   {0x1c, "INR E", 1},
   {0x1d, "DCR E", 1},
   {0x1e, "MVI E,", 2, DataTransfer, mviOpcodeHandler},
   {0x1f, "RAR", 1},
   {0x20, "RIM", 1},
   {0x21, "LXI H,", 3},
   {0x22, "SHLD adr", 3},
   {0x23, "INX H", 1},
   {0x24, "INR H", 1},
   {0x25, "DCR H", 1},
   {0x26, "MVI H,", 2, DataTransfer, mviOpcodeHandler},
   {0x27, "DAA", 1},
   {0x28, "-", 1},
   {0x29, "DAD H", 1},
   {0x2a, "LHLD", 3},
   {0x2b, "DCX H", 1},
   {0x2c, "INR L", 1},
   {0x2d, "DCR L", 1},
   {0x2e, "MVI L,", 2, DataTransfer, mviOpcodeHandler},
   {0x2f, "CMA", 1},
   {0x30, "SIM", 1},
   {0x31, "LXI SP,", 3},
   {0x32, "STA", 3}, // STA addr
   {0x33, "INX SP", 1},
   {0x34, "INR M", 1},
   {0x35, "DCR M", 1},
   {0x36, "MVI M,", 2, DataTransfer, mviOpcodeHandler},
   {0x37, "STC", 1},
   {0x38, "-", 1},
   {0x39, "DAD SP", 1},
   {0x3a, "LDA", 3}, // LDA addr
   {0x3b, "DCX SP", 1},
   {0x3c, "INR A", 1},
   {0x3d, "DCR A", 1},
   {0x3e, "MVI A,", 2, DataTransfer, mviOpcodeHandler},
   {0x3f, "CMC", 1},
   {0x40, "MOV B,B", 1, DataTransfer, movOpcodeHandler},
   {0x41, "MOV B,C", 1, DataTransfer, movOpcodeHandler},
   {0x42, "MOV B,D", 1, DataTransfer, movOpcodeHandler},
   {0x43, "MOV B,E", 1, DataTransfer, movOpcodeHandler},
   {0x44, "MOV B,H", 1, DataTransfer, movOpcodeHandler},
   {0x45, "MOV B,L", 1, DataTransfer, movOpcodeHandler},
   {0x46, "MOV B,M", 1, DataTransfer, movOpcodeHandler},
   {0x47, "MOV B,A", 1, DataTransfer, movOpcodeHandler},
   {0x48, "MOV C,B", 1, DataTransfer, movOpcodeHandler},
   {0x49, "MOV C,C", 1, DataTransfer, movOpcodeHandler},
   {0x4a, "MOV C,D", 1, DataTransfer, movOpcodeHandler},
   {0x4b, "MOV C,E", 1, DataTransfer, movOpcodeHandler},
   {0x4c, "MOV C,H", 1, DataTransfer, movOpcodeHandler},
   {0x4d, "MOV C,L", 1, DataTransfer, movOpcodeHandler},
   {0x4e, "MOV C,M", 1, DataTransfer, movOpcodeHandler},
   {0x4f, "MOV C,A", 1, DataTransfer, movOpcodeHandler},
   {0x50, "MOV D,B", 1, DataTransfer, movOpcodeHandler},
   {0x51, "MOV D,C", 1, DataTransfer, movOpcodeHandler},
   {0x52, "MOV D,D", 1, DataTransfer, movOpcodeHandler},
   {0x53, "MOV D,E", 1, DataTransfer, movOpcodeHandler},
   {0x54, "MOV D,H", 1, DataTransfer, movOpcodeHandler},
   {0x55, "MOV D,L", 1, DataTransfer, movOpcodeHandler},
   {0x56, "MOV D,M", 1, DataTransfer, movOpcodeHandler},
   {0x57, "MOV D,A", 1, DataTransfer, movOpcodeHandler},
   {0x58, "MOV E,B", 1, DataTransfer, movOpcodeHandler},
   {0x59, "MOV E,C", 1, DataTransfer, movOpcodeHandler},
   {0x5a, "MOV E,D", 1, DataTransfer, movOpcodeHandler},
   {0x5b, "MOV E,E", 1, DataTransfer, movOpcodeHandler},
   {0x5c, "MOV E,H", 1, DataTransfer, movOpcodeHandler},
   {0x5d, "MOV E,L", 1, DataTransfer, movOpcodeHandler},
   {0x5e, "MOV E,M", 1, DataTransfer, movOpcodeHandler},
   {0x5f, "MOV E,A", 1, DataTransfer, movOpcodeHandler},
   {0x60, "MOV H,B", 1, DataTransfer, movOpcodeHandler},
   {0x61, "MOV H,C", 1, DataTransfer, movOpcodeHandler},
   {0x62, "MOV H,D", 1, DataTransfer, movOpcodeHandler},
   {0x63, "MOV H,E", 1, DataTransfer, movOpcodeHandler},
   {0x64, "MOV H,H", 1, DataTransfer, movOpcodeHandler},
   {0x65, "MOV H,L", 1, DataTransfer, movOpcodeHandler},
   {0x66, "MOV H,M", 1, DataTransfer, movOpcodeHandler},
   {0x67, "MOV H,A", 1, DataTransfer, movOpcodeHandler},
   {0x68, "MOV L,B", 1, DataTransfer, movOpcodeHandler},
   {0x69, "MOV L,C", 1, DataTransfer, movOpcodeHandler},
   {0x6a, "MOV L,D", 1, DataTransfer, movOpcodeHandler},
   {0x6b, "MOV L,E", 1, DataTransfer, movOpcodeHandler},
   {0x6c, "MOV L,H", 1, DataTransfer, movOpcodeHandler},
   {0x6d, "MOV L,L", 1, DataTransfer, movOpcodeHandler},
   {0x6e, "MOV L,M", 1, DataTransfer, movOpcodeHandler},
   {0x6f, "MOV L,A", 1, DataTransfer, movOpcodeHandler},
   {0x70, "MOV M,B", 1, DataTransfer, movOpcodeHandler},
   {0x71, "MOV M,C", 1, DataTransfer, movOpcodeHandler},
   {0x72, "MOV M,D", 1, DataTransfer, movOpcodeHandler},
   {0x73, "MOV M,E", 1, DataTransfer, movOpcodeHandler},
   {0x74, "MOV M,H", 1, DataTransfer, movOpcodeHandler},
   {0x75, "MOV M,L", 1, DataTransfer, movOpcodeHandler},
   {0x76, "HLT", 1},
   {0x77, "MOV M,A", 1, DataTransfer, movOpcodeHandler},
   {0x78, "MOV A,B", 1, DataTransfer, movOpcodeHandler},
   {0x79, "MOV A,C", 1, DataTransfer, movOpcodeHandler},
   {0x7a, "MOV A,D", 1, DataTransfer, movOpcodeHandler},
   {0x7b, "MOV A,E", 1, DataTransfer, movOpcodeHandler},
   {0x7c, "MOV A,H", 1, DataTransfer, movOpcodeHandler},
   {0x7d, "MOV A,L", 1, DataTransfer, movOpcodeHandler},
   {0x7e, "MOV A,M", 1, DataTransfer, movOpcodeHandler},
   {0x7f, "MOV A,A", 1, DataTransfer, movOpcodeHandler},
   {0x80, "ADD B", 1},
   {0x81, "ADD C", 1},
   {0x82, "ADD D", 1},
   {0x83, "ADD E", 1},
   {0x84, "ADD H", 1},
   {0x85, "ADD L", 1},
   {0x86, "ADD M", 1},
   {0x87, "ADD A", 1},
   {0x88, "ADC B", 1},
   {0x89, "ADC C", 1},
   {0x8a, "ADC D", 1},
   {0x8b, "ADC E", 1},
   {0x8c, "ADC H", 1},
   {0x8d, "ADC L", 1},
   {0x8e, "ADC M", 1},
   {0x8f, "ADC A", 1},
   {0x90, "SUB B", 1},
   {0x91, "SUB C", 1},
   {0x92, "SUB D", 1},
   {0x93, "SUB E", 1},
   {0x94, "SUB H", 1},
   {0x95, "SUB L", 1},
   {0x96, "SUB M", 1},
   {0x97, "SUB A", 1},
   {0x98, "SBB B", 1},
   {0x99, "SBB C", 1},
   {0x9a, "SBB D", 1},
   {0x9b, "SBB E", 1},
   {0x9c, "SBB H", 1},
   {0x9d, "SBB L", 1},
   {0x9e, "SBB M", 1},
   {0x9f, "SBB A", 1},
   {0xa0, "ANA B", 1},
   {0xa1, "ANA C", 1},
   {0xa2, "ANA D", 1},
   {0xa3, "ANA E", 1},
   {0xa4, "ANA H", 1},
   {0xa5, "ANA L", 1},
   {0xa6, "ANA M", 1},
   {0xa7, "ANA A", 1},
   {0xa8, "XRA B", 1},
   {0xa9, "XRA C", 1},
   {0xaa, "XRA D", 1},
   {0xab, "XRA E", 1},
   {0xac, "XRA H", 1},
   {0xad, "XRA L", 1},
   {0xae, "XRA M", 1},
   {0xaf, "XRA A", 1},
   {0xb0, "ORA B", 1},
   {0xb1, "ORA C", 1},
   {0xb2, "ORA D", 1},
   {0xb3, "ORA E", 1},
   {0xb4, "ORA H", 1},
   {0xb5, "ORA L", 1},
   {0xb6, "ORA M", 1},
   {0xb7, "ORA A", 1},
   {0xb8, "CMP B", 1},
   {0xb9, "CMP C", 1},
   {0xba, "CMP D", 1},
   {0xbb, "CMP E", 1},
   {0xbc, "CMP H", 1},
   {0xbd, "CMP L", 1},
   {0xbe, "CMP M", 1},
   {0xbf, "CMP A", 1},
   {0xc0, "RNZ", 1},
   {0xc1, "POP B", 1},
   {0xc2, "JNZ", 3}, // JNZ addr
   {0xc3, "JMP", 3}, // JMP addr
   {0xc4, "CNZ", 3}, // CNZ addr
   {0xc5, "PUSH B", 1},
   {0xc6, "ADI", 2},
   {0xc7, "RST 0", 1},
   {0xc8, "RZ", 1},
   {0xc9, "RET", 1},
   {0xca, "JZ", 3}, // JZ addr
   {0xcb, "-", 1},
   {0xcc, "CZ", 3}, // CZ addr
   {0xcd, "CALL", 3}, // CALL addr
   {0xce, "ACI", 2}, 
   {0xcf, "RST 1", 1},
   {0xd0, "RNC", 1},
   {0xd1, "POP D", 1},
   {0xd2, "JNC", 3}, // JNC addr
   {0xd3, "OUT", 2},
   {0xd4, "CNC", 3}, // CNC addr
   {0xd5, "PUSH D", 1},
   {0xd6, "SUI", 2},
   {0xd7, "RST 2", 1},
   {0xd8, "RC", 1},
   {0xd9, "-", 1},
   {0xda, "JC", 3}, // JC addr
   {0xdb, "IN", 2},
   {0xdc, "CC", 3}, // CC addr
   {0xdd, "-", 1},
   {0xde, "SBI", 2},
   {0xdf, "RST 3", 1},
   {0xe0, "RPO", 1},
   {0xe1, "POP H", 1},
   {0xe2, "JPO", 3}, // JPO addr
   {0xe3, "XTHL", 1},
   {0xe4, "CPO", 3}, // CPO addr
   {0xe5, "PUSH H", 1},
   {0xe6, "ANI", 2},
   {0xe7, "RST 4", 1},
   {0xe8, "RPE", 1},
   {0xe9, "PCHL", 1},
   {0xea, "JPE", 3}, // JPE addr
   {0xeb, "XCHG", 1},
   {0xec, "CPE", 3}, // CPE addr
   {0xed, "-", 1},
   {0xee, "XRI", 2},
   {0xef, "RST 5", 1},
   {0xf0, "RP", 1},
   {0xf1, "POP PSW", 1},
   {0xf2, "JP", 3}, // JP addr
   {0xf3, "DI", 1},
   {0xf4, "CP", 3}, // CP addr
   {0xf5, "PUSH PSW", 1},
   {0xf6, "ORI", 2},
   {0xf7, "RST 6", 1},
   {0xf8, "RM", 1},
   {0xf9, "SPHL", 1},
   {0xfa, "JM", 3}, // JM addr
   {0xfb, "EI", 1},
   {0xfc, "CM", 3}, // CM addr
   {0xfd, "-", 1},
   {0xfe, "CPI", 2},
   {0xff, "RST 7", 1},
};


