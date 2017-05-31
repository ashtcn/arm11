#include "decode.h"

void decode_instruction(system_state_t *machine) {
  // PRE: Instruction is not all 0
  word_t fetched = machine->fetched_instruction;
  instruction_t *instruction = machine->decoded_instruction;
  instruction->cond = fetched >> (WORD_SIZE - 4);

  // No longer consider Cond, so remove it
  fetched &= MASK_FIRST_4;

  if (!machine->fetched_instruction) {
    // Halt instruction
    halt(machine);
  } else if ((fetched >> (WORD_SIZE - 8)) == 0xA) { // Branch iff 1010 (0xA)
    // Branch
    branch(machine);
  } else if ((fetched >> (WORD_SIZE - 6)) == 0x1) {
    // Single Data Transfer
    multiply(machine);
  } else if (!(fetched >> 22) && (((fetched >> 4) & 0xF) == 0x9)) {
    //Multiply
    single_data_transfer(machine);
  } else if (!(fetched >> (WORD_SIZE - 6))) {
    // Data Processing
    data_processing(machine);
  } else {
    // Unknown instruction
    fprintf(stderr, "Unknown instruction, PC: %u", machine->registers[PC]);
    exit_program(machine);
  }
}

void halt(system_state_t *machine) {
  machine->decoded_instruction->type = ZER;
}

void branch(system_state_t *machine) {
  uint32_t offset = machine->fetched_instruction & 0xFFFFFF; // First 24 bits
  offset <<= 2;

  // Two's complement sign extention
  if ((machine->fetched_instruction >> 23) & 0x1) {
    // Number is negative
    offset |= 0xF3000000; // Pad left with 6 one's
  }
  machine->decoded_instruction->immediate_value = offset;
}

void multiply(system_state_t *machine) {
  instruction_t *instruction = machine->decoded_instruction;
  word_t fetched = machine->fetched_instruction;

  instruction->type = MUL;
  instruction->flag_0 = (fetched >> 21) & 0x1;
  instruction->flag_1 = (fetched >> 20) & 0x1;
  instruction->rn = (fetched >> 12) & 0x4;
  instruction->rd = (fetched >> 16) & 0x4;
  instruction->rs = (fetched >> 8) & 0x4;
  instruction->rm = fetched & 0x4;
}

void single_data_transfer(system_state_t *machine) {

}

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
    instruction->immediate_value = fetched & 0xFF;
    instruction->shift_type = ror;
    instruction->shift_amount = (fetched >> 8) & 0xF;
  } else {
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
