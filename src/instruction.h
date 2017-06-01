/** A header to define the instruction_t type. */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdbool.h>
#include <stdint.h>
#include "global.h"

/**
 * @brief A struct that holds information about a decoded instruction.
 */
typedef struct {
  /** The type of instruction (None, Zero, DPI, MUL, SDT, or BRA). */
  instruction_type_t type;
  /** The condition code. */
  byte_t cond;
  /** The opcode, for data processing instructions. */
  opcode_t operation;

  /** An immediate offset or operand. */
  uint32_t immediate_value;

  /** Register Rn. */
  reg_address_t rn;
  /** Register Rd. */
  reg_address_t rd;
  /** Register Rs. */
  reg_address_t rs;
  /** Register Rm. */
  reg_address_t rm;

  /** Holds the I or A bit (depending on instruction type). */
  bool flag_0;
  /** Holds the S or P bit (depending on instruction type). */
  bool flag_1;
  /** Holds the U bit (SDT instructions only). */
  bool flag_2;
  /** Holds the L bit (SDT instructions only). */
  bool flag_3;

  /** The type of shift to be used. */
  shift_t shift_type;
  /** The number of shifts to be applied. */
  byte_t shift_amount;
} instruction_t;

#endif
