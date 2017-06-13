/**
 * @file encode.c
 * @brief Functions to encode an instruction_t
 */

#include "encode.h"

word_t encode(instruction_t *instruction) {
  switch (instruction->type) {
    case DPI:
      return encode_dpi(instruction);
      break;
    case MUL:
      return encode_mul(instruction);
      break;
    case SDT:
      return encode_sdt(instruction);
      break;
    case BRA:
      return encode_branch(instruction);
      break;
    case ZER:
      return 0;
      break;
    case NUL:
    default:
      fprintf(stderr, "Incorrect instruction type");
      return 0;
      break;
  }
}

/**
 * @brief Returns the condition code in the correct bit position
 *
 * @param instruction Given instruction
 * @returns The condiiton code
 */
word_t add_cond(instruction_t *instruction) {
  return ((word_t) instruction->cond) << (WORD_SIZE - 4);
}

/**
 * @brief Encodes data processing instructions
 *
 * Given a data processing instruction this will return the 32 bit instruction
 * corresponding with given instruction_t
 *
 * @param instruction Given instruction
 * @returns 32 bit instruction
 */
word_t encode_dpi(instruction_t *instruction) {
  word_t binary = 0;
  binary |= ((word_t) instruction->operation) << 21;
  binary |= ((word_t) instruction->flag_0) << 25;
  binary |= ((word_t) instruction->flag_1) << 20;
  if (instruction->rd != -1) {
    binary |= ((word_t) instruction->rd) << 12;
  }
  if (instruction->operation != MOV) {
    binary |= ((word_t) instruction->rn) << 16;
  }
  if (instruction->flag_0) {
    binary |= instruction->immediate_value;
    binary |= (0xF & instruction->shift_amount) << 8;
  } else {
    binary |= instruction->rm;
    binary |= ((word_t) instruction->shift_type) << 5;
    if (instruction->rs != -1) {
      binary |= 1L << 4;
      // Shift by a register
      binary |= ((word_t) instruction->rs) << 8;
    } else {
      // Shift by a constant amount
      binary |= instruction->shift_amount << 7;
    }
  }
  return binary | add_cond(instruction);
}

/**
 * @brief Encodes multiply instructions
 *
 * Given a multiply instruction this will return the 32 bit instruction
 * corresponding with given instruction_t
 *
 * @param instruction Given instruction
 * @returns 32 bit instruction
 */
word_t encode_mul(instruction_t *instruction) {
  word_t binary = 0x90;
  binary |= ((word_t) instruction->rd) << 16;
  binary |= ((word_t) instruction->rs) << 8;
  binary |= ((word_t) instruction->rm);
  binary |= ((word_t) instruction->flag_0) << 21;
  binary |= ((word_t) instruction->flag_1) << 20;
  if (instruction->flag_0) {
    binary |= ((word_t) instruction->rn) << 12;
  }
  return binary | add_cond(instruction);
}


/**
 * @brief Encodes single data transfer instructions
 *
 * Given a single data transfer instruction this will return the 32 bit
 * instruction corresponding with given instruction_t
 *
 * @param instruction Given instruction
 * @returns 32 bit instruction
 */
word_t encode_sdt(instruction_t *instruction) {
  word_t binary = 0x04000000;
  binary |= ((word_t) instruction->flag_0) << 25;
  binary |= ((word_t) instruction->flag_1) << 24;
  binary |= ((word_t) instruction->flag_2) << 23;
  binary |= ((word_t) instruction->flag_3) << 20;
  if (-1 != instruction->rn) {
    binary |= ((word_t) instruction->rn) << 16;
  }
  binary |= ((word_t) instruction->rd) << 12;
  if (instruction->flag_0) {
    if (-1 != instruction->rm) {
      binary |= instruction->rm;
    }
    binary |= ((word_t) instruction->shift_type) << 5;

    if (instruction->rs != -1) {
      binary |= 1L << 4;
      // Shift by a register
      binary |= ((word_t) instruction->rs) << 8;
    } else {
      // Shift by a constant amount
      binary |= ((0x1F) & instruction->shift_amount) << 7;
    }
  } else {
    binary |= ((word_t) instruction->immediate_value) & 0xFFF;
  }
  return binary | add_cond(instruction);
}

/**
 * @brief Encodes branch instructions
 *
 * Given a branch instruction this will return the 32 bit
 * instruction corresponding with given instruction_t
 *
 * @param instruction Given instruction
 * @returns 32 bit instruction
 */
word_t encode_branch(instruction_t *instruction) {
  word_t binary = 0x0A000000;
  binary |= 0xFFFFFF & instruction->immediate_value;
  return binary | add_cond(instruction);
}
