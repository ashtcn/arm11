#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include <stdint.h>
#include "global.h"
#include "instruction.h"

typedef struct {
  word_t registers[NUM_REGISTERS];
  byte_t memory[NUM_ADDRESSES];
  word_t fetched_instruction;
  instruction_t *decoded_instruction;
  bool has_fetched_instruction;
} system_state_t;

#endif
