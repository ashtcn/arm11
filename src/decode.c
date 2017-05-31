#include "decode.h"

void decode_instruction(system_state_t *machine) {
  // PRE: Instruction is not all 0
  word_t fetched = machine->fetched_instruction;
  instruction_t *instruction = machine->decoded_instruction;
  instruction->cond = fetched >> (WORD_SIZE - 4);

  // No longer consider Cond, so remove it
  fetched &= MASK_FIRST_4;

  if ((fetched >> (WORD_SIZE - 8)) == 0xA) { // Branch iff 1010 (0xA)
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
  } else if (!machine->fetched_instruction) {
    // Halt instruction
    halt(machine);
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
  machine->decoded_instruction->immediate_value = machine->fetched_instruction
                                                & 0xFFFFFF; // First 24 bits
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
  instruction->operation = (fetched >> 21) & 0x4;
  instruction->rn = (fetched >> 16) & 0x4;
  instruction->rd = (fetched >> 12) & 0x4;
  instruction->immediate_value = fetched & 0xC;
}
