#include <stdbool.h>
#include "constants.h"

struct instruction {
  instruction_code code;
  cond condition;
  opcode operation;

  unsigned long immediate_value = 0;

  // Registers
  char rn = -1;
  char rd = -1;
  char rs = -1;
  char rm = -1;

  // Flags
  bool flag_0 = false; // I or A
  bool flag_1 = false; // S or P
  bool flag_2 = false; // U
  bool flag_3 = false; // L

  // Shifts and rotations
  shift shift_type;
  unsigned char shift_amount = 0;
};
