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

word_t add_cond(instruction_t *instruction) {
  return ((word_t) instruction->cond) << (WORD_SIZE - 4);
}

word_t encode_dpi(instruction_t *instruction) {
  word_t binary = 0;
  binary |= ((word_t) instruction->operation) << 21;
  binary |= ((word_t) instruction->flag_0) << 25;
  binary |= ((word_t) instruction->flag_1) << 20;
  binary |= ((word_t) instruction->rd) << 12;
  if (instruction->operation != MOV) {
    binary |= ((word_t) instruction->rn) << 16;
  }
  if (instruction->flag_0) {
    binary |= instruction->immediate_value;
    binary |= instruction->shift_amount << 8;
  } else {
    binary |= instruction->rm;
    binary |= ((word_t) instruction->shift_type) << 5;

    if (instruction->rs == -1) {
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

word_t encode_mul(instruction_t *instruction) {
  word_t binary = 0xA0;
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

word_t encode_sdt(instruction_t *instruction) {
  word_t binary = 0x04000000;
  binary |= ((word_t) instruction->flag_0) << 25;
  binary |= ((word_t) instruction->flag_1) << 24;
  binary |= ((word_t) instruction->flag_2) << 23;
  binary |= ((word_t) instruction->flag_3) << 20;
  binary |= ((word_t) instruction->rn) << 16;
  binary |= ((word_t) instruction->rd) << 12;
  if (instruction->flag_0) {
    binary |= instruction->rm;
    binary |= ((word_t) instruction->shift_type) << 5;

    if (instruction->rs == -1) {
      binary |= 1L << 4;
      // Shift by a register
      binary |= ((word_t) instruction->rs) << 8;
    } else {
      // Shift by a constant amount
      binary |= instruction->shift_amount << 7;
    }
  } else {
    binary |= ((word_t) instruction->immediate_value);
  }
  return binary | add_cond(instruction);
}

word_t encode_branch(instruction_t *instruction) {
  word_t binary = 0x0A000000;
  binary |= instruction->immediate_value;
  return binary | add_cond(instruction);
}
