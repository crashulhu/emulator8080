#ifndef OPCODE_HANDLER
#define OPCODE_HANDLER
#include <stdio.h>
#include <stdlib.h>

#include "emulate8080.h"


uint8_t *getRegisterFromCode(uint8_t code, struct State8080* state);


void unimplementedInstruction(struct State8080 *state);

void nopOpcodeHandler(struct State8080 *state);

// Data Transfer Group


void movOpcodeHandler(struct State8080 *state);
void mviOpcodeHandler(struct State8080 *state);
void lxiOpcodeHandler(struct State8080 *state);



#endif
