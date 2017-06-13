/**
 * @file global.h
 * @brief Definition of useful constants and type aliases.
 */

#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdint.h>

/** The total number of registers. */
#define NUM_REGISTERS 17
/** The total number of memory addresses. */
#define NUM_ADDRESSES 65536
/** The architecture word size. */
#define WORD_SIZE 32
/** The register number of the program counter. */
#define PC 15
/** The register number of the current program status register. */
#define CPSR 16
/** A mask which removes the first 4 bits when used with bitwise and. */
#define MASK_FIRST_4 0xFFFFFFF
/** A mask which removes the first 8 bits when used with bitwise and. */
#define MASK_FIRST_8 0xFFFFFF

/** The first memory address for accessing GPIO pins. */
#define GPIO_ACCESS_START 0x20200000
/** The number of bytes allocated for accessing GPIO pins. */
#define GPIO_ACCESS_SIZE 12
/** The first memory address for clearing GPIO pins. */
#define GPIO_CLEAR_START 0x20200028
/** The number of bytes allocated for clearing GPIO pins. */
#define GPIO_CLEAR_SIZE 4
/** The first memory address for setting GPIO pins. */
#define GPIO_SET_START 0x2020001C
/** The number of bytes allocated for setting GPIO pins. */
#define GPIO_SET_SIZE 4

#define INITIAL_ARRAY_SIZE 64

/**
 * @brief A setting which determines the format of output.
 *
 * * Using COMPLIANT_MODE will print to stdout in the exact format required by
 * test cases. Only registers and memory are printed. Errors are printed to
 * stdout.
 * * Otherwise, a much more detailed ouput will be printed, including details
 * on instructions. Formatting is improved. Errors are printed to stderr. The
 * recommended setting is false.
 */
#define COMPLIANT_MODE true

/**
 * @brief An enum that identifies the type of condition.
 */
typedef enum {
  /** Equal. */
  EQ = 0,
  /** Not equal. */
  NE = 1,
  /** Greater or equal. */
  GE = 0xA,
  /** Less than. */
  LT = 0xB,
  /** Greater than. */
  GT = 0xC,
  /** Less than or equal to. */
  LE = 0xD,
  /** No condition (always). */
  AL = 0xE,
} condition_t;

/**
 * @brief An enum that identifies the format of the instruction.
 */
typedef enum {
  /** Data processing instruction. */
  DPI,
  /** Multiply instruction. */
  MUL,
  /** Single data transfer instruction. */
  SDT,
  /** Branch instruction. */
  BRA,
  /** All zero (STOP) instruction. */
  ZER,
  /** NULL (not present) instruction. */
  NUL,
} instruction_type_t;

/**
 * @brief An enum used for defining the type of shift for the shifter to use.
 */
typedef enum {
  /** Logical shift left. */
  LSL = 0,
  /** Logical shift right. */
  LSR = 1,
  /** Arithmetic shift right. */
  ASR = 2,
  /** Rotate right. */
  ROR = 3,
} shift_t;

/**
 * @brief An enum used for defining the opcode.
 */
typedef enum {
  /** And. */
  AND = 0x0,
  /** Exclusive or. */
  EOR = 0x1,
  /** Subtract. */
  SUB = 0x2,
  /** Reverse subtract. */
  RSB = 0x3,
  /** Add. */
  ADD = 0x4,
  /** And, set flags only. */
  TST = 0x8,
  /** Exclusive or, set flags only. */
  TEQ = 0x9,
  /** Subtract, set flags only. */
  CMP = 0xA,
  /** Or. */
  ORR = 0xC,
  /** Move. */
  MOV = 0xD,
} opcode_t;

/**
 * @brief An enum used for retrieving individual flag bits from CPSR register
 */
typedef enum {
  /** N (bit 4): the last result was negative. */
  N = 0x8,
  /** Z (bit 3): the last result was zero. */
  Z = 0x4,
  /** C (bit 2): the last result caused a bit to be carried out. */
  C = 0x2,
  /** V (bit 1): the last result overflowed. */
  V = 0x1,
} cpsr_flags_t;

/**
 * @brief An enum used for storing the mnemonic in assembly
 */
typedef enum {
  /** Add. */
  ADD_M,
  /** Subtract. */
  SUB_M,
  /** Reverse subtract.*/
  RSB_M,
  /** And.*/
  AND_M,
  /** Exclusive or.*/
  EOR_M,
  /** Or.*/
  ORR_M,
  /** Move.*/
  MOV_M,
  /** Test.*/
  TST_M,
  /** Is equal.*/
  TEQ_M,
  /** Compare.*/
  CMP_M,
  /** Multiply.*/
  MUL_M,
  /** Multiply with addition.*/
  MLA_M,
  /** Load.*/
  LDR_M,
  /** Store.*/
  STR_M,
  /** Branch if equal.*/
  BEQ_M,
  /** Branch if negative.*/
  BNE_M,
  /** Branch if greater than or equal.*/
  BGE_M,
  /** Branch if less than.*/
  BLT_M,
  /** Branch if greater than.*/
  BGT_M,
  /** Branch if less than or equal.*/
  BLE_M,
  /** Unconditional branch.*/
  B_M,
  /** Shift.*/
  SHIFT_M,
  /** And eq, an all zero command*/
  ANDEQ_M,
} mnemonic_t;

/** A type alias for a byte (8 bits). */
typedef uint8_t byte_t;
/** A type alias for a register number (supports up to 2^8 registers). */
typedef int8_t reg_address_t;
/** A type alias for a memory address (supports up to 2^16 addresses). */
typedef uint16_t address_t;
/** A type alias for a word (32 bits). */
typedef uint32_t word_t;

#endif
