/**
 * @file decode.c
 * @brief Functions for the decode cycle.
 */

#include "decode.h"

/**
 * @brief Decodes the fetched instruction in current system state.
 *
 * Given the pointer to the current system state, it moves the
 * fetched instruction information into the decoded_instruction struct
 * (for use when executing the decoded instruction).
 * A pre-condition is that the instruction must not be all zero (type ZER).
 * @param machine The current system state.
 */
void decode_instruction(system_state_t *machine) {
  word_t fetched = machine->fetched_instruction;
  instruction_t *instruction = machine->decoded_instruction;
  instruction->cond = fetched >> (WORD_SIZE - 4);

  // No longer consider Cond, so remove it
  fetched &= MASK_FIRST_4;

  if (!machine->fetched_instruction) {
    // Halt instruction
    halt(machine);
  } else if ((fetched >> (WORD_SIZE - 8)) == 0xA) {
    // Branch
    branch(machine);
  } else if ((fetched >> (WORD_SIZE - 6)) == 0x1) {
    // Single Data Transfer
    single_data_transfer(machine);
  } else if (!(fetched >> 22) && (((fetched >> 4) & 0xF) == 0x9)) {
    //Multiply
    multiply(machine);
  } else if (!(fetched >> (WORD_SIZE - 6))) {
    // Data Processing
    data_processing(machine);
  } else {
    // Unknown instruction
    fprintf(stderr, "Unknown instruction, PC: %u", machine->registers[PC]);
    exit_program(machine);
  }
}

/**
 * @brief Sets decoded_instruction type to a stop (ZER) instruction.
 *
 * @param machine The current system state.
 */
void halt(system_state_t *machine) {
  machine->decoded_instruction->type = ZER;
}

/**
 * @brief Set branch instruction data in decoded_instruction.
 *
 * The offset (24 bits) is bit 0 to 23 of the branch instruction.
 * It is then bit shifted to the left by 2 and then sign extended to 32 bits.
 * The offset is stored in the immediate_value of the decoded_instruction.
 * @param machine The current system state.
 */
void branch(system_state_t *machine) {
  machine->decoded_instruction->type = BRA;
  uint32_t offset = machine->fetched_instruction & 0xFFFFFF; // Last 24 bits
  offset <<= 2;

  // Two's complement sign extention
  if ((machine->fetched_instruction >> 23) & 0x1) {
    // Number is negative
    offset |= 0xFC000000; // Pad left with 6 one's
  }
  machine->decoded_instruction->immediate_value = offset;
}

/**
 * @brief Set multiply instruction data in decoded_instruction.
 *
 * The fields in decoded_instruction are used as follows:
 * * flag_0 stores the A bit (if set, perform multiply and accumulate).
 * * flag_1 stores the S bit (if set, CPSR flags are set when executed).
 * * rd is the destination register address.
 * * rn, rs and rm are the addresses of the operand registers.
 * @param machine The current system state.
 */
void multiply(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = MUL;
  instruction->flag_0 = (fetched >> 21) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->rn = (fetched >> 12) & 0xF;
  instruction->rd = (fetched >> 16) & 0xF;
  instruction->rs = (fetched >> 8) & 0xF;
  instruction->rm = fetched & 0xF;
}

/**
 * @brief Set single_data_transfer instruction data in decoded_instruction.
 *
 * The fields in decoded_instruction are used as follows:
 * * flag_0 stores the I bit:
 *   * If set, Offset is used as a shifted register.
 *   * Otherwise, Offset is used as an unsigned 12 bit immediate offset).
 * * flag_1 stores the P bit:
 *   * If set, pre-indexing is used.
 *   * Otherwise, post-indexing is used.
 * * flag_2 stores the U bit:
 *   * If set, Offset is added to the base register.
 *   * Otherwise, Offset is subtracted from the base register.
 * * flag_3 stores the L bit:
 *   * If set, the word is loaded from memory.
 *   * Otherwise, the word is stored into memory.
 * * rd is the source/destination register address.
 * * rn is the base register.
 * @param machine The current system state.
 */
void single_data_transfer(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = SDT;
  instruction->flag_0 = (fetched >> 25) & 0x1;
  instruction->flag_1 = (fetched >> 24) & 0x1;
  instruction->flag_2 = (fetched >> 23) & 0x1;
  instruction->flag_3 = (fetched >> 20) & 0x1;
  instruction->rn = (fetched >> 16) & 0xF;
  instruction->rd = (fetched >> 12) & 0xF;

  if (instruction->flag_0) {
    // Register offset
    instruction->rm = fetched & 0xF;

    if ((fetched >> 4) & 0x1) {
      // Shift by a register
      instruction->shift_type = (fetched >> 5) & 0x3;
      instruction->rs = (fetched >> 8) & 0xF;
    } else {
      // Shift by a constant amount
      instruction->shift_type = (fetched >> 5) & 0x3;
      instruction->shift_amount = (fetched >> 7) & 0x1F;
    }
  } else {
    // Immediate offset
    instruction->immediate_value = fetched & 0xFFF;
  }
}

/**
 * @brief Set data_processing instruction data in decoded_instruction.
 *
 * The fields in decoded_instruction are used as follows:
 * * flag_0 stores the I bit:
 *   * If set, the Operand2 is used as an immediate constant.
 *   * Otherwise, Operand2 is used as a shifted register.
 * * flag_1 stores the S bit (if set, CPSR flags are set when executed).
 * * operation is used to store the corresponding opcode_t enum to the opcode.
 *   provided in the fetched_instruction.
 * * rd is the source/destination register address.
 * * rn is the first operand register.
 * @param machine The current system state.
 */
void data_processing(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = DPI;
  instruction->flag_0 = (fetched >> 25) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->operation = (fetched >> 21) & 0xF;
  instruction->rn = (fetched >> 16) & 0xF;
  instruction->rd = (fetched >> 12) & 0xF;

  if (instruction->flag_0) {
    // Immediate operand
    instruction->immediate_value = fetched & 0xFF;
    instruction->shift_type = ROR;
    instruction->shift_amount = ((fetched >> 8) & 0xF) << 1;
  } else {
    // Register operand
    instruction->rm = fetched & 0xF;

    if ((fetched >> 4) & 0x1) {
      // Shift by a register
      instruction->shift_type = (fetched >> 5) & 0x3;
      instruction->rs = (fetched >> 8) & 0xF;
    } else {
      // Shift by a constant amount
      instruction->shift_type = (fetched >> 5) & 0x3;
      instruction->shift_amount = (fetched >> 7) & 0x1F;
    }
  }
}
