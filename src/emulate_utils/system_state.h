/**
 * @file system_state.h
 * @brief A header to define the system_state_t type.
 */

#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H
#include "../instruction.h"

/**
 * @brief A struct that holds information about the current system state.
 */
typedef struct {
  /** Holds the values currently held in registers. */
  word_t registers[NUM_REGISTERS];
    /** Holds the values currently held in memory. */
  byte_t memory[NUM_ADDRESSES];
    /** Holds the last fetched instruction, as a word. */
  word_t fetched_instruction;
    /** Holds the last decoded instruction, as an instruction_t type. */
  instruction_t *decoded_instruction;
    /** Whether or not the system currently has a fetched instruction. */
  bool has_fetched_instruction;
} system_state_t;

#endif
