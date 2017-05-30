#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdbool.h>
#include <stdint.h>
#include "global.h"

typedef struct {
  instruction_type_t type;
  byte_t cond;
  opcode_t operation;

  uint32_t immediate_value;

  // Registers
  reg_address_t rn;
  reg_address_t rd;
  reg_address_t rs;
  reg_address_t rm;

  // Flags
  bool flag_0; // I or A
  bool flag_1; // S or P
  bool flag_2; // U
  bool flag_3; // L

  // Shifts and rotations
  shift_t shift_type;
  byte_t shift_amount;
} instruction_t;

const instruction_t NULL_INSTRUCTION = {
  .type = NUL,
  .immediate_value = 0,
  .rn = -1,
  .rd = -1,
  .rs = -1,
  .rm = -1,
  .flag_0 = false,
  .flag_1 = false,
  .flag_2 = false,
  .flag_3 = false,
  .shift_amount = 0,
};

#endif
