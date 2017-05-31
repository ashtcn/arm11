#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdint.h>

#define NUM_REGISTERS 17
#define NUM_ADDRESSES 65536
#define WORD_SIZE 32
#define PC 15
#define CPSR 16
#define MASK_FIRST_4 0xFFFFFFF
#define MASK_FIRST_6 0x3FFFFFF
#define MASK_FIRST_8 0xFFFFFF

typedef enum {
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
} opcode_t;

typedef enum {
  eq = 0,
  ne = 1,
  ge = 0xA,
  lt = 0xB,
  gt = 0xC,
  le = 0xD,
  al = 0xE,
} condition_t;

typedef enum {
  lsl = 0,
  lsr = 1,
  asr = 2,
  ror = 3,
} shift_t;

/**
 * @brief An enum that identifies the format of the instruction
 */
typedef enum {
  /** Data processing instruction */
  DPI,
  /** Multiply instruction */
  MUL,
  /** Single data transfer instruction */
  SDT,
  /** Branch instruction */
  BRA,
  /** All zero (STOP) instruction */
  ZER,
  /** NULL (not present) instruction */
  NUL,
} instruction_type_t;

/**
 * @brief An enum used for retrieving individual flag bits from CPSR register
 */
typedef enum {
  /** N (bit 4): the last result was negative */
  N = 0x8,
  /** Z (bit 3): the last result was zero */
  Z = 0x4,
  /** C (bit 2): the last result caused a bit to be carried out */
  C = 0x2,
  /** V (bit 1): the last result overflowed */
  V = 0x1,
} cpsr_flags_t;

typedef uint8_t byte_t;
typedef int8_t reg_address_t;
typedef uint16_t address_t;
typedef uint32_t word_t;

#endif
