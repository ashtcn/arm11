#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdbool.h>
#include <stdint.h>
#include "global.h"

typedef struct {
  instruction_type type = NUL;
  byte cond;
  opcode operation;

  uint32_t immediate_value = 0;

  // Registers
  reg_address rn = -1;
  reg_address rd = -1;
  reg_address rs = -1;
  reg_address rm = -1;

  // Flags
  bool flag_0 = false; // I or A
  bool flag_1 = false; // S or P
  bool flag_2 = false; // U
  bool flag_3 = false; // L

  // Shifts and rotations
  shift shift_type;
  byte shift_amount = 0;
} instruction;

#endif
