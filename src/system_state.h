#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include <stdint.h>
#include "global.h"
#include "instruction.h"

typedef struct {
  word registers[NUM_REGISTERS] = {0};
  byte memory[NUM_ADDRESSES] = {0};
  word fetched_instruction;
  instruction *decoded_instruction;
  bool has_fetched_instruction = false;
} system_state;

#endif
