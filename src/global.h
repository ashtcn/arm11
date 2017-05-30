#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdint.h>

#define NUM_REGISTERS 17
#define NUM_ADDRESSES 65536
#define WORD_SIZE 32
#define PC 15
#define CPSR 16
#define MASK_FIRST_4 ((~0) >> 4)
#define MASK_FIRST_6 ((~0) >> 6)
#define MASK_FIRST_8 ((~0) >> 8)

enum opcode {
  AND = 0,
  EOR = 1,
  SUB = 2,
  RSB = 3,
  ADD = 4,
  TST = 0x8,
  TEQ = 0x9,
  CMP = 0xA,
  ORR = 0xC,
  MOV = 0xD,
};

enum condition {
  eq = 0,
  ne = 1,
  ge = 0xA,
  lt = 0xB,
  gt = 0xC,
  le = 0xD,
  al = 0xE,
};

enum shift {
  lsl = 0,
  lsr = 1,
  asr = 2,
  ror = 3,
};

enum instruction_type {
  DPI = 0,
  MUL = 1,
  SDT = 2,
  BRA = 3,
};

enum cpsr_flags {
  N = 0x8,
  Z = 0x4,
  C = 0x2,
  V = 0x1,
};

typedef uint8_t byte;
typedef int8_t reg_address;
typedef uint16_t address;
typedef uint32_t word;

#endif
