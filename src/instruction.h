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

#endif
